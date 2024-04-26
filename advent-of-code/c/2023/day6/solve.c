#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define NUM_EXAMPLE_RACES 3

const double example_time[NUM_EXAMPLE_RACES]     = {7, 15, 30};
const double example_distance[NUM_EXAMPLE_RACES] = {9, 40 ,200};

#define NUM_RACES 4

const double race_time[NUM_RACES] = {45, 98, 83, 73};
const double race_dist[NUM_RACES] = {295, 1734, 1278, 1210};

int quadratic_root_diff(double a, double b, double c);

int main(int argc, char** argv) {
	int example_answer = 1;

	for (int i = 0; i < NUM_EXAMPLE_RACES; i++) {
		//-1charge_time^2 + time_limit*charge_time - minimum_distance
		// a                    b                           c

		example_answer *= quadratic_root_diff(
			-1,
			example_time[i],
			(example_distance[i] + 1) * -1
		);
	}

	int answer = 1;

	for (int i = 0; i < NUM_RACES; i++) {
		answer *= quadratic_root_diff(
			-1,
			race_time[i],
			(race_dist[i] + 1) * -1
		);
	}

	printf("Example: %d %s\n", example_answer, example_answer == 288 ? "Correct" : "Wrong");
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
