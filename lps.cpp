// Implementation of a linear time algorithm that find the longest palindromic
// substring of a given string.
//
// Author: Mikhail Andrenkov
// Date: October 22, 2017
//
// Credit: This code is a modification of a Manacher's Algorithm implementation
//         from https://algs4.cs.princeton.edu/53substring/Manacher.java.html
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string expand(const std::string&);
std::string solve(const std::string&);

// Returns the longest palindromic substring of |given|.
std::string solve(const std::string &given) {
	// Expand the given string to include all palindrome indexes.
	std::string pad = expand(given);
	// Declare the LPS array to store the longest palindromic substring values.
	std::vector<int> lps(pad.size(), 1);

	// The center of the active palindrome.
	int c = 0;
	// The rightmost index of the active palindrome;
	int r = 0;
	for (int i = 0; i < pad.size(); ++i) {
		// The center index of the mirror palindrome.
		int j = 2*c - i;
		// If |i| is past |r|, no information can be recycled.
		if (i < r) {
			lps[i] = std::min(r - i, lps[j]);
		}

		// Check if the current palindrome extends beyond the active palindrome.
		int l = lps[i];
		while (i - l >= 0 && i + l < pad.size() && pad[i - l] == pad[i + l]) {
			++lps[i];
			++l;
		}

		// Update the active palindrome if the current palindrome reaches further right.
		if (i + lps[i] - 1 > r) {
			c = i;
			r = i + lps[i] - 1;
		}
	}

	// Extract the longest palindrome.
	auto it = std::max_element(lps.begin(), lps.end());
	int index = std::distance(lps.begin(), it);
	int size = lps[index];

	// Convert the expanded index and length to match the original string.
	int start = (index + 1)/2 - size/2;
	int length = std::max(1, size - 1);
	return given.substr(start, length);
}

// Returns |given| with a "|" inserted before and after character.
std::string expand(const std::string &given) {
	std::string buffer = "|";
	for (const auto &c : given) {
		buffer += c;
		buffer += "|";
	}
	return buffer;
}

// Execution entry point.
int main() {
	// Declare some sanity-check tests.
	std::vector<std::pair<const std::string, const std::string>> tests = {
		{"a", "a"},
		{"aba", "aba"},
		{"xabbab", "abba"},
		{"xababay", "ababa"}
	};

	for (const auto &p : tests) {
		std::string given = p.first;
		std::string want = p.second;
		std::string lps = solve(given);

		std::string result = want == lps ? "PASS" : "FAIL";
		std::cout << result << ": ";
		std::cout << "solve(\"" << given << "\") = \"" << lps << "\"";
		std::cout << ", want \"" << want << "\".\n";
	}

	return 0;
}
