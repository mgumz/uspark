/*------------------------------------------------------------------*\
  This software was written by Mathias Gumz <mg@2hoch5.com>
\*------------------------------------------------------------------*/

#include "libowfat/scan.h"
#include "libowfat/str.h"

#include <sys/types.h>
#include <unistd.h>
#include <float.h>

/*------------------------------------------------------------------*\
\*------------------------------------------------------------------*/

static inline double max_of(double, double);
static inline double min_of(double, double);
static inline double clamp(double lower, double n, double upper);
static inline int get_double_from_env(const char** env, const char* name, double *n);
static inline ssize_t write_nl(int fd, const char* buf, size_t blen);
static inline int usage();

/*------------------------------------------------------------------*\
\*------------------------------------------------------------------*/

const char errParseMin[] = "error: parse USPARK_MIN";
const char errParseMax[] = "error: parse USPARK_MAX";
const char errUndefinedMinMax[] = "error: min / max undefined";
const char errMinLargerAsMax[] = "error: USPARK_MIN > USPARK_MAX";

/*------------------------------------------------------------------*\
\*------------------------------------------------------------------*/

const char* themeBars[] =    { "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", };
const char* themeLines[] =   { "▁", "⎽", "⎼", "‒", "⎻", "⎺", "‾", "▔", };
const char* themeBraille[] = { "⣀", "⠤", "⠒", "⠉", };

/*------------------------------------------------------------------*\
\*------------------------------------------------------------------*/

int main(int argc, char* argv[]) {

    double min = DBL_MAX;
    double max = DBL_MIN;

    const char** sparks = themeBars;
    size_t n_sparks = sizeof(themeBars) / sizeof(themeBars[0]);

    /*--------------------------------------------------------*\
    \*--------------------------------------------------------*/
    for (int i = 1; i < argc; i++) {
        if (str_diff(argv[i], "-h") == 0) {
            usage();
            return 0;
        } else if (str_diff(argv[i], "-lines") == 0) {
            sparks = themeLines;
            n_sparks = sizeof(themeLines) / sizeof(themeLines[0]);
        } else if (str_diff(argv[i], "-bars") == 0) {
            sparks = themeBars;
            n_sparks = sizeof(themeBars) / sizeof(themeBars[0]);
        } else if (str_diff(argv[i], "-braille") == 0) {
            sparks = themeBraille;
            n_sparks = sizeof(themeBraille) / sizeof(themeBraille[0]);
        } else if (str_diff(argv[i], "--") == 0) {
            argv = &argv[i];
            argc = argc - i;
            break;
        }
    }
    if (argc <= 1) {
        usage();
        return 0;
    }

    /*--------------------------------------------------------*\
      phase-1: scan all arguments to calculate min/max
    \*--------------------------------------------------------*/
    for (int i = 1; i < argc; i++) {
        double n = 0;
        const size_t sl = str_len(argv[i]);
        const size_t l = scan_double(argv[i], &n);
        if (l != sl) { // parsing failed
            write_nl(2, argv[i], str_len(argv[i]));
            continue;
        }

        min = min_of(min, n);
        max = max_of(max, n);
    }

    /*--------------------------------------------------------*\
      phase "1.5" - based upon min/max: calculate how to
      map the seen numbers to the spark-lines.
    \*--------------------------------------------------------*/
    extern const char** environ;
    if (get_double_from_env(environ, "USPARK_MIN=", &min) == 2) {
        write_nl(2, errParseMin, sizeof(errParseMin));
        return 2;
    }
    if (get_double_from_env(environ, "USPARK_MAX=", &max) == 2) {
        write_nl(2, errParseMax, sizeof(errParseMax));
        return 2;
    }

    if (min == DBL_MAX || max == DBL_MIN) {
        write_nl(2, errUndefinedMinMax, sizeof(errUndefinedMinMax));
        return 3;
    }

    if (min > max) {
        write_nl(2, errMinLargerAsMax, sizeof(errMinLargerAsMax));
        return 4;
    }


    double scale = 1.0;
    if (min != max) {
        scale = (double)(n_sparks - 1) / (max - min);
    }

    /*--------------------------------------------------------*\
      phase-2: scan all arguments and print sparks
    \*--------------------------------------------------------*/
    for (int i = 1; i < argc; i++) {
        double n = 0;
        const size_t sl = str_len(argv[i]);
        const size_t l = scan_double(argv[i], &n);

        if (l != sl) { // parsing failed
            write(2, argv[i], sl);
            write(1, " ", 1);
            continue;
        }

        n = clamp(min, n, max);

        const size_t s = (size_t)((n - min) * scale);
        write(1, sparks[s], str_len(sparks[s]));
    }

    write(1, "\n", 1);

    return 0;
}

static inline int usage() {

    static const char txt[] = "uspark [n [n ...]]";
    write_nl(1, txt, sizeof(txt));
    return 0;
}

static inline ssize_t write_nl(int fd, const char* buf, size_t blen) {

    ssize_t n = write(fd, buf, blen);
    n += write(fd, "\n", 1);
    return n;
}

// try to get `name` from `env`
// example: FOO=1.0
// rc:
//  * 0 - not found
//  * 1 - found, parsing ok
//  * 2 - found, invalid double
static inline int get_double_from_env(const char** env, const char* name, double *n) {

    int found = -1;
    for (int i = 0; env[i] != NULL; i++) {
        if (str_start(env[i], name) == 1) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        return 0;
    }

    const char* v = env[found] + str_len(name);
    double d = 0;
    size_t s = str_len(v);
    size_t sl = scan_double(v, &d);
    if (s == sl) { // parsing ok
        *n = d;
        return 1;
    }

    return 2;
}

static inline double clamp(double lower, double n, double upper) {
    if (lower > n) {
        n = lower;
    }
    if (upper < n) {
        n = upper;
    }
    return n;
}

static inline double max_of(double a, double b) {
    if (a > b) {
        return a;
    }
    return b;
}

static inline double min_of(double a, double b) {
    if (a < b) {
        return a;
    }
    return b;
}

