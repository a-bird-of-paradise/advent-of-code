use std::ops::{Add,AddAssign,Mul,Sub,SubAssign,MulAssign};
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

impl Direction { 
    pub fn is_cardinal(&self) -> bool {
        self.x == 0 || self.y == 0
    }
    pub fn is_intercardinal(&self) -> bool {
        abs(self.x) == abs(self.y)
    }
}
