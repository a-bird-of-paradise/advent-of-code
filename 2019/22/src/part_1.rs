
use crate::parser;
use parser::{Instruction,Action,read_lines};

pub fn do_part_1(raw_file: &str) -> usize {
    let action_list = read_lines(&raw_file).unwrap().1;
    let mut deck = make_factory_deck(10007);
    apply_actions(&action_list, &mut deck);
    deck.iter().position(|&x| x == 2019).unwrap()
}

fn apply_actions(action_list: &Vec<Action>, deck: &mut Vec<usize>) {
    for action in action_list {
        match action.inst {
            Instruction::DealIntoNewStack => deal_into_new_stack(deck),
            Instruction::CutNCards => cut_n(deck, action.param),
            Instruction::DealWithIncrementN => deal_increment_n(deck, action.param)
        }
    }
}

fn deal_into_new_stack(deck: &mut Vec<usize>) {
    deck.reverse();
}

fn cut_n(deck: &mut Vec<usize>, n: i64) {
    if n > 0 {
        deck.rotate_left(n.try_into().unwrap());
    } else {
        deck.rotate_right((-n).try_into().unwrap());
    }
}

fn deal_increment_n (deck: &mut Vec<usize>, n : i64) {
    let len = deck.len();
    let mut answer = vec![0usize; len];
    for i in 0..len {
        answer[(i * n as usize) % len] = deck[i];
    }
    *deck = answer;
}

pub fn make_factory_deck(n: usize) -> Vec<usize> {
    let mut answer = vec![0usize; n];
    for i in 0..n {
        answer[i] = i;
    }
    answer
}

#[test]
fn test_make_factory_deck() {
    assert_eq!(make_factory_deck(10),vec![0,1,2,3,4,5,6,7,8,9]);
}

#[test]
fn test_cut_n() {
    let mut deck = make_factory_deck(10);
    cut_n(&mut deck,3);
    assert_eq!(deck,vec![3,4,5,6,7,8,9,0,1,2]);
    deck = make_factory_deck(10);
    cut_n(&mut deck, -4);
    assert_eq!(deck,vec![6,7,8,9,0,1,2,3,4,5]);
}

#[test]
fn test_deal_into_new_stack() {
    let mut deck = make_factory_deck(10);
    deal_into_new_stack(&mut deck);
    assert_eq!(deck, vec![9,8,7,6,5,4,3,2,1,0]);
}

#[test]
fn test_deal_increment_n() {
    let mut deck = make_factory_deck(10);
    deal_increment_n(&mut deck, 3);
    assert_eq!(deck, vec![0,7,4,1,8,5,2,9,6,3]);
}

#[test]
fn test_apply_actions_1() {
    let mut deck = make_factory_deck(10);
    let text = 
"deal with increment 7
deal into new stack
deal into new stack";
    apply_actions(&read_lines(text).unwrap().1, &mut deck);
    assert_eq!(deck,vec![0,3,6,9,2,5,8,1,4,7]);
}
#[test]
fn test_apply_actions_3() {
    let mut deck = make_factory_deck(10);
    let text = 
"deal with increment 7
deal with increment 9
cut -2";
    apply_actions(&read_lines(text).unwrap().1, &mut deck);
    assert_eq!(deck,vec![6,3,0,7,4,1,8,5,2,9]);
}
#[test]
fn test_apply_actions_4() {
    let mut deck = make_factory_deck(10);
    let text = 
"deal into new stack
cut -2
deal with increment 7
cut 8
cut -4
deal with increment 7
cut 3
deal with increment 9
deal with increment 3
cut -1";
    apply_actions(&read_lines(text).unwrap().1, &mut deck);
    assert_eq!(deck,vec![9,2,5,8,1,4,7,0,3,6]);
}