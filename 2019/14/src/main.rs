use std::collections::BTreeMap;
use std::env;
use std::fs;

mod parser;
use parser::{parse_file, reduce_file};

mod pair;
use pair::Pair;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");

    let part_1 = process(&raw_file, 1);

    println!("{part_1}");

    let part_2 = part_2(&raw_file);

    println!("{part_2}");

}

fn part_2(raw_file_contents: &str) -> i64 {

    let mut quantity: i64 = 1;

    let mut answer = process(raw_file_contents, quantity);
    let trillion: i64 = 1000000000000;

    // seek up to first quantity that needs over a trillion ore

    while answer < trillion {
        quantity *= 2; 
        answer = process(raw_file_contents, quantity);
    }

    let mut lower_quantity : i64 = quantity / 2; 
    let mut prior_midpoint : i64 = 0;
    let mut answer_2 : i64 = 0;

    // binary search - use prior_midpoint as state, stop when in steady state

    loop {
        let midpoint = lower_quantity + (quantity - lower_quantity) / 2;

        if prior_midpoint == midpoint { break; }

        let mid_answer = process(&raw_file_contents, midpoint); 
        answer_2 = midpoint;

        if mid_answer == trillion {
            break;
        } else if mid_answer < trillion {
            lower_quantity = midpoint;
        } else { // mid_answer > trillion
            quantity = midpoint;
        }
        prior_midpoint = midpoint;
    }

    answer_2

}

fn process(raw_file_contents: &str, quantity: i64) -> i64 {
    let parsed: Vec<(Pair<'_>, Vec<Pair<'_>>)> = parse_file(&raw_file_contents).unwrap().1;
    let reactions: BTreeMap<Pair<'_>, Vec<Pair<'_>>> = reduce_file(&parsed);
    let precursors : BTreeMap<&str, Vec<Pair>> = reactions.iter().map(|(k,v)| (k.name,v.clone())).collect();
    let produced : BTreeMap<&str,i64> = reactions.iter().map(|(k,_v)| (k.name, k.count)).collect();
    
    let mut stockpile : BTreeMap<&str, i64> = BTreeMap::new();

    produce(&reactions, &precursors, &produced, &mut stockpile, "FUEL",quantity)
}

/// depth first search to make product
fn produce<'a> (
    reactions: &BTreeMap<Pair,Vec<Pair>>, 
    precursors: &BTreeMap<&'a str, Vec<Pair<'a>>>, 
    produced: &BTreeMap<&'a str,i64> ,
    stockpile: &mut BTreeMap<&'a str, i64>,
    product: &'a str,
    mut quantity: i64) -> i64 
{
    if product == "ORE" { return quantity; }

    let quantity_made_here: i64 = produced[product];
    let multiple_made : i64;

    let current_stockpile: i64 = *stockpile.entry(product).or_insert(0);
    
    if current_stockpile >= quantity {
        stockpile.entry(product).and_modify(|v| { *v -= quantity });
        return 0;
    } else {
        quantity -= current_stockpile;
        multiple_made = round_up_to_multiple(quantity, quantity_made_here);
        stockpile.entry(product).and_modify(|v| { *v = quantity_made_here * multiple_made - quantity });
    }

    let mut precursor_product : i64 = 0;

    for precursor in &precursors[product] {
        precursor_product += produce(reactions, precursors, produced, stockpile, precursor.name, precursor.count * multiple_made);
    }

    precursor_product

}

fn round_up_to_multiple (q: i64, b: i64) -> i64 { if q % b != 0 { 1 + q / b } else { q / b } }

#[test]
fn test_p1() {
    let raw_file_contents_1: String = fs::read_to_string("testinput1.txt").expect("Should have been able to read the file");
    let raw_file_contents_2: String = fs::read_to_string("testinput2.txt").expect("Should have been able to read the file");
    let raw_file_contents_3: String = fs::read_to_string("testinput3.txt").expect("Should have been able to read the file");
    let raw_file_contents_4: String = fs::read_to_string("testinput4.txt").expect("Should have been able to read the file");
    let raw_file_contents_5: String = fs::read_to_string("testinput5.txt").expect("Should have been able to read the file");
    let quantity: i64 = 1;
    assert_eq!(process(&raw_file_contents_1, quantity),31      );
    assert_eq!(process(&raw_file_contents_2, quantity),165     );
    assert_eq!(process(&raw_file_contents_3, quantity),13312   );
    assert_eq!(process(&raw_file_contents_4, quantity),180697  );
    assert_eq!(process(&raw_file_contents_5, quantity),2210736 );
}

#[test]
fn test_p2 () {
    let raw_file_contents_3: String = fs::read_to_string("testinput3.txt").expect("Should have been able to read the file");
    let raw_file_contents_4: String = fs::read_to_string("testinput4.txt").expect("Should have been able to read the file");
    let raw_file_contents_5: String = fs::read_to_string("testinput5.txt").expect("Should have been able to read the file");
    assert_eq!(part_2(&raw_file_contents_3),82892753);
    assert_eq!(part_2(&raw_file_contents_4),5586022 );
    assert_eq!(part_2(&raw_file_contents_5),460664  );

}