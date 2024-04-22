#include "command.h"
#include<algorithm>
#include<functional>
#include<numeric>

using namespace std::placeholders;

double areaVec(const std::vector<Point>& pol) {
	double area = 0.5 * std::abs((pol[pol.size() - 1].x * pol[0].y) -
		(pol[pol.size() - 1].y * pol[0].x) +
		std::accumulate(pol.begin(), pol.end() - 1, 0.0,
			[](double sum, std::vector<Point>::const_reference ref) {
				return sum + (ref.x * (*(&ref + 1)).y) -
				(ref.y * (*(&ref + 1)).x);
			})
	);
	return area;
}

double area(const Polygon& pol) {
    return areaVec(pol.points);
}

double areaVertexes(const std::vector<Polygon>& pol, int count) {
    double sumArea = std::accumulate(pol.begin(), pol.end(), 0.0,
        [count](double sum, std::vector<Polygon>::const_reference ref) {
            if (ref.points.size() == count) {
                return sum + area(ref);
            }
            else {
                return sum;
            }
        }
    );
    return sumArea;
}

double areaOdd(const std::vector<Polygon>& pol) {
    double sumArea = std::accumulate(pol.begin(), pol.end(), 0.0,
        [](double sum, std::vector<Polygon>::const_reference ref) {
            if (ref.points.size() % 2 == 1) {
                return sum + area(ref);
            }
            else {
                return sum;
            }
        }
    );
    return sumArea;
}

double areaEven(const std::vector<Polygon>& pol) {
    double sumArea = std::accumulate(pol.begin(), pol.end(), 0.0,
        [](double sum, std::vector<Polygon>::const_reference ref) {
            if (ref.points.size() % 2 == 0) {
                return sum + area(ref);
            }
            else {
                return sum;
            }
        }
    );
    return sumArea;
}

double areaMean(const std::vector<Polygon>& pol) {
    int count = 0;
    double sumArea = std::accumulate(pol.begin(), pol.end(), 0.0,
        [&count](double sum, std::vector<Polygon>::const_reference ref) {
            ++count;
            return sum + area(ref);
        }
    );
    return sumArea / count;
}

double areaMaxMin(const std::vector<Polygon>& pol, bool isMax) {
	if (isMax) {
		return area(*std::max_element(pol.begin(), pol.end(), [](const Polygon& first, const Polygon& second) {
			return area(first) < area(second);
			}
		));
	}
	else {
		return area(*std::min_element(pol.begin(), pol.end(), [](const Polygon& first, const Polygon& second) {
			return area(first) < area(second);
			}
		));
	}
}

int vertexesMaxMin(const std::vector<Polygon>& pol, bool isMax) {
    if (isMax) {
        return (std::max_element(pol.begin(), pol.end(), [](const Polygon& first, const Polygon& second) {
            return first.points.size() < second.points.size();
            }
        )->points.size());
    }
    else {
        return (std::min_element(pol.begin(), pol.end(), [](const Polygon& first, const Polygon& second) {
            return first.points.size() < second.points.size();
            }
        )->points.size());
    }
}

int countVertexes(const std::vector<Polygon>& pol, int count) {
    return std::count_if(pol.begin(), pol.end(), std::bind(
        [](int first, const Polygon& p) {return first == p.points.size(); }, count, _1)
    );
}

int countOdd(const std::vector<Polygon>& pol) {
    return std::count_if(pol.begin(), pol.end(),
        [](std::vector<Polygon>::const_reference ref) {
            return ref.points.size() % 2 == 1;
        });
}

int countEven(const std::vector<Polygon>& pol) {
    return std::count_if(pol.begin(), pol.end(),
        [](std::vector<Polygon>::const_reference ref) {
            return ref.points.size() % 2 == 0;
        });
}

int lessArea(const std::vector<Polygon>& vec, const Polygon& pol) {
    return std::count_if(vec.begin(), vec.end(),
		std::bind(std::less<double>(), area(pol), std::bind(area, _1))
    );
}

int maxSeq(const std::vector<Polygon>& vec, const Polygon& pol) {
	size_t buffMax = 0;
	size_t res = std::accumulate(vec.begin(), vec.end(), 0,
		[&buffMax, &pol](size_t counter, std::vector<Polygon>::const_reference ref) {
			if (pol == ref && counter >= buffMax) {
				return ++counter;
			}
			else if (pol == ref) {
				++counter;
			}
			else {
				if (buffMax < counter) {
					buffMax = counter;
				}
				counter = 0;
				return counter;
			}
		}
	);
	return std::max(buffMax, res);
}


std::istream& operator>>(std::istream& in, CommandName&& com) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	std::string temp;
	in >> temp;
	if (in) {
		com.name = temp;
	}
	return in;
}

std::istream& operator>>(std::istream& in, CommandNumber&& com) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	int temp;
	in >> temp;
	if (in && temp >= 3) {
		com.number = temp;
	}
	else {
		in.setstate(std::ios::failbit);
	}
	return in;
}



std::istream& operator>>(std::istream& in, Command&& com) {
	std::istream::sentry sentry(in);
	if (!sentry) {
		return in;
	}
	iofmtguard guard(in);
	std::string name;
	in >> std::ws >> std::noskipws >> CommandName{ name } >> Delimiter{ ' ' };
	if (name == "AREA") {
		char next = in.peek();
		if (next <= '9' && next >= '0') {
			int count;
			in >> CommandNumber{ count };
			checkEndOfString(in);
			if (in) {
				com.stream << name << ' ' << count << '\n' << areaVertexes(com.data, count);
			}
			else {
				com.stream << "<INVALID COMMAND>";
			}
		}
		else {
			std::string name2;
			in >> CommandName{ name2 };
			checkEndOfString(in);
			if (in) {
				if (name2 == "ODD") {
					com.stream << name << ' ' << name2 << '\n' << areaOdd(com.data);
				}
				else if (name2 == "EVEN") {
					com.stream << name << ' ' << name2 << '\n' << areaEven(com.data);
				}
				else if (name2 == "MEAN" && com.data.size() != 0) {
					com.stream << name << ' ' << name2 << '\n' << areaMean(com.data);
				}
				else {
					com.stream << "<INVALID COMMAND>";
				}
			}
			else {
				com.stream << "<INVALID COMMAND>";
			}
		}
	}
	else if ((name == "MAX" || name == "MIN") && (com.data.size() != 0)) {
		std::string name2;
		in >> CommandName{ name2 };
		checkEndOfString(in);
		if (in) {
			if (name2 == "AREA") {
				com.stream << name << ' ' << name2 << '\n' << areaMaxMin(com.data, (name == "MAX") ? true : false);
			}
			else if (name2 == "VERTEXES") {
				com.stream << name << ' ' << name2 << '\n' << vertexesMaxMin(com.data, (name == "MAX") ? true : false);
			}
			else {
				com.stream << "<INVALID COMMAND>";
			}
		}
		else {
			com.stream << "<INVALID COMMAND>";
		}
	}
	else if (name == "COUNT") {
		char next = in.peek();
		if (next <= '9' && next >= '0') {
			int count;
			in >> CommandNumber{ count };
			checkEndOfString(in);
			if (in) {
				com.stream << name << ' ' << count << '\n' << countVertexes(com.data, count);
			}
			else {
				com.stream << "<INVALID COMMAND>";
			}
		}
		else {
			std::string name2;
			in >> CommandName{ name2 };
			checkEndOfString(in);
			if (in) {
				if (name2 == "ODD") {
					com.stream << name << ' ' << name2 << '\n' << countOdd(com.data);
				}
				else if (name2 == "EVEN") {
					com.stream << name << ' ' << name2 << '\n' << countEven(com.data);
				}
				else {
					com.stream << "<INVALID COMMAND>";
				}
			}
			else {
				com.stream << "<INVALID COMMAND>";
			}
		}
	}
	else if (name == "LESSAREA") {
		Polygon pol;
		in >> pol;
		if (in) {
			com.stream << name << ' ' << pol << '\n' << lessArea(com.data, pol);
		}
		else {
			com.stream << "<INVALID COMMAND>";
		}
	}
	else if (name == "MAXSEQ") {
		Polygon pol;
		in >> pol;
		if (in) {
			com.stream << name << ' ' << pol << '\n' << maxSeq(com.data, pol);
		}
		else {
			com.stream << "<INVALID COMMAND>";
		}
	}
	else {
		com.stream << "<INVALID COMMAND>";
	}
	return in;
}
