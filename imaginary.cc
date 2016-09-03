#include "imaginary.h"
using namespace std;

Imag::Imag(Root r, Root i)
{
	real = r;
	imag = i;
}

Imag::Imag(int i)
{
	real = i;
	imag = 0;
}

Imag operator+(const Imag& l, const Imag& r)
{
	Imag i;
	i.real = l.real + r.real;
	i.imag = l.imag + r.imag;
	return i;
}

Imag operator-(const Imag& l, const Imag& im)
{
	Imag i;
	i.real = l.real - im.real;
	i.imag = l.imag - im.imag;
	return i;
}

Imag operator*(const Imag& l, const Imag& im)
{
	Imag i;
	i.real = l.real * im.real - l.imag * im.imag;
	i.imag = l.real * im.imag + l.imag * im.real;
	return i;
}

Imag operator/(const Imag& l, const Imag& im2)
{
	Imag i, im = im2; 
	im.imag = Root(0) - im.imag;
	i = l * im;
	i.real = i.real / (im.real * im.real + im.imag * im.imag);
	i.imag = i.imag / (im.real * im.real + im.imag * im.imag);
	return i;
}

ostream& operator<<(ostream& o, const Imag& im)
{
	o << '(' << im.real << ")+(" << im.imag << ')' << 'i';
	return o;
}
