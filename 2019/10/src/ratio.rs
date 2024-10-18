use gcd::Gcd;
use std::cmp::Ordering;

#[derive(Eq, Hash, PartialEq, Debug, Clone, Copy)] // debug for printer
pub struct Ratio  {
    pub p: i32,
    pub q: i32
}

#[derive(PartialEq,Debug,PartialOrd)]
pub enum Quadrant {
    First = 1,
    Second = 2,
    Third = 3,
    Fourth = 4
}

impl Ratio {
    pub fn new(p: i32, q: i32) -> Self {
        
        // degenerate cases: either p or q is 0
        // in which case return a unit vector in the right direction
        if p == 0 && q == 0 { return Self {p , q}; }

        if p == 0 {
            if q > 0 { return Self { p:0, q:1 }; }
            else /* q < 0 */ { return Self { p:0, q:-1 } ; }
        }

        if q == 0 { 
            if p > 0 { return Self { p:1, q:0 }; }
            else /* p < 0 */ { return Self { p:-1, q:0}; }
        }

        let pabs = p.abs() as u32;
        let qabs = q.abs() as u32; 
        let pqgcd = pabs.gcd(qabs) as i32;

        return Self { p : p / pqgcd, q : q / pqgcd };
    }

    pub fn quadrant(&self) -> Quadrant {
        if self.p >  0 && self.q >= 0 { return Quadrant::First;  }
        if self.p <= 0 && self.q >  0 { return Quadrant::Second; }
        if self.p >= 0 && self.q <  0 { return Quadrant::Fourth; }
        if self.p <  0 && self.q <= 0 { return Quadrant::Third;  }
        panic!("Cannot place self in quadrant");
    }
    /// assumes (p,q) = (y,x) in cartesian coords
    pub fn angle(&self) -> f64 {
        let temp =  f64::atan2(self.q as f64,self.p as f64);
        let answer : f64 = if temp < 0.0 { temp + 2.0 * std::f64::consts::PI } else { temp };
        return answer;
    }
}

impl Ord for Ratio {
    fn cmp(&self, other: &Self) -> Ordering {
        self.angle().total_cmp(&other.angle())
    }
}

impl PartialOrd for Ratio {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.angle().total_cmp(&other.angle()))
    }
}

#[test]
fn test_ratio_quadrant () {
    // ratio is dy==p, dx==q
    let ratio_1a = Ratio::new(1,0);
    let ratio_1b = Ratio::new(1,1);
    assert_eq!(Quadrant::First, ratio_1a.quadrant());
    assert_eq!(Quadrant::First, ratio_1b.quadrant());
    let ratio_2a = Ratio::new(0,1);
    let ratio_2b = Ratio::new(-1,1);
    assert_eq!(Quadrant::Second, ratio_2a.quadrant());
    assert_eq!(Quadrant::Second, ratio_2b.quadrant());
    let ratio_3a = Ratio::new(-1,0);
    let ratio_3b = Ratio::new(-1,-1);
    assert_eq!(Quadrant::Third, ratio_3a.quadrant());
    assert_eq!(Quadrant::Third, ratio_3b.quadrant());
    let ratio_4a = Ratio::new(0,-1);
    let ratio_4b = Ratio::new(1,-1);
    assert_eq!(Quadrant::Fourth, ratio_4a.quadrant());
    assert_eq!(Quadrant::Fourth, ratio_4b.quadrant());
    assert!(Quadrant::First < Quadrant::Second && Quadrant::First < Quadrant::Third && Quadrant::First < Quadrant::Fourth);
    assert!(Quadrant::Second < Quadrant::Third && Quadrant::Second < Quadrant::Fourth);
    assert!(Quadrant::Third < Quadrant::Fourth);
    assert!(Quadrant::First == Quadrant::First);
    println!("{}", ratio_1a.angle() / std::f64::consts::PI);
    println!("{}", ratio_1b.angle() / std::f64::consts::PI);
    println!("{}", ratio_2a.angle() / std::f64::consts::PI);
    println!("{}", ratio_2b.angle() / std::f64::consts::PI);
    println!("{}", ratio_3a.angle() / std::f64::consts::PI);
    println!("{}", ratio_3b.angle() / std::f64::consts::PI);
    println!("{}", ratio_4a.angle() / std::f64::consts::PI);
    println!("{}", ratio_4b.angle() / std::f64::consts::PI);
    assert!(ratio_1a < ratio_1b && ratio_1b < ratio_2a && ratio_2a < ratio_2b);
}