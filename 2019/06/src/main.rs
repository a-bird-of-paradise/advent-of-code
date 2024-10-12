use std::fs;
use std::env;
use std::collections::HashMap;
use std::collections::BTreeSet;

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file: String = fs::read_to_string(&file_path).expect("Should have been able to read the file");

    let lines : Vec<&str> = raw_file.trim().split('\n').collect();

    let mut tree : HashMap<&str, Vec<&str>> = HashMap::new();                   // parent -> children
    let mut parent_tree : HashMap<&str,&str> = HashMap::new();                  // child -> parent
    let mut depths : HashMap<&str, u64> = HashMap::new();

    for line in lines {
        let mut iter = line.trim().splitn(2, ')');
        let first: &str = iter.next().unwrap();
        let second: &str = iter.next().unwrap();
        tree.entry(&first).or_insert_with(Vec::new).push(&second);
        parent_tree.insert(&second, &first); 
    }

    depths.insert("COM", 0);
    recurse(&tree, &mut depths, "COM");

    let mut answer_1 : u64 = 0;

    for item in &depths {
        answer_1 += item.1;
    }

    println!("{answer_1}");

    let mut path_you : BTreeSet<&str> = BTreeSet::new();
    let mut path_san : BTreeSet<&str> = BTreeSet::new(); 

    let mut current : &str = "YOU";

    while current != "COM" {
        current = parent_tree[&current]; 
        path_you.insert(&current);
    }

    current = "SAN";

    while current != "COM" {
        current = parent_tree[&current]; 
        path_san.insert(&current);
    }

    let common_points : BTreeSet<&str> = path_san.intersection(&path_you).cloned().collect();

    let mut highest_common_depth : u64 = 0; 

    for item in common_points {
        if depths[&item] > highest_common_depth {
            highest_common_depth = depths[&item];
        }
    }

    let answer_2: u64  = depths[parent_tree["SAN"]] + depths[parent_tree["YOU"]] - 2 * highest_common_depth;

    println!("{answer_2}");

}

fn recurse<'a> (
    tree : &HashMap<&str, Vec<&'a str>>,
    depths : &mut HashMap<&'a str,u64>,
    current : &str 
)  {

    if !tree.contains_key(&current) { return; }
    let depth: u64 = depths[&current];
    
    for child in &tree[&current] {
        depths.insert(&child, depth + 1);
        recurse(tree, depths, child);
    }
}