use nom::character::complete::{alpha1, newline, space0, space1, i64};
use nom::multi::{separated_list1, many1};
use nom::{IResult, bytes::complete::tag, sequence::tuple, combinator::opt};
use std::collections::BTreeMap;

use crate::Pair;

pub fn reduce_file<'a>(input: &'a Vec<(Pair<'a>,Vec<Pair<'a>>)>) -> BTreeMap<Pair<'a>,Vec<Pair<'a>>> {
    let mut answer : BTreeMap<Pair<'a>,Vec<Pair<'a>>> = BTreeMap::new();

    for x in input {
        answer.entry(x.0).or_insert(x.1.clone());
    }

    for (_k, v) in &mut answer {
        v.sort();
    } // impose an order

    return answer;
}

pub fn parse_file(input: &str) -> IResult<&str, Vec<(Pair,Vec<Pair>)>> {
    many1(parse_line)(input)
}

fn parse_line(input: &str) -> IResult<&str, ( Pair, Vec<Pair> ) > {
    let (remaining, parse) = tuple((
        parse_num_alpha_list,
        tag("=>"), 
        parse_num_alpha, 
        opt(tag("\r")), 
        opt(newline)))(input)?;
        Ok((remaining, (parse.2,parse.0)))
}

fn parse_num_alpha_list(input: &str) -> IResult<&str, Vec<Pair>> {
    separated_list1(tag(","), parse_num_alpha)(input)
}

fn parse_num_alpha(input: &str) -> IResult<&str,Pair> {

    let (remaining,parse) = tuple((
        space0,
        i64,
        space1,
        alpha1,
        space0
    ))(input)?;
    Ok((remaining,Pair {name: parse.3, count: parse.1}))
}

#[test]
fn test_parse_line() {
    let line = "1 A, 2 B => 3 C\r\n";
    let parsed = parse_line(line).unwrap().1;

    assert_eq!(parsed.0, Pair { name : "C", count : 3});
    assert_eq!(parsed.1, vec![ Pair { name : "A", count : 1 }, Pair { name : "B", count : 2 }]);
}

#[test]
fn test_num_alpha_list() {
    let nal_1 = "1 A, 2 B, 3 C";
    let nal_2 = "     4      D,                  5 E,6 F          ";

    let parsed_1 = parse_num_alpha_list(nal_1).unwrap().1;
    let parsed_2 = parse_num_alpha_list(nal_2).unwrap().1;

    assert_eq!(parsed_1, vec![Pair{name : "A", count : 1}, Pair{name : "B", count : 2}, Pair{name : "C", count : 3}, ]);
    assert_eq!(parsed_2, vec![Pair{name : "D", count : 4}, Pair{name : "E", count : 5}, Pair{name : "F", count : 6}, ]);
}

#[test] 
fn test_num_alpha() {
    let na_1 = "3 things";
    let na_2 = " 123453      items";

    let parsed_1 = parse_num_alpha(na_1);
    let parsed_2 = parse_num_alpha(na_2);

    assert_eq!(parsed_1.as_ref().unwrap().1.name,"things");
    assert_eq!(parsed_1.as_ref().unwrap().1.count,3);
    assert_eq!(parsed_2.as_ref().unwrap().1.name,"items");
    assert_eq!(parsed_2.as_ref().unwrap().1.count,123453);
}