use std::{collections::{HashMap, HashSet}, hash::Hash};

#[derive(Debug)]
pub struct Graph<T> where T: Eq + Hash + Clone {
    nodes: HashMap<T, usize>,
    connections: Vec<HashSet<usize>>
}

pub struct GraphIter<'a, T> where T: Eq + Hash + Clone {
    nodes: Vec<&'a T>,
    index: usize
}

impl<T> Graph<T> where T: Eq + Hash + Clone {
    pub fn new() -> Graph<T> {
        Graph {
            nodes: HashMap::new(),
            connections: Vec::new()
        }
    }

    fn index_of(&self, node: &T) -> Option<usize> {
        self.nodes.get(node).copied()
    }

    fn node_at(&self, index: usize) -> Option<&T> {
        for (node, i) in &self.nodes {
            if *i == index {
                return Some(&node);
            }
        }
        None
    }

    pub fn insert(&mut self, node: &T) -> usize {
        match self.nodes.get(node) {
            Some(&v) => v,
            None => {
                let i = self.nodes.len();
                self.nodes.insert(node.clone(), i);
                self.connections.push(HashSet::new());
                i
            }
        }
    }

    pub fn connect(&mut self, a: &T, b: &T) {
        let ai = self.insert(a);
        let bi = self.insert(b);
        self.connections[ai].insert(bi);
        self.connections[bi].insert(ai);
    }

    pub fn disconnect(&mut self, a: &T, b: &T) {
        if let (Some(ai), Some(bi)) = (self.index_of(a), self.index_of(b)) {
            self.connections[ai].remove(&bi);
            self.connections[bi].remove(&ai);
        }
    }

    pub fn neighbours(&self, node: &T) -> GraphIter<T> {
        GraphIter {
            nodes: match self.index_of(node) {
                Some(i) => self.connections[i].iter().map(|n| self.node_at(*n).unwrap()).collect(),
                None => Vec::new()
            },
            index: 0
        }
    }

    pub fn nodes(&self) -> GraphIter<T> {
        GraphIter {
            nodes: self.nodes.iter().map(|n| n.0).collect(),
            index: 0
        }
    }
}

impl<'a, T: 'a> Iterator for GraphIter<'a, T> where T: Eq + Hash + Clone {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        let i= self.index;
        self.index += 1;
        if let Some(n) = self.nodes.get(i) {
            return Some(*n);
        }
        None
    }
}
