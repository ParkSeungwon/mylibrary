#include "rational.h"
using namespace std;

Ratio::Ratio(int n, int d)
{
	bool plus = true;
	if(n<0) {
		plus = !plus;
		n = -n;
	}
	if(d<0) {
		plus = !plus;
		d = -d;
	}
	if(n == 0 || d == 0) {
		this->n = 0; 
		this->den = 1;
		return;
	}
	auto np = Ratio::prime_div(n);
	auto dp = Ratio::prime_div(d);
	auto nit = np.begin();
	auto dit = dp.begin(); 
	while(nit != np.end() && dit != dp.end()) {
		if(*nit > *dit) dit++;
		else if(*nit < *dit) nit++;
		else {
			dp.erase(dit); 
			np.erase(nit);
		}
	}
	
	int sum = 1;
	for(auto& a : np) sum *= a; 
	if(plus) this->n = sum;
	else this->n = -sum;
	sum = 1;
	for(auto& a : dp) sum *= a; 
	this->den = sum;
}
bool Ratio::operator<(const Ratio& r)
{
	Ratio t = *this - r;
	return t.n < 0;
}

bool Ratio::operator>(const Ratio& r)
{
	Ratio t = *this - r;
	return t.n > 0;
}

Ratio& Ratio::operator+=(const Ratio& r)
{
	 return *this = *this + r;
}

vector<unsigned int> Ratio::prime_div(int num)
{
	vector<unsigned int> prime, component;
	bool isPrime;

	for(unsigned int i=2; ;i++) {
		isPrime = true;
		for(auto& a : prime) {
			if(i % a == 0) {
				isPrime = false;
				break;
			}
		}
		if(isPrime) {
			prime.push_back(i);
			while(num % i == 0) {
				component.push_back(i);
				num /= i;
			}
		}
		if(1 == num) return component;
	}
}

ostream& operator<<(ostream& ostr, const Ratio& rhs)
{
	if(rhs.den == 1) ostr << rhs.n;
	else ostr << rhs.n << '/' << rhs.den;
	return ostr;
}

Ratio operator+(const Ratio& l, const Ratio& r)
{
	int i = l.n * r.den + r.n * l.den;
	int d = l.den * r.den;
	return Ratio(i, d);
}


Ratio operator-(const Ratio& l, const Ratio& r)
{
	int i = l.n * r.den - r.n * l.den;
	int d = l.den * r.den;
	return Ratio(i, d);
}
Ratio operator*(const Ratio& l, const Ratio& r)
{
	int i = l.n * r.n;
	int d = l.den * r.den;
	return Ratio(i, d);
}
Ratio operator/(const Ratio& l, const Ratio& r)
{
	int i = l.n * r.den;
	int d = l.den * r.n;
	return Ratio(i, d);
}
