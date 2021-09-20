use std::{io, thread, time};

fn main() {
    let mut source_str = String::new();
    println!("Enter a string: ");
    io::stdin()
        .read_line(&mut source_str)
        .expect("Failed to read input");

    if source_str.len() < 2 {
        println!("Error: String must be at least two characters.");
    }

    let mut print_string = source_str.clone();
    while print_string.len() > 0 {
        println!("{}", print_string);
        print_string = String::from(&print_string[0..print_string.len() - 1]);
        thread::sleep(time::Duration::from_millis(50));
    }
    while print_string.len() < source_str.len() {
        println!("{}", print_string);
        print_string = String::from(&source_str[0..print_string.len() + 1]);
        thread::sleep(time::Duration::from_millis(50));
    }
}
