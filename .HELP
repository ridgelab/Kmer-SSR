Find SSRs in FASTA sequence data 

Input:

  -i in.fasta
                        The input file in fasta format. All sequence characters
                        will be converted to uppercase. [default: stdin]

                        If your fasta file is compressed, do not use -i. Simply
                        use zcat, bzcat, or a similar tool and pipe it into this
                        program.

Output:

  -o out.tsv
                        The output file in tab-separated value (tsv) format.
                        Please see `README' column details. [default: stdout]

Algorithmic:

  -a a1,..,aN
                        A comma-separated list of valid, uppercase characters
                        (nucleotides). [default=A,C,G,T]

  -A
                        Report non-atomic SSRs (e.g., AT repeated 6 times will
                        also report an ATAT repeated 3 times and an ATATAT
                        repeated 2 times).

  -e
                        Disable all filters and SSR validation to report every
                        SSR. Similar to: -A -r 2 -R <big_number> -n 2 -N
                        <big_number>. This will override any options set for
                        -n, -N, -r, -R, and -s.

  -p p1,..,pN
                        A comma-separated list of period sizes (i.e., kmer
                        lengths). Inclusive ranges are also supported using a
                        hyphen. [default=4-6]

  -l int
                        Keep only SSRs with total length (number of
                        nucleotides) >= n [default: 2]

  -L int
                        Keep only SSRs with total length (number of
                        nucleotides) <= N [default: 10,000]

  -n int
                        Keep only SSRs with total length (number of
                        nucleotides) >= n [default: 2]

  -N int
                        Keep only SSRs with total length (number of
                        nucleotides) <= N [default: 10,000]

  -r int
                        Keep only SSRs that repeat >= r times [default: 2]

  -R int
                        Keep only SSRs that repeat <= R times [default:
                        10,000]

  -s s1,..,sN
                        A comma-separated list of SSRs to search for; e.g.
                        \"AC,GTTA,TTCTG,CCG\" or \"TGA\". Please note that other
                        options may prevent SSRs specified with this option
                        from appearing in the output. For example, if -p is
                        \"4-6\", then an SSR with a repeating \"AC\" will never be
                        displayed because \"AC\" has a period size of 2 (and, as
                        it turns out, 2 is not in the range 4-6).

Misc:

  -d
                        Disable the progress bar that normally prints to
                        stderr. Will automatically be disabled if (a) reading
                        from stdin or (b) writing to stdout without
                        redirecting it to a file.

  -h
                        Show this help message and exit

  -Q int
                        Max size of the tasks queue [default: 1,000]

  -t int
                        Number of threads [default: 1]

  -v
                        Show version number and exit

