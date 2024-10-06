use std::fs;

fn main() {
    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");

    let parts = contents.lines();
    let mut answer_1 : i64 = 0; 
    let mut answer_2 : i64 = 0;

    for part in parts {
        answer_1 = answer_1 +      fuel_required(part.parse::<i64>().expect("Couldn't parse!"));
        answer_2 = answer_2 + iter_fuel_required(part.parse::<i64>().expect("Couldn't parse!"));
    }

    println!("{answer_1}");
    println!("{answer_2}");
}

fn fuel_required(mass : i64) -> i64 {
   return (mass / 3) - 2;
}

fn iter_fuel_required(mass : i64) -> i64 {
    let mut mass_to_go = mass;
    let mut answer : i64 = 0;

    while mass_to_go > 0 {
        let mut mass_fuel = fuel_required(mass_to_go); 
        if mass_fuel < 0 { mass_fuel = 0; }
        answer += mass_fuel;
        mass_to_go = mass_fuel;
    }
    return answer;
}

#[test]
fn test_fuel_required() {
    assert_eq!(fuel_required(12), 2);
    assert_eq!(fuel_required(14), 2);
    assert_eq!(fuel_required(1969), 654);
    assert_eq!(fuel_required(100756), 33583);
}

#[test]
fn test_iter_fuel_required() {
    assert_eq!(iter_fuel_required(12), 2);
    assert_eq!(iter_fuel_required(14), 2);
    assert_eq!(iter_fuel_required(1969), 966);
    assert_eq!(iter_fuel_required(100756), 50346);
}