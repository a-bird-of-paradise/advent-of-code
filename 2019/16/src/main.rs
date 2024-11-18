use std::env;
use std::fs;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");    
    let numbers : Vec<i32> = raw_file
        .trim()
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .map(|u| u as i32)
        .collect();
    
    let answer1 = multiphase(&numbers,100);

    let printme = answer1.into_iter().map(|i| i.to_string()).collect::<String>();

    println!("{}",&printme[..8]);

    let long_numbers = numbers.repeat(10000);

    let long_string : String = long_numbers
        .clone()
        .into_iter()
        .map(|c| c.to_string())
        .collect();

    let offset = long_string[..7].to_string().parse::<usize>().unwrap();

    let tail : Vec<i32> = long_numbers[offset..].iter().rev().cloned().collect();

    let mut cumsum : Vec<i32> = tail;

    for _i in 1..=100 {
        cumsum = cumsum
            .into_iter()
            .scan(0, |acc , x| {
                *acc += x;
                Some(*acc)
            })
            .map(|x| x % 10)
            .collect();
    }

    let len = cumsum.len();

    let answer : Vec<i32> = cumsum[len-8..].iter().rev().cloned().collect();

    let answer_text = answer.iter().fold(0,|acc,x| acc * 10 + x);

    println!("{answer_text}");

}

fn multiphase(input_signal : &Vec<i32>, n_phase: u32) -> Vec<i32> {
    let mut i : u32 = 0;
    let mut output_signal : Vec<i32> = input_signal.clone();
    while i < n_phase {
        output_signal = phase(&output_signal);
        i += 1; 
    }
    output_signal
}

fn phase(input_signal : &Vec<i32>) -> Vec<i32> {
    let mut output_signal : Vec<i32> = Vec::new();
    let mut accum : i32 = 0;

    let nu = input_signal.len();
    let n = nu as i32;

    for i in 1..=n {
        let pattern: Vec<i32> = expand(&vec![0,1,0,-1],i,n);
        for j in 0usize..nu {
            accum += pattern[j] * input_signal[j];
        }

        if accum < 0 { accum *= -1; }
        accum %= 10;
        output_signal.push(accum);
        accum = 0;
    }

    output_signal
}

fn expand(pattern : &Vec<i32>, n : i32, len : i32) -> Vec<i32> {
    let mut answer : Vec<i32> = Vec::new();
    let base : i32 = pattern.len().try_into().expect("Out of range");
    let mut i : i32 = 0;
    let mut j : i32 = 0;
    let mut k : i32 = 0;

    while i <= len {

        answer.push(pattern[j as usize]);

        k += 1;

        if k == n {
            k = 0;
            j += 1;
        }

        if j == base {j = 0;}
        i += 1;
    }

    answer.remove(0);

    answer

}

#[test]
fn test_expand(){
    let pattern = vec![0,1,0,-1];
    assert_eq!(expand(&pattern,2,15), vec![0, 1, 1, 0, 0, -1, -1, 0, 0, 1, 1, 0, 0, -1, -1]);
}

#[test]
fn test_phase() {
    let input_1 = vec![1,2,3,4,5,6,7,8];
    let output_1 = phase(&input_1);
    let output_2 = phase(&output_1);
    let output_3 = phase(&output_2);
    let output_4 = phase(&output_3);

    assert_eq!(output_1,vec![4, 8, 2, 2, 6, 1, 5, 8]);
    assert_eq!(output_2,vec![3, 4, 0, 4, 0, 4, 3, 8]);
    assert_eq!(output_3,vec![0, 3, 4, 1, 5, 5, 1, 8]);
    assert_eq!(output_4,vec![0, 1, 0, 2, 9, 4, 9, 8]);
}

#[test]
fn test_multiphase() {
    let input_1 = vec![1,2,3,4,5,6,7,8];
    let output_1 = multiphase(&input_1,4);
    assert_eq!(output_1,vec![0, 1, 0, 2, 9, 4, 9, 8]);

    let input_2 = "80871224585914546619083218645595"
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .map(|u| u as i32)
        .collect();

    let output_2 = multiphase(&input_2,100);
    let output_2_flat = output_2.into_iter().map(|i| i.to_string()).collect::<String>();
    assert_eq!(&output_2_flat[..8],"24176176");

    let input_3 = "19617804207202209144916044189917"
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .map(|u| u as i32)
        .collect();

    let output_3 = multiphase(&input_3,100);
    let output_3_flat = output_3.into_iter().map(|i| i.to_string()).collect::<String>();
    assert_eq!(&output_3_flat[..8],"73745418");

    let input_4 = "69317163492948606335995924319873"
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .map(|u| u as i32)
        .collect();

    let output_4 = multiphase(&input_4,100);
    let output_4_flat = output_4.into_iter().map(|i| i.to_string()).collect::<String>();
    assert_eq!(&output_4_flat[..8],"52432133");
    
}