use std::fs;
use std::env;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(file_path.clone()).expect("Should have been able to read the file");
    let numbers : Vec<u32> = raw_file
        .trim()
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .collect();

    let height : u32 = 6;
    let width : u32 = 25;

    let numbers_len_u32 : u32 = numbers.len().try_into().unwrap();
    let num_layers: u32 = numbers_len_u32 / (6 * 25);

    let mut min_zeroes_seen : u32 = u32::MAX;
    let mut min_zero_index : u32 = num_layers;

    for i in 0..num_layers {
        let this_zero_seen = num_zeroes(&numbers, height, width, i);

        if this_zero_seen < min_zeroes_seen {
            min_zero_index = i;
            min_zeroes_seen = this_zero_seen;
        }
    }

    let mut count_1 : u32 = 0;
    let mut count_2 : u32 = 0; 

    for i in 0..width {
        for j in 0..height {
            let x = numbers[to_index(i, j, min_zero_index, height, width)];
            if x == 1 { count_1 += 1; }
            if x == 2 { count_2 += 1; }
        }
    }

    let answer_1: u32 = count_1 * count_2;

    println!("{}",answer_1);

    let mut rendered : Vec<u32> = Vec::new();
    let hw_usize : usize = (height * width).try_into().unwrap();
    rendered.resize(hw_usize, 2);

    for z in 0..num_layers {
        for x in 0..width {
            for y in 0..height {
                if rendered[to_index(x, y, 0, height, width)] == 2 {
                    rendered[to_index(x, y, 0, height, width)] = numbers[to_index(x, y, z, height, width)];
                }
            }
        }
    }

    print_layers(&rendered, height, width);

}

fn num_zeroes(numbers : &Vec<u32>, height : u32, width : u32, layer : u32) -> u32 {
    let mut answer : u32 = 0;

    for i in 0..width {
        for j in 0..height {
            if numbers[to_index(i, j, layer, height, width)] == 0 {
                answer += 1;
            }
        }
    }

    return answer;
}

fn to_index(x : u32, y : u32, z : u32, height : u32, width : u32) -> usize {
    return (x + width * ( y + height * z)).try_into().unwrap(); 
}

fn print_layers(numbers : &Vec<u32>, height : u32, width : u32) {
   let len_32 : u32 = numbers.len().try_into().unwrap();
   assert_eq!(len_32 % (height * width), 0);

    let nlayers : u32 = len_32 / (height * width);

    for i in 0..nlayers {
        println!("Layer {i}:");
        for j in 0..height {
            for k  in 0..width {
                let to_print = numbers[to_index(k,j,i,height,width)];
                if to_print == 0 { print!(" "); } else { print!("{to_print}"); }
            }
            print!("\n");
        }
        print!("\n");
    }
}

#[test]
fn test_print_layers() {
    let numbers : Vec<u32> = "123456789012"
        .chars()
        .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
        .collect();
    let height : u32 = 2;
    let width : u32 = 3; 
    print_layers(&numbers, height, width);
}

#[test]
fn test_num_zeroes () {
    let numbers : Vec<u32> = "123456789012"
    .chars()
    .map(|c| c.to_digit(10).expect("Couldn't parse integer"))
    .collect();
    let height : u32 = 2;
    let width : u32 = 3; 

    assert_eq!(num_zeroes(&numbers, height, width, 0),0);
    assert_eq!(num_zeroes(&numbers, height, width, 1),1);
}