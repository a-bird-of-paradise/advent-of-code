use crate::parser;
use parser::{Instruction,Action,read_lines};

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct LinearCongruentialFunction {
    pub a: i64,
    pub b: i64, 
    pub m: i64
}

impl LinearCongruentialFunction {
    pub fn new(a: i64, b: i64, m: i64)  -> Self {
        Self{ a:a, b:b, m:m }
    }

    pub fn compose(&self, g: &LinearCongruentialFunction) -> Self {
        assert_eq!(self.m, g.m);
        // rem_euclid is what you really want when you think you want the % operator 
        let new_a = ( ( (self.a as i128) * (g.a as i128) ).rem_euclid(self.m as i128) ) as i64;
        let new_b = ( ( (( self.b as i128) * (g.a as i128) ) + g.b as i128).rem_euclid(self.m as i128) ) as i64;
        Self { a: new_a, b: new_b, m: self.m }
    }

    pub fn eval(&self, x: i64) -> i64 {
        ( ( (self.a as i128) * (x as i128) + (self.b as i128) ).rem_euclid(self.m as i128) ) as i64
    }

    pub fn apply(&self, n: usize) -> Vec<usize> {
        let mut locations = Vec::<usize>::new();
        for i in 0..n {
            locations.push(self.eval(i as i64) as usize);
        }
        let mut deck = vec![0usize;locations.len()];
        for i in 0 .. n {
            deck[locations[i]] = i;
        }
        deck
    }

    pub fn power(&self, k: i64) -> LinearCongruentialFunction {
        let mut answer = Self { a: 1, b: 0, m:self.m } ;
        let mut current_pow2_func = self.clone();
        let mut k_ = k;

        while k_ > 0 {
            if k_ % 2 != 0 { 
                answer = answer.compose(&current_pow2_func);
            }
            k_ /= 2;
            current_pow2_func = current_pow2_func.compose(&current_pow2_func);
        }
        answer
    }
}

impl Default for LinearCongruentialFunction {
    fn default() -> Self {
        Self { a: 0, b: 0, m: 1 }
    }
}

impl From<(Action,usize)> for LinearCongruentialFunction {
    fn from(src: (Action,usize)) -> Self {
        match src.0.inst {
            Instruction::CutNCards => Self { a: 1, b: -src.0.param, m: src.1 as i64 },
            Instruction::DealWithIncrementN => Self { a: src.0.param, b: 0, m: src.1 as i64 },
            Instruction::DealIntoNewStack => Self { a: -1, b: -1, m: src.1 as i64 }
        }
    }
}

pub fn do_part_1_again(raw_file: &str) -> usize {
    let action_list = read_lines(&raw_file).unwrap().1;
    let composed_action = compose_actions(&action_list, 10007);
    let deck = composed_action.apply(10007);
    deck.iter().position(|&x| x == 2019).unwrap()

}

pub fn compose_actions(action_list: &Vec<Action>, len: usize) -> LinearCongruentialFunction
{
    let mut composed_action = LinearCongruentialFunction { a: 1, b: 0, m: len as i64 };

    for action in action_list {
        composed_action = composed_action.compose(&(*action,len).into());
    }

    composed_action
}

#[test]
fn test_construction() {
    let action_1 = Action { inst: Instruction::CutNCards, param: 23 };
    let action_2 = Action { inst: Instruction::DealWithIncrementN, param: 23 };
    let action_3 = Action { inst: Instruction::DealIntoNewStack, param: 0 };
    let m = 10007; 

    let lcg_1 = LinearCongruentialFunction::from((action_1,m));
    let lcg_2 = LinearCongruentialFunction::from((action_2,m));
    let lcg_3 = LinearCongruentialFunction::from((action_3,m));

    assert_eq!(lcg_1,LinearCongruentialFunction { a: 1, b: -23, m: 10007});
    assert_eq!(lcg_2,LinearCongruentialFunction { a: 23, b: 0, m: 10007});
    assert_eq!(lcg_3,LinearCongruentialFunction { a: -1, b: -1, m: 10007});

}

#[test]
fn test_cut_n() {
    let action = Action { inst : Instruction::CutNCards, param: 3 };
    let lcg = LinearCongruentialFunction::from((action,10));
    let deck = lcg.apply(10);
    assert_eq!(deck,vec![3,4,5,6,7,8,9,0,1,2]);
    
    let action = Action { inst : Instruction::CutNCards, param: -4 };
    let lcg = LinearCongruentialFunction::from((action,10));
    let deck = lcg.apply(10);
    assert_eq!(deck,vec![6,7,8,9,0,1,2,3,4,5]);
}

#[test]
fn test_deal_into_new_stack() {
    let action = Action { inst : Instruction::DealIntoNewStack, param: 0 };
    let lcg = LinearCongruentialFunction::from((action,10));
    let deck = lcg.apply(10);
    assert_eq!(deck, vec![9,8,7,6,5,4,3,2,1,0]);
}

#[test]
fn test_deal_increment_n() {
    let action = Action { inst : Instruction::DealWithIncrementN, param: 3 };
    let lcg = LinearCongruentialFunction::from((action,10));
    let deck = lcg.apply(10);
    assert_eq!(deck, vec![0,7,4,1,8,5,2,9,6,3]);
}

#[test]
fn test_apply_actions_1() {
    let text = 
"deal with increment 7
deal into new stack
deal into new stack";
    let action_list = read_lines(text).unwrap().1;
    let composed_action = compose_actions(&action_list, 10);
    let deck = composed_action.apply(10);
    assert_eq!(deck,vec![0,3,6,9,2,5,8,1,4,7]);
}

#[test]
fn test_apply_actions_3() {
    let text = 
"deal with increment 7
deal with increment 9
cut -2";
    let action_list = read_lines(text).unwrap().1;
    let composed_action = compose_actions(&action_list, 10);
    let deck = composed_action.apply(10);
    assert_eq!(deck,vec![6,3,0,7,4,1,8,5,2,9]);
}
#[test]
fn test_apply_actions_4() {
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
    let action_list = read_lines(text).unwrap().1;
    let composed_action = compose_actions(&action_list, 10);
    let deck = composed_action.apply(10);
    assert_eq!(deck,vec![9,2,5,8,1,4,7,0,3,6]);
}