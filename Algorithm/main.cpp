#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <numeric>
#include "input.h"
#include "command.h"

int main() {
	std::cout << "Enter file name with polygons: ";
	std::string fileName;
	std::cin >> fileName;
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		std::cout << "Error: invalid filename";
		return -1;
	}

	std::vector <Polygon > polygons;
	while (!fin.eof()) {
		std::copy(
			std::istream_iterator<Polygon>(fin),
			std::istream_iterator<Polygon>(),
			std::back_inserter(polygons)
		);
		if (!fin) {
			fin.clear();
			fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	fin.close();

	std::vector<Polygon> test = polygons;
	std::cout << "Data before:\n";
	std::copy(
		polygons.begin(),
		polygons.end(),
		std::ostream_iterator<Polygon>(std::cout, "\n")
	);

	std::cout << "Enter file name with commands: ";
	std::cin >> fileName;
	std::ifstream fileCommandIn(fileName);
	if (!fileCommandIn) {
		std::cout << "Error: invalid filename";
		return -1;
	}

	while (!fileCommandIn.eof()) {
		fileCommandIn >> Command{ polygons, std::cout };
		if (!fileCommandIn) {
			fileCommandIn.clear();
			fileCommandIn.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		std::cout << '\n';
	}

	std::cout << "Data after:\n";
	std::copy(
		polygons.begin(),
		polygons.end(),
		std::ostream_iterator<Polygon>(std::cout, "\n")
	);
	std::cout << "End programm";

	return 0;
}