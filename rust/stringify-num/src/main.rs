use std::env;
use std::io;

const NUMBER_DIGITS: [&str; 10] = [
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
];

const NUMBER_PREFIXES: [&str; 8] = [
    "twen", "thir", "four", "fif", "six", "seven", "eigh", "nine"
];

const HUNDRED_SUFFIXES: [&str; 6] = [
    "thousand", "million", "billion", "trillion", "quadrillion", "quintillion"
];

fn stringify_hundred(number: i64) -> String {
	let mut result = String::from("");

	// Specially handle numbers under 10 (specifically for zero).
	if number < 10 {
		return String::from(NUMBER_DIGITS[number as usize]);
	}

	// Add hundreds
	if number >= 100 {
		result = String::from(NUMBER_DIGITS[(number / 100 % 10) as usize]) + " hundred";
		// If there are no remaining digits, exit early.
		if number % 100 == 0 {
			return result;
		} else {
			result += " ";
		}
	}
	// Check for the teen range and handle it specially.
	if number % 100 < 20 && number % 100 > 9 {
		match number % 10 {
			0 => result += "ten",
			1 => result += "eleven",
			2 => result += "twelve",
			_ => {
				result += NUMBER_PREFIXES[(number % 10 - 2) as usize];
				result += "teen";
			}
		};
		return result;
	} else if number % 100 >= 20 {
		result += NUMBER_PREFIXES[(number / 10 % 10 - 2) as usize];
		result += "ty";
		if number % 10 == 0 {
			return result;
		} else {
			result += " ";
		}
	}
	return result + NUMBER_DIGITS[(number % 10) as usize];
}

fn stringify_number(mut number: i64) -> String {
	let negative = number < 0;
	number = number.abs();
	let mut result = stringify_hundred(number % 1000);

	let mut factor: i64 = 1000;
	let mut factor_index = 0;
	while number >= factor {
		let next_hundred = stringify_hundred(number / factor % 1000) + " " + HUNDRED_SUFFIXES[factor_index];
		if number % factor != 0 {
			result.insert_str(0, " ");
			result.insert_str(0, &next_hundred);
		} else {
			result = next_hundred;
		}
		factor *= 1000;
		factor_index += 1;
	}

	if negative {
		result.insert_str(0, "negative ");
	}
	return result;
}

fn main() {
	let source_num: isize;
	if env::args().len() >= 2 {
		let args: Vec<String> = env::args().collect();
		source_num = args[1].parse().unwrap();
	} else {
		let mut source_str: String = String::new();
		println!("Enter a number: ");
		io::stdin()
			.read_line(&mut source_str)
			.expect("Failed to read input");
		source_num = source_str[..source_str.len() - 1].parse().unwrap();
	}
	println!("Number {} is {}", source_num, stringify_number(source_num as i64));
}
