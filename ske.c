/* ID Header:
 Student Name:
 Student ID:
 Submission Date:
 File:
*/

#include "vmsim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// usage
static void usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s --mode=bb  --base=N --limit=N --trace=FILE \n"
        "  %s --mode=seg --config=FILE --trace=FILE \n",
        prog, prog);
}

// CLI
bool parse_args(int argc, char **argv, sim_opts_t *o) {
    (void)argc; (void)argv;
    if (o) memset(o, 0, sizeof(*o));
    fprintf(stderr, "TODO: parse_args()\n");
    return false;
}

//bb
int run_bb(const sim_opts_t *o, stats_t *st) {
    (void)o; (void)st;
    fprintf(stderr, "TODO: run_bb()\n");
    return 0;
}

//seg
int run_seg(const sim_opts_t *o, stats_t *st) {
    (void)o; (void)st;
    fprintf(stderr, "TODO: run_seg()\n");
    return 0;
}

//main()
int main(int argc, char **argv) {
    sim_opts_t opts;
    if (!parse_args(argc, argv, &opts)) { usage(argv[0]); return 1; }
    stats_t st = (stats_t){0};
    if (opts.mode == MODE_BB) return run_bb(&opts, &st);
    else return run_seg(&opts, &st);
}
