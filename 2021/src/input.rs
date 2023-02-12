use std::path::Path;
use std::error::Error;
use std::str::FromStr;

#[derive(Debug)]
pub struct Input {
    pub source: String,
    pub lines: Vec<String>
}

impl Input {
    pub fn build(p: &Path) -> Result<Input, Box<dyn Error>> {
        let s = std::fs::read_to_string(p)?;
        let lines: Vec<String> = s.lines().map(|f| String::from_str(f).unwrap()).collect::<Vec<String>>();
        Ok(Input {
            source: s,
            lines: lines
        })
    }
}
