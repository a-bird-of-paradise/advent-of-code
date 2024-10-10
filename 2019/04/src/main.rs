use std::env;

fn main() {
    let arg : String = env::args().nth(1).expect("Need an input string.");
    let numbers : Vec<i64> = arg
        .trim()
        .split('-')
        .map(|s| s.parse::<i64>().expect("Couldn't parse integer"))
        .collect();

    if numbers.len() != 2 { panic!("Need two numbers in the form x-y."); }
    if numbers[0] >= numbers[1] { panic!("Larger number should come second."); }
    if numbers[0] < 0 { panic!("Must have non negative first number."); }

    let mut answer_1: i64 = 0; 
    let mut answer_2 : i64 = 0;

    for i in numbers[0]..numbers[1] {
        if has_adjacent(i) && nondecreasing(i) { 
            answer_1 = answer_1 + 1;
            if has_adjacent_pair(i) {
                answer_2 = answer_2 + 1;
            }
        }
    }

    println!("{answer_1}");
    println!("{answer_2}");
}

fn has_adjacent(x : i64) -> bool {
    let mut prior : char = '!';
    let mut done_first : bool = false;
    let mut answer: bool = false;

    for i in x.to_string().chars() {
        if !done_first {
            prior = i;
            done_first = true;
            continue;
        }
        if prior == '!' { panic!("Never happens"); }
        if i == prior { answer = true; }
        prior = i;
    }
    return answer;
}

fn nondecreasing(x : i64) -> bool {
    let mut remaining : i64 = x / 10; 
    let mut prior_digit : i64 = x % 10; 
    let mut answer : bool = true; 

    while remaining > 0 {
        let current_digit: i64 = remaining % 10; 
        remaining = remaining / 10; 
        if current_digit > prior_digit { answer = false; }
        prior_digit = current_digit;
    }

    return answer; 
}

fn has_adjacent_pair(x : i64) -> bool {
    let mut prior : char = '!';
    let mut done_first : bool = false;
    let mut answer: bool = false;
    let mut seen_prior : i64 = 0; 

    for i in x.to_string().chars() {
        if !done_first {
            prior = i;
            done_first = true;
            seen_prior = 1; 
            continue;
        }
        if prior == '!' { panic!("Never happens"); }
        if i == prior { seen_prior = seen_prior + 1; }
        else if seen_prior == 2 { answer = true; seen_prior = 1; }
        else { seen_prior = 1; }
        prior = i;
    }
    if seen_prior == 2 { answer = true; }
    return answer;
}

#[test]
fn test_has_adjacent () {
    assert!(has_adjacent(111111));
    assert!(has_adjacent(223450));
    assert!(!has_adjacent(123789));
}

#[test]
fn test_nondecreasing () {
    assert!(nondecreasing(111111));
    assert!(!nondecreasing(223450));
    assert!(nondecreasing(123789));
}
#[test]
fn test_has_adjacent_pair () {
    assert!(has_adjacent_pair(112233));
    assert!(!has_adjacent_pair(123444));
    assert!(has_adjacent_pair(111122));
}
