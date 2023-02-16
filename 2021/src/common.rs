mod vec2;
mod heightmap;
mod grid;

pub mod math {
    pub use super::vec2::Vec2;
}

pub mod structures {
    pub use super::heightmap::{Heightmap, HeightmapPoint};
    pub use super::grid::{Grid, GridPoint};
}
