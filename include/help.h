#pragma once
#define HELP_MESSAGE \
"Find SSRs in FASTA sequence data\n\
\n\
Input:\n\
\n\
  -i in.fasta\n\
                        The input file in fasta format. All sequence characters\n\
                        will be converted to uppercase. [default: stdin]\n\
\n\
                        If your fasta file is compressed, do not use -i. Simply\n\
                        use zcat, bzcat, or a similar tool and pipe it into this\n\
                        program.\n\
\n\
Output:\n\
\n\
  -o out.tsv\n\
                        The output file in tab-separated value (tsv) format.\n\
                        Please see `README' column details. [default: stdout]\n\
\n\
Algorithmic:\n\
\n\
  -a a1,..,aN\n\
                        A comma-separated list of valid, uppercase characters\n\
                        (nucleotides). [default=A,C,G,T]\n\
\n\
  -A\n\
                        Report non-atomic SSRs (e.g., AT repeated 6 times will\n\
                        also report an ATAT repeated 3 times and an ATATAT\n\
                        repeated 2 times).\n\
\n\
  -e\n\
                        Disable all filters and SSR validation to report every\n\
                        SSR. Similar to: -A -r 2 -R <big_number> -n 2 -N\n\
                        <big_number>. This will override any options set for\n\
                        -n, -N, -r, -R, and -s.\n\
\n\
  -p p1,..,pN\n\
                        A comma-separated list of period sizes (i.e., kmer\n\
                        lengths). Inclusive ranges are also supported using a\n\
                        hyphen. [default=4-6]\n\
\n\
  -l int\n\
                        Keep only SSRs with total length (number of\n\
                        nucleotides) >= n [default: 2]\n\
\n\
  -L int\n\
                        Keep only SSRs with total length (number of\n\
                        nucleotides) <= N [default: 10,000]\n\
\n\
  -n int\n\
                        Keep only SSRs with total length (number of\n\
                        nucleotides) >= n [default: 2]\n\
\n\
  -N int\n\
                        Keep only SSRs with total length (number of\n\
                        nucleotides) <= N [default: 10,000]\n\
\n\
  -r int\n\
                        Keep only SSRs that repeat >= r times [default: 2]\n\
\n\
  -R int\n\
                        Keep only SSRs that repeat <= R times [default:\n\
                        10,000]\n\
\n\
  -s s1,..,sN\n\
                        A comma-separated list of SSRs to search for; e.g.\n\
                        \"AC,GTTA,TTCTG,CCG\" or \"TGA\". Please note that other\n\
                        options may prevent SSRs specified with this option\n\
                        from appearing in the output. For example, if -p is\n\
                        \"4-6\", then an SSR with a repeating \"AC\" will never be\n\
                        displayed because \"AC\" has a period size of 2 (and, as\n\
                        it turns out, 2 is not in the range 4-6).\n\
\n\
Misc:\n\
\n\
  -d\n\
                        Disable the progress bar that normally prints to\n\
                        stderr. Will automatically be disabled if (a) reading\n\
                        from stdin or (b) writing to stdout without\n\
                        redirecting it to a file.\n\
\n\
  -h\n\
                        Show this help message and exit\n\
\n\
  -Q int\n\
                        Max size of the tasks queue [default: 1,000]\n\
\n\
  -t int\n\
                        Number of threads [default: 1]\n\
\n\
  -v\n\
                        Show version number and exit\n\
\n"

