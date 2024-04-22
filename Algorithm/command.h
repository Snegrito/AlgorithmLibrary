#ifndef COMMANDS_H
#define COMMANDS_H
#include"input.h"

double areaVec(const std::vector<Point>& pol);
double area(const Polygon& pol);
double areaVertexes(const std::vector<Polygon>& pol, int count);
double areaOdd(const std::vector<Polygon>& pol);
double areaEven(const std::vector<Polygon>& pol);
double areaMean(const std::vector<Polygon>& pol);
double areaMaxMin(const std::vector<Polygon>& pol, bool isMax = true);
int vertexesMaxMin(const std::vector<Polygon>& pol, bool isMax = true);
int countVertexes(const std::vector<Polygon>& pol, int count);
int countOdd(const std::vector<Polygon>& pol);
int countEven(const std::vector<Polygon>& pol);
int lessArea(const std::vector<Polygon>& vec, const Polygon& pol);
int maxSeq(const std::vector<Polygon>& vec, const Polygon& pol);

struct CommandName {
	std::string& name;
};

struct CommandNumber {
	int& number;
};

struct Command {
	std::vector<Polygon>& data;
	std::ostream& stream;
};

std::istream& operator>>(std::istream& in, CommandName&& com);
std::istream& operator>>(std::istream& in, CommandNumber&& com);
std::istream& operator>>(std::istream& in, Command&& com);

#endif