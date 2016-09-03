#pragma once
#include <vector>
#include <ostream>

class Ratio
{
public:
	int n, den;
	
	Ratio(int n = 0, int den = 1);
	bool operator==(const Ratio& r) {return (n == r.n) && (den == r.den);}
	bool operator!=(const Ratio& r) {return !(*this == r);}
	Ratio& operator+=(const Ratio& r);
	bool operator>(const Ratio& r);
	bool operator<(const Ratio& r);
	bool operator>=(const Ratio& r) {return *this == r || *this > r;}
	bool operator<=(const Ratio& r) {return *this == r || *this < r;}

	static std::vector<unsigned> prime_div(int num);

protected:

private:
};

std::ostream& operator<<(std::ostream& ostr, const Ratio& rhs);
Ratio operator+(const Ratio& l, const Ratio& r);
Ratio operator-(const Ratio& l, const Ratio& r);
Ratio operator*(const Ratio& l, const Ratio& r);
Ratio operator/(const Ratio& l, const Ratio& r);
