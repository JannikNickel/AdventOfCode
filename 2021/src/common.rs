mod vec2;
mod vec3;
mod heightmap;
mod grid;
mod graph;

pub mod math {
    pub use super::vec2::Vec2;
    pub use super::vec3::Vec3;
}

pub mod structures {
    pub use super::heightmap::{Heightmap, HeightmapPoint};
    pub use super::grid::{Grid, GridPoint};
    pub use super::graph::Graph;
}
