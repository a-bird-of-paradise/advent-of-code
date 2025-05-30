pub trait AOC {
    fn part_one(&self, input: &str) -> String;
    fn part_two(&self, input: &str) -> String;
}

pub fn skip_nth<T>(vec: &[T], n: usize) -> Vec<T> where T: Clone {
    vec
        .iter()
        .enumerate()
        .filter_map(|(i, el)| (i != n).then_some(el.clone()))
        .collect()
}