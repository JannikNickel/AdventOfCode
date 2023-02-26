use std::ops::{Add, AddAssign, Sub, SubAssign, Mul, MulAssign, Div, DivAssign, Neg, Index, IndexMut};
use std::cmp::Eq;

#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
pub struct Vec2 {
    pub x: i32,
    pub y: i32
}

impl Vec2 {
    pub fn new(x: i32, y: i32) -> Vec2 {
        Vec2 { x, y }
    }

    pub fn length(&self) -> i32 {
        i32::abs(self.x) + i32::abs(self.y)
    }
}

impl Index<usize> for Vec2 {
    type Output = i32;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.x,
            1 => &self.y,
            _ => panic!("Index out of range!")
        }
    }
}

impl IndexMut<usize> for Vec2 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.x,
            1 => &mut self.y,
            _ => panic!("Index out of range!")
        }
    }
}

impl Add for Vec2 {
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            x: self.x + other.x,
            y: self.y + other.y
        }
    }
}

impl AddAssign for Vec2 {
    fn add_assign(&mut self, other: Self) {
        self.x += other.x;
        self.y += other.y;
    }
}

impl Sub for Vec2 {
    type Output = Self;

    fn sub(self, other: Self) -> Self::Output {
        Self {
            x: self.x - other.x,
            y: self.y - other.y
        }
    }
}

impl SubAssign for Vec2 {
    fn sub_assign(&mut self, other: Self) {
        self.x -= other.x;
        self.y -= other.y;
    }
}

impl Mul<i32> for Vec2 {
    type Output = Self;

    fn mul(self, s: i32) -> Self::Output {
        Self {
            x: self.x * s,
            y: self.y * s
        }
    }
}

impl MulAssign<i32> for Vec2 {
    fn mul_assign(&mut self, s: i32) {
        self.x *= s;
        self.y *= s;
    }
}

impl Div<i32> for Vec2 {
    type Output = Self;

    fn div(self, s: i32) -> Self::Output {
        Self {
            x: self.x / s,
            y: self.y / s
        }
    }
}

impl DivAssign<i32> for Vec2 {
    fn div_assign(&mut self, s: i32) {
        self.x /= s;
        self.y /= s;
    }
}

impl Neg for Vec2 {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            x: -self.x,
            y: -self.y
        }
    }
}
