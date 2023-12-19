CC_OPTS = -Wall -I ../lib -L ../lib -l advent

all: solve solve-p2

solve: solve.c parser.c
	gcc -o $@ $^ $(CC_OPTS)

solve-p2: solve.c parser-p2.c
	gcc -o $@ $^ $(CC_OPTS)
