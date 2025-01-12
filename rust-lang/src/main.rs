use std::env;
use std::process;

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();

    if args.is_empty() {
        usage();
        process::exit(1);
    }

    let mut max = f64::MIN;
    let mut min = f64::MAX;

    // phase-1: scan for min/max
    for a in args.iter() {
        if let Ok(n) = a.trim().parse::<f64>() {
            max = n.max(max);
            min = n.min(min);
        } else {
            // TODO: only if "verbose" or "strict"
            eprintln!("invalid number: {}", a);
        }
    }

    min = get_f64_from_env("USPARK_MIN", min);
    max = get_f64_from_env("USPARK_MAX", max);

    // TODO: kind of "theme" possible
    // " ░▒▓█";
    let sparks = "▁▂▃▄▅▆▇█";

    let mut scale = 1_f64;
    if min != max {
        scale = (sparks.chars().count() - 1) as f64 / (max - min);
    }

    // phase-2: print sparks
    for a in args.iter() {
        if let Ok(n) = a.trim().parse::<f64>() {
            let n = n.clamp(min, max);
            let s = ((n - min) * scale).floor() as usize;
            print!("{}", sparks.chars().nth(s).unwrap());
        } else {
            // TODO: either skip invalid value or print "hole"
            print!(" ");
        }
    }
    println!("\n");
}

fn usage() {
    println!("uspark <number-1> <number-2> ...")
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
