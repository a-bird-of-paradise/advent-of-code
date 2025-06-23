use std::{fmt::Debug, ops::{Add, AddAssign, Deref, DerefMut, Mul, MulAssign, Neg, Sub, SubAssign}};
use num::abs;

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


#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Default, Hash)]
pub struct Point {
    pub x: i64,
    pub y: i64
}
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Default, Hash)]
pub struct Direction {
    pub x: i64,
    pub y: i64
}

impl Point          { pub const fn new() -> Self { Self { x: 0, y: 0 } } }
impl Direction      { pub const fn new() -> Self { Self { x: 0, y: 0 } } }

impl Add for Direction {
    type Output = Direction;
    fn add(self, other: Direction) -> Direction {
        Direction {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Sub for Direction {
    type Output = Direction;
    fn sub(self, other: Direction) -> Direction {
        Direction {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl Add<Point> for Direction {
    type Output = Point;
    fn add(self, other: Point) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Sub<Point> for Direction {
    type Output = Point;
    fn sub(self, other: Point) -> Point {
        Point {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl Add<Direction> for Point {
    type Output = Point;
    fn add(self, other: Direction) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Sub<Direction> for Point {
    type Output = Point;
    fn sub(self, other: Direction) -> Point {
        Point {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl AddAssign for Direction {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl AddAssign<Direction> for Point {
    fn add_assign(&mut self, other: Direction) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl SubAssign for Direction {
    fn sub_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x - other.x,
            y: self.y - other.y,
        };
    }
}

impl SubAssign<Direction> for Point {
    fn sub_assign(&mut self, other: Direction) {
        *self = Self {
            x: self.x - other.x,
            y: self.y - other.y,
        };
    }
}

impl<T> Mul<T> for Direction where T:Into<i64> {
    type Output = Direction;
    fn mul(self, rhs: T) -> Self {
        let rhsi = rhs.into();
        Self { x: self.x * rhsi, y: self.y * rhsi }
    }
}

impl<T> MulAssign<T> for Direction where T:Into<i64> {
    fn mul_assign(&mut self, rhs: T) {
        let rhsi = rhs.into();
        self.x *= rhsi;
        self.y *= rhsi;
    }
}

impl Sub for Point { // x + y meaningless but x - y is a distance vector 
    type Output = Direction;
    fn sub(self, rhs: Point) -> Direction {
        Direction { x: self.x - rhs.x, y: self.y - rhs.y }
    }
}

impl Neg for Direction {
    type Output = Direction;

    fn neg(self) -> Self::Output {
        Self { x: -self.x, y: -self.y }
    }
}

impl Direction { 
    pub fn is_cardinal(&self) -> bool {
        self.x == 0 || self.y == 0
    }

    pub fn is_intercardinal(&self) -> bool {
        abs(self.x) == abs(self.y)
    }

    pub fn rot_right(&self) -> Self {
        Self { x: -self.y, y: self.x }
    }
}

impl Point {
    pub fn cardinal_neighbours(&self) -> Vec<Self> {
        vec![
            *self + Direction{ x:  0, y:  1 },
            *self + Direction{ x:  1, y:  0 },
            *self + Direction{ x:  0, y: -1 },
            *self + Direction{ x: -1, y:  0 }
        ]
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum TreeNode<T> {
    Leaf(T),
    Children(Vec<TreeNode<T>>)
}

impl<T> TreeNode<T> where T: Clone{
    pub fn new_leaf(from: &T) -> Self {
        Self::Leaf(from.clone())
    }

    pub fn new_leaves(from: &Vec<T>) -> Self {
        Self::Children(
            from
                .iter()
                .map(|t| Self::new_leaf(&t))
                .collect()
        )
    }
}

impl<T> TreeNode<T> {
    pub fn num_leaves(&self) -> u64 {
        match self {
            TreeNode::Leaf(_) => { 1 },
            TreeNode::Children(v) => { v.iter().map(|c| c.num_leaves()).sum() }
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Tree<T> {
    pub root: Option<TreeNode<T>>
}

impl<T> Tree<T> where T:Clone {
    pub fn new(from: &Vec<T>) -> Self {
        Self { root: Some(TreeNode::new_leaves(from)) }
    }
}

impl<T> Tree<T> {
    pub fn num_leaves(&self) -> u64 {
        match &self.root {
            None => { 0 },
            Some(t) => { t.num_leaves() }
        }
    }
}

impl TreeNode<u64> {
    pub fn walk(&mut self) {
        // if I have children, walk them
        match self {
            TreeNode::Children(v) => { 
                for c in v { c.walk(); }
            },
            TreeNode::Leaf(v) => {
                if *v == 0 {
                    *v = 1;
                } else if (v.checked_ilog10().unwrap_or(0)+1) % 2 == 0 {
                    let n_digits = v.checked_ilog10().unwrap_or(0)+1;
                    let divisor = 10_u64.pow(n_digits / 2);
                    let split = vec![*v / divisor, *v % divisor];
                    *self = TreeNode::new_leaves(&split);
                } else {
                    *v *= 2024;
                }
            }
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Grid<T> {
    pub cells: Vec<T>,
    pub width: usize,
    pub height: usize
}

impl<T> Grid<T> {
    pub fn get(&self, x: usize, y: usize) -> &T {
        &self.cells[x + y * self.width]
    }

    pub fn set(&mut self, x: usize, y: usize) -> &mut T {
        &mut self.cells[x + y * self.width]
    }

    pub fn iter(&self) -> impl Iterator<Item = &T> {
        self.cells.iter()
    }

    pub fn index_to_point(&self, index: usize) -> Point {
        Point { x: index.rem_euclid(self.width as usize) as i64, y: index.div_euclid(self.width as usize) as i64 }
    }

    pub fn point_to_index(&self, point: &Point) -> usize {
        (point.y as usize * self.width) + point.x as usize
    }
}

impl<T> IntoIterator for Grid<T> {
    type Item = T;
    type IntoIter = std::vec::IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        self.cells.into_iter()
    }
}

impl<T> Deref for Grid<T> {
    type Target = [T]; 
    fn deref(&self) -> &[T] {
        &self.cells[..]
    }
}

impl<T> DerefMut for Grid<T> {
    fn deref_mut(&mut self) -> &mut [T] {
        &mut self.cells[..]
    }
}

impl<T> From<Vec<Vec<T>>> for Grid<T> where T:Clone {
    fn from(item: Vec<Vec<T>>) -> Self {
        let height = item.len();
        let width = item[0].len();
        let inner = item.iter().flatten().cloned().collect();
        Self { cells: inner, width: width, height: height }
    }
}

impl<T> std::fmt::Display for Grid<T> where T: std::fmt::Display {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        for y in 0..self.height {
            for x in 0..self.width {
                write!(f,"{}",self.get(x, y))?;
            }
            write!(f,"\n")?;
        }
        Ok(())
    }
}