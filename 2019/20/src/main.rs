use std::{collections::{HashMap, HashSet, VecDeque}, env, fs, thread};

fn main() {
    let file_path : String = env::args().nth(1).expect("No input filename provided.");
    let raw_file = fs::read_to_string(&file_path).expect("Should have been able to read the file");
    let file_rows : Vec<&str> = raw_file
        .split('\n')
        .map(|line| line.strip_suffix('\r').unwrap_or(line))
        .collect();

    let file = shred_file(&file_rows);

    let (graph,letters) = make_graph_and_list(&file);

    thread::scope(|scope| {
        let t1 = scope.spawn(|| bfs_part_1(&graph, &letters));
        let t2 = scope.spawn(|| bfs_part_2(&graph, &letters));

        println!("{}",t1.join().unwrap());
        println!("{}",t2.join().unwrap());
    });

}

fn bfs_part_1(
    graph : &HashMap<(usize,usize), HashSet<Link>>,
    letters : &HashMap<String,HashSet<(usize,usize)>>) -> usize 
{
    let start = letters.get("AA").unwrap().iter().nth(0).unwrap();
    let goal = letters.get("ZZ").unwrap().iter().nth(0).unwrap();

    // BFS from start to end

    let mut Q : VecDeque<(usize,usize,usize)> = VecDeque::new();
    let mut visited : HashSet<(usize,usize)> = HashSet::new(); 

    let mut answer: usize = usize::MAX;

    Q.push_back((start.0,start.1,0));
    visited.insert(*start);

    while Q.len() != 0 {
        let v = Q.pop_front().unwrap();
        if v.0 == goal.0 && v.1 == goal.1 {
            answer = v.2;
            break;
        }

        for neighbour in graph.get(&(v.0,v.1)).unwrap() {
            if !visited.contains(&neighbour.pt) {
                visited.insert(neighbour.pt);
                Q.push_back((neighbour.pt.0,neighbour.pt.1,v.2+1));
            }
        }
    }
    answer 
}

fn bfs_part_2(
    graph : &HashMap<(usize,usize), HashSet<Link>>,
    letters : &HashMap<String,HashSet<(usize,usize)>>) -> usize 
{
    let start = letters.get("AA").unwrap().iter().nth(0).unwrap();
    let goal = letters.get("ZZ").unwrap().iter().nth(0).unwrap();

    let start_location = Location::new(start.0,start.1,0);
    let goal_location = Location::new(goal.0, goal.1, 0);

    // BFS from start to end

    let mut Q : VecDeque<State> = VecDeque::new();
    let mut visited : HashSet<Location> = HashSet::new(); 

    let mut answer: usize = usize::MAX;

    Q.push_back(State::new(start_location.row, start_location.col, start_location.depth, 0));
    visited.insert(start_location);

    while Q.len() != 0 {
        let v = Q.pop_front().unwrap();

        if v.l.row == goal_location.row && v.l.col == goal_location.col && v.l.depth == goal_location.depth {
            answer = v.steps;
            break;
        }

        let mut next_states = Vec::<State>::new();

        let neighbours = graph.get(&(v.l.row,v.l.col)).unwrap();

        for neighbour in neighbours {

            if neighbour.lt == LinkType::TopLevelOnly && v.l.depth != 0 {
                continue;
            } 
            else if neighbour.lt == LinkType::TopLevelOnly && v.l.depth == 0 {
                let candidate_location = Location::new(neighbour.pt.0, neighbour.pt.1, v.l.depth);
                if ! visited.contains(&candidate_location) {
                    visited.insert(candidate_location);
                    Q.push_back(State { l: candidate_location, steps: (v.steps + 1) });
                }
            }
            else if neighbour.lt == LinkType::Normal {
                let candidate_location = Location::new(neighbour.pt.0, neighbour.pt.1, v.l.depth);
                if ! visited.contains(&candidate_location) {
                    visited.insert(candidate_location);
                    Q.push_back(State { l: candidate_location, steps: (v.steps + 1) });
                }
            } else if neighbour.lt == LinkType::ToOuterEdge {
                let candidate_location = Location::new(neighbour.pt.0, neighbour.pt.1, v.l.depth + 1);
                if ! visited.contains(&candidate_location) {
                    visited.insert(candidate_location);
                    Q.push_back(State { l: candidate_location, steps: (v.steps + 1) });
                }
            } 
            else if neighbour.lt == LinkType::ToInnerEdge && v.l.depth == 0 {
                continue;
            } 
            else if neighbour.lt == LinkType::ToInnerEdge && v.l.depth > 0 {
                let candidate_location = Location::new(neighbour.pt.0, neighbour.pt.1, v.l.depth - 1);
                if ! visited.contains(&candidate_location) {
                    visited.insert(candidate_location);
                    Q.push_back(State { l: candidate_location, steps: (v.steps + 1) });
                }
            }
        }
    }
    answer 
}

fn make_graph_and_list(file: &Vec<Vec<char>>) -> (
    HashMap<(usize,usize), HashSet<Link>>, // adjacency graph 
    HashMap<String,HashSet<(usize,usize)>> // letter map 
) {
    let mut graph : HashMap<(usize,usize), HashSet<Link>> = HashMap::new();
    let mut letters : HashMap<String,HashSet<(usize,usize)>> = HashMap::new();

    for row in 0..file.len() {
        for col in 0..file[row].len() {
            if file[row][col] == '.' {
                
                if row != 0 {
                    if file[row-1][col] == '.' {
                        graph.entry((row,col)).or_insert_with(HashSet::new).insert(Link::new((row-1,col), LinkType::Normal));
                        graph.entry((row-1,col)).or_insert_with(HashSet::new).insert(Link::new((row,col), LinkType::Normal));
                    }
                }

                if row != file.len() - 1 {
                    if file[row+1][col] == '.' {
                        graph.entry((row,col)).or_insert_with(HashSet::new).insert(Link::new((row+1,col), LinkType::Normal));
                        graph.entry((row+1,col)).or_insert_with(HashSet::new).insert(Link::new((row,col), LinkType::Normal));
                    }
                }
                
                if col != 0 {
                    if file[row][col-1] == '.' {
                        graph.entry((row,col)).or_insert_with(HashSet::new).insert(Link::new((row,col-1), LinkType::Normal));
                        graph.entry((row,col-1)).or_insert_with(HashSet::new).insert(Link::new((row,col), LinkType::Normal));
                    }
                }

                if col != file[row].len() - 1 {
                    if file[row][col+1] == '.' { 
                        graph.entry((row,col)).or_insert_with(HashSet::new).insert(Link::new((row,col+1), LinkType::Normal));
                        graph.entry((row,col+1)).or_insert_with(HashSet::new).insert(Link::new((row,col), LinkType::Normal));
                    }
                }
            }

            if file[row][col].is_ascii_uppercase() {
                if row != file.len() - 1 {
                    if file[row+1][col].is_ascii_uppercase() {
                        let mut key : String = String::new();
                        let row_offset : i64; 

                        if row == 0 { 
                            row_offset = 2 
                        }
                        else if row == file.len() - 2 { 
                            row_offset = -1 
                        }
                        else if file[row+2][col] == '.' { 
                            row_offset = 2 
                        }
                        else { 
                            row_offset = -1 
                        }

                        key.push(file[row][col]);
                        key.push(file[row+1][col]);
                        letters.entry(key).or_insert_with(HashSet::new).insert(((row as i64 + row_offset) as usize,col));
                    }
                }
                
                if col != file[row].len() - 1 {
                    if file[row][col+1].is_ascii_uppercase() {
                        let mut key : String = String::new();
                        let col_offset : i64; 

                        if col == 0 { 
                            col_offset = 2 
                        }
                        else if col == file[row].len() - 2 { 
                            col_offset = -1 
                        }
                        else if file[row][col+2] == '.' { 
                            col_offset = 2 
                        }
                        else { 
                            col_offset = -1 
                        }
                        key.push(file[row][col]);
                        key.push(file[row][col+1]);
                        letters.entry(key).or_insert_with(HashSet::new).insert((row,(col as i64 + col_offset) as usize));
                    }
                }
            }
        }
    }

    for item in &letters {
        if item.1.len() == 2 {

            let mut the_iter = item.1.iter();
            let first = the_iter.next().unwrap();
            let second = the_iter.next().unwrap();

            let first_is_outer : bool; 
            let second_is_outer : bool; 

            first_is_outer = first.0 == 2
                || first.0 == file.len() - 3
                || first.1 == 2
                || first.1 == file[first.0].len() - 3;

            second_is_outer = second.0 == 2
                || second.0 == file.len() - 3
                || second.1 == 2
                || second.1 == file[second.0].len() - 3; 

            assert!(first_is_outer ^ second_is_outer);

            if first_is_outer {

                graph.entry(*first).or_insert_with(HashSet::new).insert(Link::new(*second, LinkType::ToInnerEdge));
                graph.entry(*second).or_insert_with(HashSet::new).insert(Link::new(*first,LinkType::ToOuterEdge));

            } else {

                graph.entry(*first).or_insert_with(HashSet::new).insert(Link::new(*second, LinkType::ToOuterEdge));
                graph.entry(*second).or_insert_with(HashSet::new).insert(Link::new(*first,LinkType::ToInnerEdge));

            }
        }
    }

    // anything that goes to the first or last cell is marked 

    let AA_set = letters.get("AA").unwrap();
    let ZZ_set = letters.get("ZZ").unwrap();

    assert!(AA_set.len() == ZZ_set.len());
    assert!(AA_set.len() == 1);

    let AA_index = AA_set.iter().nth(0).unwrap();
    let ZZ_index = ZZ_set.iter().nth(0).unwrap();

    let AA_target= graph[AA_index].iter().nth(0).unwrap().clone();
    let ZZ_target = graph[ZZ_index].iter().nth(0).unwrap().clone();

    graph.entry(*AA_index).and_modify(|e| { e.clear(); e.insert(Link::new(AA_target.pt,LinkType::TopLevelOnly)); });
    graph.entry(*ZZ_index).and_modify(|e| { e.clear(); e.insert(Link::new(ZZ_target.pt,LinkType::TopLevelOnly)); });

    
    let mut answer_graph : HashMap<(usize,usize), HashSet<Link>> = HashMap::new();

    for item in graph.iter() {
        for link in item.1.iter() {
            let mut new_link = *link;
            if new_link.pt == *AA_index || new_link.pt == *ZZ_index {
                new_link.lt = LinkType::TopLevelOnly;
            }
            answer_graph.entry(*item.0).or_insert_with(HashSet::default).insert(new_link);
        }
    }

    (answer_graph,letters)
}

fn shred_file(file: &Vec<&str>) -> Vec<Vec<char>>
{
    let mut answer: Vec<Vec<char>> = Vec::new();
    for row in file {
        let mut new_row : Vec<char> = Vec::new();
        for c in row.chars() {
            new_row.push(c);
        }
        answer.push(new_row);
    }
    answer
}

#[derive(Debug,PartialEq, Eq, PartialOrd, Ord, Hash, Default, Copy, Clone)]
enum LinkType
{
    #[default] Normal,
    ToOuterEdge,
    ToInnerEdge,
    TopLevelOnly
}

#[derive(Debug,PartialEq, Eq, PartialOrd, Ord, Hash, Default, Copy, Clone)]
struct Link
{
    pt: (usize,usize),
    lt: LinkType
}

impl Link {
    fn new(pt : (usize,usize), lt : LinkType) -> Self {
        Self { pt , lt }
    }
}

#[derive(Debug,PartialEq, Eq, PartialOrd, Ord, Hash, Default, Copy, Clone)]
struct State {
    l: Location,
    steps: usize
}

impl State {
    fn new(row: usize, col: usize, depth: usize, steps: usize) -> Self {
        Self { l : Location::new(row, col, depth) , steps }
    } 
}


#[derive(Debug,PartialEq, Eq, PartialOrd, Ord, Hash, Default, Copy, Clone)]
struct Location {
    row: usize,
    col: usize, 
    depth: usize
}

impl Location {
    fn new( row: usize, col: usize, depth: usize) -> Self {
        Self {row, col, depth }
    }
}