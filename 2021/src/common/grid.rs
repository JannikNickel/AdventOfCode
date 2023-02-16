pub struct Grid<T> where T: Clone {
    width: usize,
    height: usize,
    data: Vec<GridPoint<T>>
}

pub struct GridPoint<T> where T: Clone {
    pub x: usize,
    pub y: usize,
    pub value: T
}

impl<T> Grid<T> where T: Clone {
    pub fn new(width: usize, height: usize, initial: &T) -> Grid<T> {
        let data = (0..(width * height)).map(|i| GridPoint {
            x: i % width,
            y: i / width,
            value: initial.clone()
        }).into_iter().collect::<Vec<GridPoint<T>>>();
        Grid {
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

    pub fn get(&self, x: usize, y: usize) -> &T {
        &self.data[y * self.width + x].value
    }

    pub fn set(&mut self, x: usize, y: usize, value: T) {
        self.data[y * self.width + x].value = value;
    }

    pub fn iter(&self) -> std::slice::Iter<'_, GridPoint<T>> {
        self.data.iter()
    }

    pub fn iter_mut(&mut self) -> std::slice::IterMut<'_, GridPoint<T>> {
        self.data.iter_mut()
    }
}
