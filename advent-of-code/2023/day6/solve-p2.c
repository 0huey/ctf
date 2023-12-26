#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

const double example_time = 71530;
const double example_distance = 940200;

const double race_time = 45988373;
const double race_dist = 295173412781210;

int quadratic_root_diff(double a, double b, double c);

int main(int argc, char** argv) {
	int example_answer = quadratic_root_diff(
		-1,
		example_time,
		(example_distance + 1) * -1
	);

	int answer = quadratic_root_diff(
		-1,
		race_time,
		(race_dist + 1) * -1
	);

	printf("Example: %d %s\n", example_answer, example_answer == 71503 ? "Correct" : "Wrong");
	printf("Answer: %d\n", answer);

	return 0;
}

int quadratic_root_diff(double a, double b, double c) {
	double root1 = (-b + sqrt(b*b - 4*a*c)) / 2*a;
	double root2 = (-b - sqrt(b*b - 4*a*c)) / 2*a;

	// floor/ceil because we only want integers between the roots
	// +1 because the answer is inclusive
	return (int)(floor(root2) - ceil(root1) + 1);
}
