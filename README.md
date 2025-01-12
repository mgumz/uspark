# uspark

▇▅▇▆▃▄▆▂▃▇▆▂▆▂▂▄▁▂▄▅▅▅▄▇▅▁▇▃▅▆▅▄▄▇▂▆▄

Displays [sparklines](https://en.wikipedia.org/wiki/Sparkline) on the terminal.
`uspark` scans over all given arguments to detect the minimum and maximum
values. Afterwards, it maps each values to a glyph from a defined set to build
the sparkline. The terminal must support UTF-8 encoded characters.

## Usage

    uspark [OPTS --] <number-1> [<number-2> … ]

Options

    -h          - show help
    -bars       - use bars (default)
    -lines      - use lines
    -braille    - use braile characters

Use `--` to separate options from arguments / numbers.

Environment Variables:

- USPARK_MIN: Use `USPARK_MIN` as the minimum value
- USPARK_MAX: Use `USPARK_MAX` as the maximum value

## Examples

Magnitude of earthquakes over 1.0 in the last 24 hours:

```bash
curl -L earthquake.usgs.gov/earthquakes/feed/v1.0/summary/2.5_day.csv --silent | \
    sed '1d' | \
    cut -d, -f5 |\
    xargs ./uspark
▃▁▁▅▁█▆▃▃▅▂▁▂▂▂▁▇▂▆▅▁▂▃▇▅▁▇▅▁█▆▆▆▂▂▅▂
```

Random numbers

```bash
seq 0 40 | sort -R | xargs ./uspark
▁▆▂▂▃▄▇▁▁▂▅▃▂▁▇▆▅▃█▆▁▁▇▄▅▄▃▂▅▂▅▃▇▅▆▃▄▄▆▇▆
```

Cloud coverage for the next x hours, as forecasted by Pirateweather:

```bash
curl -L "https://api.pirateweather.net/forecast/$API_KEY/$LAT,$LONG?units=si" | \
    jq '.hourly|.data[]|.cloudCover' | \
    xargs ./uspark
▇▇██▅▃▂▂▂▁▂▂▂▃▃▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▂▅▄▁▁▁▁
```

## FAQ

### Why the name `uspark`?

The `u` actually stands for `µ` - so it is "mikro spark". It is a little nod
towards [ustat](https://github.com/mgumz/ustat).

### Why are there multiple implementations?

I was interested in how large a self-contained binary written in Rust is in
comparison to a statically linked binary written in C. Numbers:

| Compiler                            | Build                 | Size of binary  |
| ----------------------------------- | --------------------- | --------------- |
| diet-0.34 + gcc-12.2                | diet gcc -lstatic     | 13288           |
| rustc 1.84.0 (9fc6b4312 2025-01-07) | cargo build --release | 371096          |

## License

GPL2, see COPYING

## Building

### C

You need a C-compiler. Then:

    $> cd c-lang
    $> make

### Rust

You need a Rust compiler. Then:

    $> cd rust-lang
    $> cargo build --release

## Related

- https://zachholman.com/spark/
- https://github.com/jiri/rust-spark
- https://www.fefe.de/libowfat/
- https://symbl.cc/en/unicode/blocks/block-elements/
