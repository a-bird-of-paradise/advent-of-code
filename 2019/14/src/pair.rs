
#[derive(Copy,Clone,Debug)]
pub struct Pair<'a> {
    pub name: &'a str,
    pub count: i64
}

impl Ord for Pair<'_> { // basically: we only care about the name when building maps
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.name.cmp(&other.name) // ok: actually compares *name not name (a reference)
    }
}

impl PartialOrd for Pair<'_> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Eq for Pair<'_> { }

impl PartialEq for Pair<'_>{
    fn eq(&self, other: &Self) -> bool {
        self.name.eq(other.name) // ok: actually compares *name not name (a reference)
    }
}

#[test]
fn test_pair () {
    let a = Pair { name: "alice", count: 23 };
    let b = Pair { name: "bob", count: 42 };
    let a2 = Pair { name: "alice", count : 12134213 };

    assert_eq!(a,a2); 
    assert!(a < b);
    assert!(a != b);
}