use nom::bytes::complete::tag;
use nom::character::complete::{i64, line_ending};
use nom::sequence::terminated;
use nom::Parser;
use nom::{
    IResult,
    branch::alt,
    combinator::opt,
    multi::many1
};

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Instruction {
    DealIntoNewStack,
    CutNCards,
    DealWithIncrementN
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Action {
    pub inst : Instruction,
    pub param : i64
}

pub fn read_lines(input: &str) -> IResult<&str,Vec<Action>>
{
    many1(read_line).parse(input)
}

fn read_line(input: &str) -> IResult<&str,Action> {
    let (input, parsed) = terminated(
        alt((
            deal_into_new_stack,
            cut_n,
            deal_with_increment_n
        )),
        opt(line_ending)
    ).parse(input)?;
    Ok((input, parsed))
}

fn deal_into_new_stack(input: &str) -> IResult<&str,Action> {
    let (remaining, _) = tag("deal into new stack")(input)?;
    Ok((remaining, Action { inst: Instruction::DealIntoNewStack, param : 0 } ))
}

fn cut_n(input: &str) -> IResult<&str,Action> {
    let (input, _) = tag("cut ")(input)?;
    let (input, param) = i64(input)?;
    Ok((input, Action { inst: Instruction::CutNCards, param: param}))
}

fn deal_with_increment_n(input: &str) -> IResult<&str,Action> {
    let (input, _) = tag("deal with increment ")(input)?;
    let (input, param) = i64(input)?;
    Ok((input, Action { inst: Instruction::DealWithIncrementN, param: param}))
}

#[test]
fn test_deal_into_new_stack() {
    let parsed = deal_into_new_stack("deal into new stack");
    assert!(parsed.unwrap().1 == Action { inst : Instruction::DealIntoNewStack, param : 0});
}
#[test]
fn test_cut_n() {
    let parsed = cut_n("cut 1234");
    assert!(parsed.unwrap().1 == Action { inst : Instruction::CutNCards, param : 1234});
}
#[test]
fn test_deal_with_increment_n() {
    let parsed = deal_with_increment_n("deal with increment -1234");
    assert!(parsed.unwrap().1 == Action { inst : Instruction::DealWithIncrementN, param : -1234});
}