use std::collections::HashSet;
use std::collections::VecDeque;
use std::env;
use std::fs;
use std::thread;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(&file_path).expect("Should have been able to read the file");    

    let file : Vec<&str> = raw_file
        .trim()
        .split('\n')
        .map(|line| line.strip_suffix('\r').unwrap_or(line))
        .collect();

    
    // part 1

    println!("{}",solve(&file));

    // part 2

    let (nw,ne,sw,se) = splitter(&file);

    let nw2: Vec<&str> = nw.iter().map(AsRef::as_ref).collect();
    let ne2: Vec<&str> = ne.iter().map(AsRef::as_ref).collect();
    let sw2: Vec<&str> = sw.iter().map(AsRef::as_ref).collect();
    let se2: Vec<&str> = se.iter().map(AsRef::as_ref).collect();

    let mut answer : usize = 0;

    thread::scope(|scope| {
        let t1 = scope.spawn(|| solve(&nw2));
        let t2 = scope.spawn(|| solve(&ne2));
        let t3 = scope.spawn(|| solve(&sw2));
        let t4 = scope.spawn(|| solve(&se2));

        answer += t1.join().unwrap();
        answer += t2.join().unwrap();
        answer += t3.join().unwrap();
        answer += t4.join().unwrap();
    });

    println!("{answer}");
}

fn position_to_int(width:usize, row:usize, col:usize) -> usize { assert!(col < width); width * row + col }
fn int_to_col(width:usize, position:usize) -> usize {  position % width }
fn int_to_row(width:usize, position:usize) -> usize {  position / width }

fn splitter(file: &Vec<&str>) -> (Vec<String>,Vec<String>,Vec<String>,Vec<String>)
{
    let height = file.len();
    let width = file.first().unwrap().len();

    for row in file {
        if row.len() != width {
            panic!("Ragged file!");
        }
    } 
    
    let mut start : usize = usize::MAX;

    'outer: for row in 0..height {
        for col in 0..width {
            if file[row].as_bytes()[col] as char == '@' { 
                start = position_to_int(width, row, col);
                break 'outer;
            }
        }
    }

    assert!(start != usize::MAX);

    let at_row = int_to_row(width, start);
    let at_col = int_to_col(width, start);

    let mut nw : Vec<String> = Vec::new();
    let mut ne : Vec<String> = Vec::new();
    let mut sw : Vec<String> = Vec::new();
    let mut se : Vec<String> = Vec::new();

    for row in 0..=at_row {
        nw.push(file[row][0..=at_col].to_owned());
        ne.push(file[row][at_col..width].to_owned());

        if row == at_row - 1 {
            let mut new_row : String = String::new();
            for (i, mut c) in nw[row].chars().enumerate() {
                if i == at_col { c = '#'; }
                if i == at_col - 1 { c = '@'; }
                new_row.push(c);
            }
            nw[row] = new_row;

            new_row = String::new();
            for (i, mut c) in ne[row].chars().enumerate() {
                if i == 0 { c = '#'; }
                if i == 1 { c = '@'; }
                new_row.push(c);
            }
            ne[row] = new_row;
        }

        if row == at_row {
            let mut new_row : String = String::new();
            for (_i, _c) in nw[row].chars().enumerate() {
                new_row.push('#');
            }
            nw[row] = new_row;

            new_row = String::new();
            for (_i, _c) in ne[row].chars().enumerate() {
                new_row.push('#');
            }
            ne[row] = new_row;
        }
    }
    for row in at_row..height {
        sw.push(file[row][0..=at_col].to_owned());
        se.push(file[row][at_col..width].to_owned());

        if row == at_row {
            let mut new_row = String::new();
            for(_i, _c) in sw[0].chars().enumerate() {
                new_row.push('#');
            }
            sw[0] = new_row;

            new_row = String::new();
            for(_i, _c) in se[0].chars().enumerate() {
                new_row.push('#');
            }
            se[0] = new_row;
        }

        if row == at_row + 1 {
            let mut new_row = String::new();
            for(i,mut c) in sw[1].chars().enumerate() {
                if i == at_col { c = '#'; }
                if i == at_col - 1 { c = '@'; }
                new_row.push(c);
            }
            sw[1] = new_row;

            new_row = String::new();
            for(i , mut c) in se[1].chars().enumerate() {
                if i == 0 { c = '#'; }
                if i == 1 { c = '@'; }
                new_row.push(c);
            }
            se[1] = new_row;
        }
    }

    (nw,ne,sw,se)

}

fn solve(file: &Vec<&str>) -> usize {

    let height = file.len();
    let width = file.first().unwrap().len();

    for row in file {
        if row.len() != width {
            panic!("Ragged file!");
        }
    }

    let mut passage_mask : Vec<usize> = Vec::new();
    let mut door_mask : Vec<usize> = Vec::new();
    let mut key_mask : Vec<usize> = Vec::new();

    passage_mask.resize(width * height, 0);
    door_mask.resize(width * height, 0);
    key_mask.resize(width * height, 0);

    let mut start : usize = usize::MAX;

    for row in 0..height {
        for col in 0..width {
            let c : char = file[row].as_bytes()[col] as char;

            assert!(c == '.' || c == '@' || c == '#' || c.is_ascii_alphabetic());
            
            if c != '#' { passage_mask[position_to_int(width, row, col)] = 1 }
            if c == '@' { start = position_to_int(width, row, col) }
            if c.is_ascii_lowercase() {  key_mask[position_to_int(width, row, col)] |= 1 << (c as u32 - 'a' as u32) }
            if c.is_ascii_uppercase() { door_mask[position_to_int(width, row, col)] |= 1 << (c as u32 - 'A' as u32) }

        }
    }

    assert!(start != usize::MAX);

    let mut all_keys : usize = 0;
    let mut last_key : usize = 0;

    for val in &key_mask { 
        all_keys |= *val; 
        if *val > last_key { 
            last_key = *val 
        } 
    }

    // restrict to possible keys

    for item in door_mask.iter_mut() { *item &= all_keys }

    // do a breadth first search to find the shortest path to goal

    let mut Q : VecDeque<(usize,usize,usize)> = VecDeque::new();
    let mut visited : HashSet<(usize,usize)> = HashSet::new(); // BTreeSet c. 3x slower

    let root : (usize,usize,usize) = (start,0,0);

    Q.push_back(root);
    visited.insert((root.0,root.1));
    let mut answer = root;

    while Q.len() != 0 {

        let v = Q.pop_front().unwrap();

        if v.1 == all_keys {
            answer = v;
            break;
        }

        let row: usize = int_to_row(width, v.0);
        let col: usize = int_to_col(width, v.0);
        let keyring: usize = v.1;

        // up

        if row > 0 {
            let new_row = row - 1;
            let new_col = col;
            let new_keyring = keyring | key_mask[position_to_int(width, new_row, new_col)];

            if passage_mask[position_to_int(width, new_row, new_col)] == 1 {
                if door_mask[position_to_int(width, new_row, new_col)] & new_keyring == door_mask[position_to_int(width, new_row, new_col)] {
                    if !visited.contains(&(position_to_int(width, new_row, new_col),new_keyring)) {
                        visited.insert((position_to_int(width, new_row, new_col),new_keyring));
                        Q.push_back((position_to_int(width, new_row, new_col),new_keyring,v.2 + 1));
                    }
                }
            }
        }

        // down

        if row < height-1 {
            let new_row = row + 1;
            let new_col = col;
            let new_keyring = keyring | key_mask[position_to_int(width, new_row, new_col)];

            if passage_mask[position_to_int(width, new_row, new_col)] == 1 {
                if door_mask[position_to_int(width, new_row, new_col)] & new_keyring == door_mask[position_to_int(width, new_row, new_col)] {
                    if !visited.contains(&(position_to_int(width, new_row, new_col),new_keyring)) {
                        visited.insert((position_to_int(width, new_row, new_col),new_keyring));
                        Q.push_back((position_to_int(width, new_row, new_col),new_keyring,v.2 + 1));
                    }
                }
            }
        }

        // left 

        if col > 0 {
            let new_row = row;
            let new_col = col - 1;
            let new_keyring = keyring | key_mask[position_to_int(width, new_row, new_col)];

            if passage_mask[position_to_int(width, new_row, new_col)] == 1 {
                if door_mask[position_to_int(width, new_row, new_col)] & new_keyring == door_mask[position_to_int(width, new_row, new_col)] {
                    if !visited.contains(&(position_to_int(width, new_row, new_col),new_keyring)) {
                        visited.insert((position_to_int(width, new_row, new_col),new_keyring));
                        Q.push_back((position_to_int(width, new_row, new_col),new_keyring,v.2 + 1));
                    }
                }
            }
        }

        // right

        if col < width - 1 {
            let new_row = row;
            let new_col = col + 1;
            let new_keyring = keyring | key_mask[position_to_int(width, new_row, new_col)];

            if passage_mask[position_to_int(width, new_row, new_col)] == 1 {
                if door_mask[position_to_int(width, new_row, new_col)] & new_keyring == door_mask[position_to_int(width, new_row, new_col)] {
                    if !visited.contains(&(position_to_int(width, new_row, new_col),new_keyring)) {
                        visited.insert((position_to_int(width, new_row, new_col),new_keyring));
                        Q.push_back((position_to_int(width, new_row, new_col),new_keyring,v.2 + 1));
                    }
                }
            }
        }
    }
    answer.2
}