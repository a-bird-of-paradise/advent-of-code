use std::ops::{Add,AddAssign,Mul};
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
pub struct PointOffset {
    pub x: i64,
    pub y: i64
}

impl Point          { pub const fn new() -> Self { Self { x: 0, y: 0 } } }
impl PointOffset    { pub const fn new() -> Self { Self { x: 0, y: 0 } } }

impl Add for PointOffset {
    type Output = PointOffset;
    fn add(self, other: PointOffset) -> PointOffset {
        PointOffset {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Add<Point> for PointOffset {
    type Output = Point;
    fn add(self, other: Point) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl Add<PointOffset> for Point {
    type Output = Point;
    fn add(self, other: PointOffset) -> Point {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl AddAssign for PointOffset {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl AddAssign<PointOffset> for Point {
    fn add_assign(&mut self, other: PointOffset) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl<T> Mul<T> for PointOffset where T:Into<i64> {
    type Output = PointOffset;
    fn mul(self, rhs: T) -> Self {
        let rhsi = rhs.into();
        Self { x: self.x * rhsi, y: self.y * rhsi }
    }
}

impl PointOffset { 
    pub fn is_cardinal(&self) -> bool {
        self.x == 0 || self.y == 0
    }
    pub fn is_intercardinal(&self) -> bool {
        abs(self.x) == abs(self.y)
    }
}
