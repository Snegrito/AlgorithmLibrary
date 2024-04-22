#include "input.h"

bool operator==(const Point& first, const Point& second) {
	return (first.x == second.x && first.y == second.y);
}

bool operator==(const Polygon& first, const Polygon& second) {
	return (first.points == second.points);
}

bool operator!=(const Polygon& first, const Polygon& second) {
	return !(first == second);
}

iofmtguard::iofmtguard(std::basic_ios<char>& s) :
	s_(s),
	fill_(s.fill()),
	precision_(s.precision()),
	fmt_(s.flags()) 
{}

iofmtguard::~iofmtguard() {
	s_.fill(fill_);
	s_.precision(precision_);
	s_.flags(fmt_);
}

std::istream& operator>>(std::istream& in, Delimiter&& delim) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	char c = '\0';
	c = in.peek();
	if (in && c != delim.exp) {
		in.setstate(std::ios::failbit);
	}
	else {
		in.ignore(1);
	}
	return in;
}

std::istream& operator>>(std::istream& in, Quantity&& quantity) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	int size = 0;
	in >> size;
	if (size >= 3) {
		quantity.size = size;
	}
	else {
		in.setstate(std::ios::failbit);
	}
	return in;
}

std::istream& operator>>(std::istream& in, Coordinate&& coordinate) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	int coor = 0;
	in >> coor;
	if (in) {
		coordinate.ref = coor;
	}
	return in;
}

std::istream& operator>>(std::istream& in, Point& point) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	in >> Delimiter{ '(' };
	int x = 0;
	in >> Coordinate{ x };
	in >> Delimiter{ ';' };
	int y = 0;
	in >> Coordinate{ y };
	in >> Delimiter{ ')' };
	if (in) {
		point.x = x;
		point.y = y;
	}
	return in;
}

std::istream& operator>>(std::istream& in, Polygon& pol) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	iofmtguard guard(in);
	in >> std::ws >> std::noskipws;
	Polygon polygon;
	int numberOfPoints = 0;
	in >> Quantity{ numberOfPoints };
	if (in) {
		for (int i = 0; i < numberOfPoints; ++i) {
			in >> Delimiter{ ' ' };
			Point temp{0, 0};
			in >> temp;
			if (in) {
				polygon.points.push_back(temp);
			}
		}
		if (in) {
			checkEndOfString(in);
		}
	}
	if (in) {
		pol = polygon;
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Polygon& pol) {
	std::ostream::sentry sentry(out);
	if (!sentry) {
		return out;
	}
	out << pol.points.size() << ' ';
	std::copy(
		pol.points.begin(),
		pol.points.end(),
		std::ostream_iterator<Point>(out)
	);
	return out;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
	std::ostream::sentry sentry(out);
	if (!sentry) {
		return out;
	}
	out << '(' << point.x << ';' << point.y << ')' << ' ';
	return out;
}

void checkEndOfString(std::istream& in) {
	char next = in.peek();
	while (next == ' ') {
		in.ignore(1);
		next = in.peek();
	}
	in >> Delimiter{ '\n' };
	if (!in && in.eof()) {
		in.clear(std::ios::eofbit);
	}
}

