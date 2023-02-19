use super::Solution;
use crate::input::Input;

pub struct Day16;

impl Solution for Day16 {
    fn day(&self) -> i32 { 16 }
    fn name(&self) -> &'static str { "Packet Decoder" }

    fn run1(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let binary = bin_from_hex(&input.source);
        let mut offset = 0;
        let sum = decode_packet(&binary, &mut offset)?.ver_sum();
        Some(Box::new(sum))
    }

    fn run2(&self, input: &Input) -> Option<Box<dyn std::fmt::Display>> {
        let binary = bin_from_hex(&input.source);
        let mut offset = 0;
        let res = decode_packet(&binary, &mut offset)?.evaluate();
        Some(Box::new(res))
    }
}

#[derive(Debug)]
struct Packet {
    version: usize,
    data: PacketData
}

#[derive(Debug)]
enum PacketData {
    Literal(usize),
    Operator(usize, Vec<Packet>)
}

impl Packet {
    fn ver_sum(&self) -> usize {
        let mut sum = self.version;
        if let PacketData::Operator(_, sub) = &self.data {
            for p in sub {
                sum += p.ver_sum();
            }
        }
        sum
    }

    fn evaluate(&self) -> usize {
        match &self.data {
            PacketData::Literal(v) => *v,
            PacketData::Operator(op, sub) => {
                self.evaluate_value(sub, match op {
                    0 => &|a, b| a + b,
                    1 => &|a, b| a * b,
                    2 => &|a, b| usize::min(a, b),
                    3 => &|a, b| usize::max(a, b),
                    5 => &|a, b| if a > b { 1 } else { 0 },
                    6 => &|a, b| if a < b { 1 } else { 0 },
                    7 => &|a, b| if a == b { 1 } else { 0 },
                    _ => &|a, b| a + b
                })
            }
        }
    }

    fn evaluate_value(&self, packets: &Vec<Packet>, func: &dyn Fn(usize, usize) -> usize) -> usize {
        let mut v = packets[0].evaluate();
        for p in packets.iter().skip(1) {
            v = func(v, p.evaluate());
        }
        v
    }    
}

fn decode_packet(bin: &str, offset: &mut usize) -> Option<Packet> {
    if *offset + 7 > bin.chars().count() {
        return None;
    }

    let version = bin_to_dec(read_n(bin, offset, 3));
    let id = bin_to_dec(read_n(bin, offset, 3));
    let data = match id {
        4 => {
            let mut nibbles = Vec::new();
            loop {
                let end = read_n(bin, offset, 1).chars().nth(0).unwrap() == '0';
                nibbles.push(read_n(bin, offset, 4));
                if end {
                    break;
                }
            }
            let n = bin_to_dec(nibbles.join("").as_str());
            PacketData::Literal(n)
        },
        _ => {
            let mode = read_n(bin, offset, 1).chars().nth(0).unwrap();
            let l_bits = if mode == '0' { 15 } else { 11 };
            let l = bin_to_dec(read_n(bin, offset, l_bits));
            let mut sub_packets = Vec::new();
            if mode == '0' {
                let sub_str = read_n(bin, offset, l);
                let mut sub_off = 0;
                while let Some(sub) = decode_packet(sub_str, &mut sub_off) {
                    sub_packets.push(sub);
                }
            } else {
                for _ in 0..l {
                    sub_packets.push(decode_packet(bin, offset).unwrap());
                }
            }
            PacketData::Operator(id, sub_packets)
        }
    };
    Some(Packet { version, data })
}

fn read_n<'a>(bin: &'a str, offset: &mut usize, amount: usize) -> &'a str {
    let r = &bin[*offset..(*offset + amount)];
    *offset += amount;
    r
}

fn bin_to_dec(bin: &str) -> usize{
    usize::from_str_radix(bin, 2).unwrap()
}

fn bin_from_hex(hex: &str) -> String {
    hex.chars().map(|c| hex_to_bin(c)).collect()
}

fn hex_to_bin(c: char) -> &'static str {
    match c {
        '0' => "0000",
        '1' => "0001",
        '2' => "0010",
        '3' => "0011",
        '4' => "0100",
        '5' => "0101",
        '6' => "0110",
        '7' => "0111",
        '8' => "1000",
        '9' => "1001",
        'A' => "1010",
        'B' => "1011",
        'C' => "1100",
        'D' => "1101",
        'E' => "1110",
        'F' => "1111",
        _ => ""
    }
}
