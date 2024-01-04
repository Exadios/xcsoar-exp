use std::io::{BufRead, BufReader};
use std::fs::File;
use std::path::Path;
use std::path::Display;
use std::env;
use std::process::exit;
use regex::Regex;

//------------------------------------------------------------------------------
fn main() {
  let args: Vec<String> = env::args().collect();
  dbg!(&args);
  match &args.len() {
    2 => {
      let ip_file = &args[1];
      dbg!(&ip_file);
      // Create a path to the desired file
      let path = Path::new(ip_file);
      let display = path.display();
      // Open the path in read-only mode, returns `io::Result<File>`
      let mut ip = match File::open(&path) {
          Err(why) => panic!("couldn't open {}: {}", display, why),
          Ok(file) => file,
      };
      return process(ip, display);
    }
    _ => {
      println!("{} <ip file>", &args[0]);
      exit(255);
    }
  }
}

//------------------------------------------------------------------------------
fn process(ip : File, display : Display) {
  pass_1(&ip, &display);
  pass_2(&ip, &display);
  pass_3(&ip, &display);
}

/**
 * In which all the assignments are parsed.
 */
//------------------------------------------------------------------------------
fn pass_1(mut ip : &File, display : &Display) {
  let reader = BufReader::new(ip);
  // Read the file line by line using the lines() iterator from std::io::BufRead.
  for line in reader.lines().enumerate() {
    // Show the line and its number.
    let record = line.1.unwrap();
    dbg!(record);
  }
}

/**
 * In which all the evals are resolved.
 */
//------------------------------------------------------------------------------
fn pass_2(ip : &File, display : &Display) {

}

/**
 * In which the Meson output is produced.
 */
//------------------------------------------------------------------------------
fn pass_3(ip : &File, display : &Display) {

}
