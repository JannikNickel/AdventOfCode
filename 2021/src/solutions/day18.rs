use super::Solution;
use crate::input::Input;

pub struct Day18;

impl Solution for Day18 {
    fn day(&self) -> i32 { 18 }
    fn name(&self) -> &'static str { "Snailfish" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let numbers = parse_input(input);
        let mut num = numbers[0].clone();
        for other in numbers.iter().skip(1) {
            add(&mut num, other);
        }
        let res = magnitude(&num, 0, num.len());
        Some(Box::new(res))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let numbers = parse_input(input);
        let mut max_sum = 0;
        for num_l in numbers.iter() {
            for num_r in numbers.iter() {
                let mut res = num_l.clone();
                add(&mut res, num_r);
                max_sum = i32::max(max_sum, magnitude(&res, 0, res.len()));
            }
        }
        Some(Box::new(max_sum))
    }
}

fn magnitude(num: &Vec<Token>, mut from: usize, mut to: usize) -> i32 {
    if to - from == 1 {
        if let Token::Literal(v) = num[from] {
            return v;
        }
    }
    
    let mut sum = 0;
    from += 1;
    to -= 1;
    let mut depth = 0;
    let mut comma = 0;
    for i in from..to {
        match num[i] {
            Token::Open => depth += 1,
            Token::Close => depth -= 1,
            Token::Comma if depth == 0 => {
                comma = i;
                break;
            },
            _ => {}
        }
    }

    if comma != 0 {
        sum += 3 * magnitude(num, from, comma) + 2 * magnitude(num, comma + 1, to);
    } else {
        sum += magnitude(num, from, to);
    }
    sum
}

fn add(lhs: &mut Vec<Token>, rhs: &Vec<Token>) {
    lhs.insert(0, Token::Open);
    lhs.push(Token::Comma);
    for token in rhs {
        lhs.push(*token);
    }
    lhs.push(Token::Close);

    explode_all(lhs);
    while split(lhs) { }
}

fn explode_all(num: &mut Vec<Token>) {
    while explode(num) { }
}

fn explode(num: &mut Vec<Token>) -> bool {
    let mut depth = 0;
    for i in 0..num.len() {
        match num[i] {
            Token::Open => depth += 1,
            Token::Close => depth -= 1,
            Token::Literal(_) if depth > 4 => {
                if let Some((l_lit, r_lit, end)) = read_pair(num, i - 1) {
                    explode_pair(num, i - 1, end, l_lit, r_lit);
                    return true;
                }
            },
            _ => {}
        }
    }
    false
}

fn explode_pair(num: &mut Vec<Token>, s_index: usize, e_index: usize, l_lit: i32, r_lit: i32) {
    for _ in 0..(e_index - s_index) {
        num.remove(s_index);
    }

    let left = left_lit(num, s_index);
    let right = right_lit(num, s_index);
    if let Some(li) = left {
        inc_lit(&mut num[li], l_lit);
    }
    if let Some(ri) = right {
        inc_lit(&mut num[ri], r_lit);
    }
    num.insert(s_index, Token::Literal(0));
}

fn split(num: &mut Vec<Token>) -> bool {
    for i in 0..num.len() {
        if let Token::Literal(v) = num[i] {
            if v >= 10 {
                split_lit(num, i, v);
                explode_all(num);
                return true;
            }
        }
    }
    false
}

fn split_lit(num: &mut Vec<Token>, index: usize, lit: i32) {
    num.remove(index);
    num.insert(index + 0, Token::Open);
    num.insert(index + 1, Token::Literal(lit / 2));
    num.insert(index + 2, Token::Comma);
    num.insert(index + 3, Token::Literal((lit + 1) / 2));
    num.insert(index + 4, Token::Close)
}

fn read_pair(num: &Vec<Token>, s_index: usize) -> Option<(i32, i32, usize)> {
    if s_index + 4 >= num.len() {
        return None;
    }
    if let [t0, t1, t2, t3, t4] = num[s_index..(s_index + 5)] {
        if t0 == Token::Open && t2 == Token::Comma && t4 == Token::Close {
            if let Token::Literal(l_lit) = t1 {
                if let Token::Literal(r_lit) = t3 {
                    return Some((l_lit, r_lit, s_index + 5));
                }
            }
        }
    }
    None
}

fn left_lit(num: &Vec<Token>, mut from: usize) -> Option<usize> {
    loop {
        if matches!(num[from], Token::Literal(_)) {
            return Some(from);
        }
        if from == 0 {
            break;
        }
        from -= 1;
    }
    None
}

fn right_lit(num: &Vec<Token>, mut from: usize) -> Option<usize> {
    while from < num.len() {
        if matches!(num[from], Token::Literal(_)) {
            return Some(from);
        }
        from += 1;
    }
    None
}

fn inc_lit(lit: &mut Token, increment: i32) {
    if let Token::Literal(v) = lit {
        *v += increment;
    }
}

fn parse_input(input: &Input) -> Vec<Vec<Token>> {
    let mut numbers = Vec::new();
    for line in &input.lines {
        let mut tokens = Vec::new();
        for c in line.chars() {
            if c != ' ' {
                tokens.push(match c {
                    '[' => Token::Open,
                    ']' => Token::Close,
                    ',' => Token::Comma,
                    _ => Token::Literal(c.to_digit(10).unwrap() as i32)
                });
            }
        }
        numbers.push(tokens);
    }
    numbers
}

#[derive(Debug, Clone, Copy, PartialEq)]
enum Token {
    Open,
    Close,
    Comma,
    Literal(i32)
}
