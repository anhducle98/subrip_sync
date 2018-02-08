#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>

using namespace std;

/*
0. A numeric counter identifying each sequential subtitle
1. The time that the subtitle should appear on the screen, followed by --> and the time it should disappear
2. Subtitle text itself on one or more lines
3. A blank line containing no text, indicating the end of this subtitle
*/

string fixed_width(int n, int length = 2) {
	ostringstream ss;
	ss << setfill('0') << setw(length) << n;
	return ss.str();
}

int calc_exact(int hours, int minutes, int seconds, int milliseconds) {
	int multiplier = 1;
	int res = 0;

	res += milliseconds * multiplier;

	multiplier *= 1000;
	res += seconds * multiplier;

	multiplier *= 60;
	res += minutes * multiplier;

	multiplier *= 60;
	res += hours * multiplier;

	return res;
}

string get_timecode(int milliseconds) {
	string res;

	res = fixed_width(milliseconds % 1000, 3);

	milliseconds /= 1000;
	res = fixed_width(milliseconds % 60) + "," + res;

	milliseconds /= 60;
	res = fixed_width(milliseconds % 60) + ":" + res;

	milliseconds /= 60;
	res = fixed_width(milliseconds) + ":" + res;

	return res;
}

void write_frame(vector<string> &a, int delta) {
	cout << a[0] << endl;
	istringstream ss(a[1]);
	for (int i = 0; i < 2; ++i) {
		int hours, minutes, seconds, milliseconds;
		char delimiter;
		if (i == 1) {
			ss >> delimiter >> delimiter >> delimiter;
		}
		ss >> hours >> delimiter >> minutes >> delimiter >> seconds >> delimiter >> milliseconds;
		int exact_millis = calc_exact(hours, minutes, seconds, milliseconds);
		cout << get_timecode(exact_millis + delta);
		if (i == 0) {
			cout << " --> ";
		} else {
			cout << endl;
		}
	}

	for (int i = 2; i < a.size(); ++i) {
		cout << a[i] << endl;
	}
	cout << endl;

	a.clear();
}

int main(int argc, char **argv) {
	if (argc != 3) {
		cerr << "Usage: ./main <.srt file> <milliseconds>" << endl;
		return 1;
	}
	ifstream f(argv[1]);
	int delta = atoi(argv[2]);
	string line;
	vector<string> current;
	while (getline(f, line)) {
		if (line.back() == '\r') line.pop_back();
		if (line.empty()) {
			write_frame(current, delta);
		} else {
			current.push_back(line);
		}
	}
	f.close();

	return 0;
}