use super::Solution;
use crate::input::Input;

pub struct Day21;

impl Solution for Day21 {
    fn day(&self) -> i32 { 21 }
    fn name(&self) -> &'static str { "Dirac Dice" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let mut players = parse_input(input);
        let mut dice = Dice::new();
        'game: loop {
            for player in &mut players {
                let steps = dice.throw() + dice.throw() + dice.throw();
                if player.round(steps) >= 1000 {
                    break 'game;
                }
            }
        }
        let score = players.iter().find(|n| n.score < 1000).unwrap().score * dice.uses;
        Some(Box::new(score))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        const STEPS: [i32; 7] = [3, 4, 5, 6, 7, 8, 9];
        const STEP_VARIANTS: [u64; 7] = [1, 3, 6, 7, 6, 3, 1];

        let players = parse_input(input);
        let mut wins = [0, 0];
        let mut simulations = vec![([players[0], players[1]], 0, 1)];
        while let Some((players, turn, variants)) = simulations.pop() {
            for i in 0..7 {
                let mut players = players;
                if players[turn].round(STEPS[i]) >= 21 {
                    wins[turn] += variants * STEP_VARIANTS[i];
                    continue;
                }
                simulations.push((players, (turn + 1) % players.len(), variants * STEP_VARIANTS[i]));
            }
        }
        Some(Box::new(*wins.iter().max().unwrap()))
    }
}

#[derive(Debug, Clone, Copy)]
struct Player {
    pos: i32,
    score: i32
}

impl Player {
    fn new(pos: i32) -> Player {
        Player { pos: pos, score: 0 }
    }

    fn round(&mut self, steps: i32) -> i32 {
        self.pos = (self.pos - 1 + steps) % 10 + 1;
        self.score += self.pos;
        self.score
    }
}

struct Dice {
    index: i32,
    uses: i32
}

impl Dice {
    fn new() -> Dice {
        Dice { index: 1, uses: 0 }
    }

    fn throw(&mut self) -> i32 {
        self.uses += 1;
        let i = self.index;
        self.index = self.index % 10 + 1;
        i
    }
}

fn parse_input(input: &Input) -> Vec<Player> {
    input.lines.iter().map(|n| Player::new(n.split_whitespace().last().unwrap().parse::<i32>().unwrap())).collect()
}
