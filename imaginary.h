#pragma once
#include <ostream>
#include "irrational.h"

class Imag
{
public:
	Root real, imag;

	Imag(Root r = 0, Root i = 0);
	Imag(int i);
	bool operator==(const Imag& im) {return real == im.real && imag == im.imag;}
	Root abs() {return real*real + imag*imag;}

protected:
};

std::ostream& operator<<(std::ostream& o, const Imag& i);
Imag operator+(const Imag& l, const Imag& r);
Imag operator-(const Imag& l, const Imag& r);
Imag operator*(const Imag& l, const Imag& r);
Imag operator/(const Imag& l, const Imag& r);
