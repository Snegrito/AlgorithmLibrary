#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <iostream>

struct Point {
	int x, y;
};

bool operator==(const Point& first, const Point& second);

struct Polygon {
	std::vector <Point> points;
};

bool operator==(const Polygon& first, const Polygon& second);
bool operator!=(const Polygon& first, const Polygon& second);

struct Delimiter {
	char exp;
};

struct Quantity {
	int& size;
};

struct Coordinate {
	int& ref;
};

class iofmtguard {
public:
	iofmtguard(std::basic_ios<char>& s);
	~iofmtguard();
private:
	std::basic_ios<char>& s_;
	char fill_;
	std::streamsize precision_;
	std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, Delimiter&& delim);
std::istream& operator>>(std::istream& in, Quantity&& quantity);
std::istream& operator>>(std::istream& in, Coordinate&& coordinate);
std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& pol);
std::ostream& operator<<(std::ostream& out, const Polygon& pol);
std::ostream& operator<<(std::ostream& out, const Point& point);

void checkEndOfString(std::istream& in);

#endif
