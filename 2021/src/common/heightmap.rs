pub struct Heightmap<T> where T: Sized + Copy + Clone {
    width: usize,
    height: usize,
    data: Vec<T>
}

pub struct HeightmapIter<'a, T> where T: Sized + Copy + Clone {
    data: &'a Vec<T>,
    index: usize
}

pub struct HeightmapIter2d<'a, T> where T: Sized + Copy + Clone {
    heightmap: &'a Heightmap<T>,
    index: usize
}

#[derive(Debug, Copy, Clone)]
pub struct HeightmapPoint<T> where T: Sized + Copy + Clone {
    pub x: usize,
    pub y: usize,
    pub value: T
}

pub struct NeighbourIter<'a, T, const N: usize> where T: Sized + Copy + Clone {
    data: &'a Vec<T>,
    n_indices: [i32; N],
    index: usize
}

pub struct NeighbourIter2d<'a, T, const N: usize> where T: Sized + Copy + Clone {
    heightmap: &'a Heightmap<T>,
    n_indices: [i32; N],
    index: usize
}

impl<T> Heightmap<T> where T: Sized + Copy + Clone {
    pub fn new(width: usize, height: usize, initial: T) -> Heightmap<T> {
        let data = (0..(width * height)).map(|_| initial).into_iter().collect::<Vec<T>>();
        Heightmap {
            width,
            height,
            data
        }
    }

    pub fn width(&self) -> usize {
        self.width
    }

    pub fn height(&self) -> usize { 
        self.height
    }

    pub fn get(&self, x: usize, y: usize) -> T {
        self.data[y * self.width + x]
    }

    pub fn set(&mut self, x: usize, y: usize, value: T) {
        self.data[y * self.width + x] = value;
    }

    pub fn i2dto1d(&self, x: i32, y: i32) -> i32 {
        if x < 0 || y < 0 || x as usize >= self.width || y as usize >= self.height {
            return -1;
        }

        y * self.width as i32 + x
    }

    pub fn i1dto2d(&self, i: usize) -> (usize, usize) {
        (i % self.width, i / self.width)
    }

    pub fn iter(&self) -> HeightmapIter<T> {
        HeightmapIter { data: &self.data, index: 0 }
    }

    pub fn iter2d(&self) -> HeightmapIter2d<T> {
        HeightmapIter2d { heightmap: self, index: 0 }
    }

    pub fn vn_neighbour_iter(&self, x: usize, y: usize) -> NeighbourIter<T, 4> {
        let ix = x as i32;
        let iy = y as i32;
        NeighbourIter { data: &self.data, n_indices: [
            self.i2dto1d(ix - 1, iy + 0),
            self.i2dto1d(ix + 0, iy + 1),
            self.i2dto1d(ix + 1, iy + 0),
            self.i2dto1d(ix + 0, iy - 1),
        ], index: 0 }
    }

    pub fn vn_neighbour_iter2d(&self, x: usize, y: usize) -> NeighbourIter2d<T, 4> {
        let ix = x as i32;
        let iy = y as i32;
        NeighbourIter2d { heightmap: &self, n_indices: [
            self.i2dto1d(ix - 1, iy + 0),
            self.i2dto1d(ix + 0, iy + 1),
            self.i2dto1d(ix + 1, iy + 0),
            self.i2dto1d(ix + 0, iy - 1),
        ], index: 0 }
    }

    pub fn moore_neighbour_iter(&self, x: usize, y: usize) -> NeighbourIter<T, 8> {
        let ix = x as i32;
        let iy = y as i32;
        NeighbourIter { data: &self.data, n_indices: [
            self.i2dto1d(ix - 1, iy + 0),
            self.i2dto1d(ix - 1, iy + 1),
            self.i2dto1d(ix + 0, iy + 1),
            self.i2dto1d(ix + 1, iy + 1),
            self.i2dto1d(ix + 1, iy + 0),
            self.i2dto1d(ix + 1, iy - 1),
            self.i2dto1d(ix + 0, iy - 1),
            self.i2dto1d(ix - 1, iy - 1)
        ], index: 0 }
    }
}

impl<'a, T: 'a> Iterator for HeightmapIter<'a, T> where T: Sized + Copy + Clone {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        let i = self.index;
        self.index += 1;
        self.data.get(i)
    }
}

impl<'a, T> Iterator for HeightmapIter2d<'a, T> where T: Sized + Copy + Clone {
    type Item = HeightmapPoint<T>;

    fn next(&mut self) -> Option<Self::Item> {
        let i = self.index;
        self.index += 1;
        if let Some(v) = self.heightmap.data.get(i) {
            return Some(HeightmapPoint { x: self.heightmap.i1dto2d(i).0, y: self.heightmap.i1dto2d(i).1, value: *v });
        }
        None
    }
}

impl<'a, T: 'a, const N: usize> Iterator for NeighbourIter<'a, T, N> where T: Sized + Copy + Clone {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        while self.index < N {
            let i = self.index;
            self.index += 1;
            if let Some(v) = self.data.get(self.n_indices[i] as usize) {
                return Some(v);
            }
        }
        None
    }
}

impl<'a, T: 'a, const N: usize> Iterator for NeighbourIter2d<'a, T, N> where T: Sized + Copy + Clone {
    type Item = HeightmapPoint<T>;

    fn next(&mut self) -> Option<Self::Item> {
        while self.index < N {
            let i = self.n_indices[self.index] as usize;
            self.index += 1;
            if let Some(v) = self.heightmap.data.get(i) {
                return Some(HeightmapPoint { x: self.heightmap.i1dto2d(i).0, y: self.heightmap.i1dto2d(i).1, value: *v });
            }
        }
        None
    }
}
