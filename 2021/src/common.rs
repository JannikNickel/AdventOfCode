mod vec2;
mod heightmap;

pub mod math {
    pub use super::vec2::Vec2;
}

pub mod structures {
    pub use super::heightmap::{Heightmap, HeightmapPoint};
}
