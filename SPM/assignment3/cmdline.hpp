#if !defined _CMDLINE_HPP
#define _CMDLINE_HPP

#include <cstdio>
#include <string>

#include <config.hpp>
#include <utility.hpp>


static inline void usage(const char *argv0) {
    std::printf("--------------------\n");
    std::printf("Usage: %s [options] file-or-directory [file-or-directory]\n", argv0);
    std::printf("\nOptions:\n");
    std::printf(" -r 0 does not recur, 1 will process the content of all subdirectories (default r=%d)\n", RECUR ? 1 : 0);
    std::printf(" -C compress: 0 preserves, 1 removes the original file (default C=%d)\n", REMOVE_ORIGIN && COMP ? 1 : 0);
    std::printf(" -D decompress: 0 preserves, 1 removes the original file (default D=%d)\n", REMOVE_ORIGIN && !COMP ? 1 : 0);
    std::printf(" -q 0 silent mode, 1 prints only error messages to stderr, 2 verbose (default q=%d)\n", QUITE_MODE);
    std::printf("--------------------\n");
}

int parseCommandLine(int argc, char *argv[]) {
    extern char *optarg;
    const std::string optstr = "r:C:D:q:";
    long opt, start = 1;
    bool cpresent = false, dpresent = false;

    while ((opt = getopt(argc, argv, optstr.c_str())) != -1) {
        switch (opt) {
            case 'r': {
                long n = 0;
                if (!isNumber(optarg, n)) {
                    std::fprintf(stderr, "Error: wrong '-r' option\n");
                    usage(argv[0]);
                    return -1;
                }
                RECUR = (n == 1);
                start += 2;
            } break;
            case 'C': {
                long c = 0;
                if (!isNumber(optarg, c)) {
                    std::fprintf(stderr, "Error: wrong '-C' option\n");
                    usage(argv[0]);
                    return -1;
                }
                cpresent = true;
                REMOVE_ORIGIN = (c == 1);
                COMP = true; // Set mode to compression
                start += 2;
            } break;
            case 'D': {
                long d = 0;
                if (!isNumber(optarg, d)) {
                    std::fprintf(stderr, "Error: wrong '-D' option\n");
                    usage(argv[0]);
                    return -1;
                }
                dpresent = true;
                REMOVE_ORIGIN = (d == 1);
                COMP = false; // Set mode to decompression
                start += 2;
            } break;
            case 'q': {
                long q = 0;
                if (!isNumber(optarg, q)) {
                    std::fprintf(stderr, "Error: wrong '-q' option\n");
                    usage(argv[0]);
                    return -1;
                }
                QUITE_MODE = q;
                start += 2;
            } break;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    // Ensure -C and -D are not used together
    if (cpresent && dpresent) {
        std::fprintf(stderr, "Error: -C and -D are mutually exclusive!\n");
        usage(argv[0]);
        return -1;
    }

    // Ensure at least one file or directory is provided
    if ((argc - start) <= 0) {
        std::fprintf(stderr, "Error: at least one file or directory should be provided!\n");
        usage(argv[0]);
        return -1;
    }

    return start;
}

#endif // _CMDLINE_HPP
