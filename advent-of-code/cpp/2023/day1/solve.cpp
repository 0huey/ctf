#include <iostream>
#include <fstream>

using namespace std;

const string digits = "0123456789";

int main(int argc, char* argv[]) {
	if (argc < 2) {
		return 1;
	}

	ifstream chal_file (argv[1]);

	if (!chal_file.is_open()) {
		return 2;
	}

	int result = 0;

	string line;

	while (getline (chal_file, line)) {
		char num[3];
		num[0] = line[ line.find_first_of(digits, 0) ];
		num[1] = line[ line.find_last_of(digits, line.npos) ];
		num[2] = '\0';

		result += stoi( string(num) );
	}

	cout << result << "\n";

	return 0;
}
