use std::fs::File;
use std::io::BufReader;
use std::io::prelude::*;

fn main() -> std::io::Result<()> {
    let f = File::open("example.txt")?;
    let reader = BufReader::new(f);

	let mut result: i32 = 0;

	for line in reader.lines() {
		let digits: Vec<char> = line?
			.chars()
			.filter(|x| x.is_digit(10))
			.collect();

		if digits.is_empty() {
			panic!("a line has no digits");
		}

		let num_first_last = vec![digits.first(), digits.last()];

		let num: i32 = String::from_iter(&num_first_last)
			.parse::<i32>()
			.unwrap();

		result += num;
	}
	println!("{}", result);
    Ok(())
}
