/*------------------------------------------------------------------*\
  This software was written by Mathias Gumz <mg@2hoch5.com>
\*------------------------------------------------------------------*/

use std::env;
use std::process;

fn main() {
    let spark_bars = "▁▂▃▄▅▆▇█";
    let spark_lines = "▁⎽⎼‒⎻⎺‾▔";
    let spark_braille = "⣀⠤⠒⠉";
    let spark_shades = " ░▒▓█";

    let mut sparks = &spark_bars;

    let args: Vec<String> = env::args().skip(1).collect();
    let mut numbers: Vec<String> = vec![];

    /*--------------------------------------------------------*\
    \*--------------------------------------------------------*/
    for i in 0..args.len() {
        let a = &args[i];
        if a == "-h" {
            usage();
            process::exit(0);
        } else if a == "-bars" {
            sparks = &spark_bars;
        } else if a == "-lines" {
            sparks = &spark_lines;
        } else if a == "-braille" {
            sparks = &spark_braille;
        } else if a == "-shades" {
            sparks = &spark_shades;
        } else if a == "--" {
            numbers.extend_from_slice(&args[(i + 1)..]);
            break;
        } else {
            numbers.push(a.clone());
        }
    }

    if numbers.is_empty() {
        usage();
        process::exit(1);
    }

    /*--------------------------------------------------------*\
      phase-1: scan all arguments to calculate min/max
    \*--------------------------------------------------------*/

    let mut max = f64::MIN;
    let mut min = f64::MAX;

    // phase-1: scan for min/max
    for n in numbers.iter() {
        if let Ok(n) = n.trim().parse::<f64>() {
            max = n.max(max);
            min = n.min(min);
        } else {
            // TODO: only if "verbose" or "strict"
            eprintln!("invalid number: {}", n);
        }
    }

    min = get_f64_from_env("USPARK_MIN", min);
    max = get_f64_from_env("USPARK_MAX", max);

    let mut scale = 1_f64;
    if min != max {
        scale = (sparks.chars().count() - 1) as f64 / (max - min);
    }

    /*--------------------------------------------------------*\
      phase-2: print sparks
    \*--------------------------------------------------------*/
    for n in numbers.iter() {
        if let Ok(n) = n.trim().parse::<f64>() {
            let n = n.clamp(min, max);
            let s = ((n - min) * scale).floor() as usize;
            print!("{}", sparks.chars().nth(s).unwrap());
        } else {
            // TODO: either skip invalid value or print "hole"
            print!(" ");
        }
    }
    println!();
}

fn usage() {
    println!("uspark - show sparks on the terminal

Usage:

    uspark [OPTS] [n [n ...]]

Options:

    -bars       - use bars (default)
    -lines      - use lines
    -braille    - use braille chars
    -shades     - use shades chars
    -h          - show usage / help");
}

fn get_f64_from_env(name: &str, fallback: f64) -> f64 {
    match env::var(name) {
        Ok(s) => match s.parse::<f64>() {
            Ok(n) => n,
            Err(_) => {
                eprintln!("can't parse value {} for {}", s, name);
                fallback
            }
        },
        Err(_) => fallback,
    }
}
