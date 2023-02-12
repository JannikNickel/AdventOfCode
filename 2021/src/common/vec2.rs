use std::ops::{Add, AddAssign, Sub, SubAssign, Mul, MulAssign, Div, DivAssign, Neg};

#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Vec2<T> {
    pub x: T,
    pub y: T
}

impl<T> Vec2<T> {
    pub fn new(x: T, y: T) -> Vec2<T> {
        Vec2 { x, y }
    }
}

impl<T> Vec2<T> where T: Add + Sub + Mul {
    pub fn length(&self) {
        todo!("");
    }
}

impl<T> Add for Vec2<T> where T: Add<Output = T> {
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            x: self.x + other.x,
            y: self.y + other.y
        }
    }
}

impl<T> AddAssign for Vec2<T> where T: AddAssign {
    fn add_assign(&mut self, other: Self) {
        self.x += other.x;
        self.y += other.y;
    }
}

impl<T> Sub for Vec2<T> where T: Sub<Output = T> {
    type Output = Self;

    fn sub(self, other: Self) -> Self::Output {
        Self {
            x: self.x - other.x,
            y: self.y - other.y
        }
    }
}

impl<T> SubAssign for Vec2<T> where T: SubAssign {
    fn sub_assign(&mut self, other: Self) {
        self.x -= other.x;
        self.y -= other.y;
    }
}

impl<T> Mul for Vec2<T> where T: Mul<Output = T> {
    type Output = Self;

    fn mul(self, other: Self) -> Self::Output {
        Self {
            x: self.x * other.x,
            y: self.y * other.y
        }
    }
}

impl<T> MulAssign for Vec2<T> where T: MulAssign {
    fn mul_assign(&mut self, other: Self) {
        self.x *= other.x;
        self.y *= other.y;
    }
}

impl<T> Div for Vec2<T> where T: Div<Output = T> {
    type Output = Self;

    fn div(self, other: Self) -> Self::Output {
        Self {
            x: self.x / other.x,
            y: self.y / other.y
        }
    }
}

impl<T> DivAssign for Vec2<T> where T: DivAssign {
    fn div_assign(&mut self, other: Self) {
        self.x /= other.x;
        self.y /= other.y;
    }
}

impl<T> Neg for Vec2<T> where T: Neg<Output = T> {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            x: -self.x,
            y: -self.y
        }
    }
}
