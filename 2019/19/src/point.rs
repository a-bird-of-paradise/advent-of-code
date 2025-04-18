use std::cmp::Ordering;
use std::ops::{Add,AddAssign,Sub,SubAssign};

use num::{Num,Signed};

#[derive(Eq, Hash, PartialEq, Debug, Clone, Copy)] // debug for printer
pub struct Point<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd>  where f64: From<T> {
   pub x: T,
   pub y: T
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> Point<T> where f64: From<T> {
   pub fn new(x:T, y:T) -> Self {
       Self { x, y }
   }

   pub fn sumsq(&self) -> T { return self.x*self.x + self.y*self.y; }

   /// this has positive x to right, positive y down
   pub fn angle(&self) -> f64 {
       let temp =  f64::atan2(self.x.into(),self.y.into());
       let answer : f64 = if temp < 0.0 { temp + 2.0 * std::f64::consts::PI } else { temp };
       return answer;
   }

   pub fn is_neighbour(&self, other: &Point<T>) -> bool { self.manhattan(other) == T::one() }

   //pub fn to_ratio(&self) -> Ratio { return Ratio::new(-self.y, self.x); }

   pub fn manhattan(&self, other: &Point<T>) -> T { (self.x - other.x).abs() + (self.y - other.y).abs() }

}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> Add for Point<T> where f64: From<T> {
   type Output = Self;

   fn add(self, other: Self) -> Self {
       Self {x: self.x + other.x, y: self.y + other.y}
   }
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> Sub for Point<T> where f64: From<T> {
   type Output = Self;

   fn sub(self, other: Self) -> Self {
       Self {x: self.x - other.x, y: self.y - other.y}
   }
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> AddAssign for Point<T> where f64: From<T> {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> SubAssign for Point<T> where f64: From<T> {
    fn sub_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x - other.x,
            y: self.y - other.y,
        };
    }
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> Ord for Point<T> where f64: From<T> {
    fn cmp(&self, other: &Self) -> Ordering {
        if self.angle() < other.angle() { return Ordering::Less; }
        if self.angle() > other.angle() { return Ordering::Greater; }
        
        if self.sumsq() < other.sumsq() { return Ordering::Less; }
        if self.sumsq() > other.sumsq() { return Ordering::Greater; }
        return Ordering::Equal;
    }
}

impl<T: Num + Copy + Signed + Eq + std::cmp::PartialOrd> PartialOrd for Point<T> where f64: From<T> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if self.angle() < other.angle() { return Some(Ordering::Less); }
        if self.angle() > other.angle() { return Some(Ordering::Greater); }
        
        if self.sumsq() < other.sumsq() { return Some(Ordering::Less); }
        if self.sumsq() > other.sumsq() { return Some(Ordering::Greater); }
        return Some(Ordering::Equal);
    }
}

#[test]
pub fn test_point () {
    // point is dy==p, dx==q
    let point_1a = Point::new(0,1);
    let point_1b = Point::new(1,1);
    let point_2a = Point::new(1,0);
    let point_2b = Point::new(1,-1);
    let point_3a = Point::new(0,-1);
    let point_3b = Point::new(-1,-1);
    let point_4a = Point::new(-1,0);
    let point_4b = Point::new(-1,1);
    println!("{}", point_1a.angle() / std::f64::consts::PI);
    println!("{}", point_1b.angle() / std::f64::consts::PI);
    println!("{}", point_2a.angle() / std::f64::consts::PI);
    println!("{}", point_2b.angle() / std::f64::consts::PI);
    println!("{}", point_3a.angle() / std::f64::consts::PI);
    println!("{}", point_3b.angle() / std::f64::consts::PI);
    println!("{}", point_4a.angle() / std::f64::consts::PI);
    println!("{}", point_4b.angle() / std::f64::consts::PI);
    assert!(point_1a < point_1b 
        && point_1b < point_2a
        && point_2a < point_2b 
        && point_2b < point_3a
        && point_3a < point_3b 
        && point_3b < point_4a
        && point_4a < point_4b);

    let point_1a_stretch = Point::new(0,2);
    let point_1a_clone = Point::new(0,1);
    assert!(point_1a_stretch > point_1a);
    assert!(point_1a == point_1a_clone);

    assert!(point_1a.is_neighbour(&point_1b));
    assert!(!point_1a.is_neighbour(&point_3b));

    let mut point = Point { x: 1, y: 0 };
    point += Point { x: 2, y: 3 };
    assert_eq!(point, Point { x: 3, y: 3 });

    point = Point { x: 3, y: 3 };
    point -= Point { x: 2, y: 3 };
    assert_eq!(point, Point {x: 1, y: 0});

}