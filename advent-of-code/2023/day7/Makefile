CC_OPTS = -Wall -O3 -I ../lib -L ../lib -l advent

all: solve solve-p2

solve: solve.c parser.c
	gcc -o $@ $^ $(CC_OPTS)

solve-p2: solve-p2.c almanac.c seed_eval.c
	gcc -o $@ $^ $(CC_OPTS)
