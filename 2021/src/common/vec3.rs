use std::ops::{Add, AddAssign, Sub, SubAssign, Mul, MulAssign, Div, DivAssign, Neg};
use std::cmp::Eq;

#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
pub struct Vec3 {
    pub x: i32,
    pub y: i32,
    pub z: i32
}

impl Vec3 {
    pub fn new(x: i32, y: i32, z: i32) -> Vec3 {
        Vec3 { x, y, z }
    }

    pub fn length(&self) -> i32 {
        i32::abs(self.x) + i32::abs(self.y) + i32::abs(self.z)
    }
}

impl Add for Vec3 {
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z
        }
    }
}

impl AddAssign for Vec3 {
    fn add_assign(&mut self, other: Self) {
        self.x += other.x;
        self.y += other.y;
        self.z += other.z;
    }
}

impl Sub for Vec3 {
    type Output = Self;

    fn sub(self, other: Self) -> Self::Output {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z
        }
    }
}

impl SubAssign for Vec3 {
    fn sub_assign(&mut self, other: Self) {
        self.x -= other.x;
        self.y -= other.y;
        self.z -= other.z;
    }
}

impl Mul<i32> for Vec3 {
    type Output = Self;

    fn mul(self, s: i32) -> Self::Output {
        Self {
            x: self.x * s,
            y: self.y * s,
            z: self.z * s
        }
    }
}

impl MulAssign<i32> for Vec3 {
    fn mul_assign(&mut self, s: i32) {
        self.x *= s;
        self.y *= s;
        self.z *= s;
    }
}

impl Div<i32> for Vec3 {
    type Output = Self;

    fn div(self, s: i32) -> Self::Output {
        Self {
            x: self.x / s,
            y: self.y / s,
            z: self.z / s
        }
    }
}

impl DivAssign<i32> for Vec3 {
    fn div_assign(&mut self, s: i32) {
        self.x /= s;
        self.y /= s;
        self.z /= s;
    }
}

impl Neg for Vec3 {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            x: -self.x,
            y: -self.y,
            z: -self.z
        }
    }
}
