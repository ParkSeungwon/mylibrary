#include <iostream>
#include <cmath>
#include "irrational.h"
using namespace std;

Root::Root(unsigned i, unsigned e) 
{
	if(e == 2 && i != 0) {
		auto v = Ratio::prime_div(i);
		int fr = 1;
		for(auto it = v.begin(); it+1 != v.end() && it != v.end();) {
			if(*it == *(it+1)) {
				fr *= *it;
				it = v.erase(it, it+2);
			} else it++;
		}
		int sum = 1;
		for(auto& a : v) sum *= a;
		ir.insert({sum, Ratio(fr)});
	} else ir.insert({1, i});
	clear_garbage();
}

Root::Root(Ratio r, unsigned e)
{
	if(e == 2) {
		*this = Root(r.n * r.den, 2);
		auto it = ir.begin();
		it->second = it->second * Ratio(1, r.den);
	} else ir.insert({1, r});
	clear_garbage();
}

Root operator+(const Root& l, const Root& r)
{
	Root t(l);
	for(auto& a : r.ir) {
		if(t.ir.find(a.first) == t.ir.end()) t.ir[a.first] = a.second;
		else t.ir[a.first] = t.ir[a.first] + a.second;
	}
	t.clear_garbage();
	return t;
}

Root operator-(const Root& l, const Root& r)
{
	Root t(l);
	for(auto& a : r.ir) {
		if(t.ir.find(a.first) == t.ir.end()) t.ir[a.first] = Ratio(0) - a.second;
		else t.ir[a.first] = t.ir[a.first] - a.second;
	}
	t.clear_garbage();
	return t;
}

Root operator*(const Root& l, const Root& r)
{
	Root tmp, ret;
	Ratio r2;
	map<unsigned, Ratio>::iterator it;
	for(auto& a : l.ir) {
		for(auto& b : r.ir) {
			r2 = a.second * b.second;
			tmp = Root(a.first * b.first, 2);
			it = tmp.ir.begin();//루트 앞의 유리수
			it->second = it->second * r2;
			ret = ret + tmp;
		}
	}
	ret.clear_garbage();
	return ret;
}

Root operator/(const Root& l, const Root& r)
{
	Root t(r);
	Root tm(r);//up*tm/t*tm
	Root up(1);
	map<unsigned, Ratio>::reverse_iterator it;
	while(!tm.isRational()) {
		it = tm.ir.rbegin();
		it->second = Ratio(0) - it->second;
		up = up * tm;
		t = tm * t;
		tm = t;
	}
	tm = up * l;
	for(auto& a : tm.ir) a.second = a.second / t.ir[1];
	tm.clear_garbage();
	return tm;
}

bool Root::isRational()
{
	if(ir.size() > 1)  return false;
	else if(ir.size() == 0) return true;
	else if(ir.find(1) != ir.end()) return true;
	else return false;
}

bool Root::operator==(const Root& r)
{
	if(ir.size() == r.ir.size()) {
		for(auto& a : r.ir) if(ir[a.first] != a.second) return false;
		return true;
	} else return false;
}

bool Root::operator<(const Root& r)
{
	if(*this == r) return false;
	else {
		Root t = *this - r;
		return t.approx() < 0;
	}
}

float Root::approx()
{
	float f = 0.0, n, d;
	for(auto& a : ir) {
		n = a.second.n;
		d = a.second.den;
		f += sqrtf(a.first) * n / d;
	}
	return f;
}
	
ostream& operator<<(ostream& o, const Root& rhs)
{
	int i = 0;
	for(auto& a : rhs.ir) {
		if((Ratio)a.second > 0 && i != 0) o << '+';
		i++;
		if(a.first == 1) o << a.second;
		else if((Ratio)a.second == 1) o << "_/" << a.first;//first, second는 자신이 무슨 형식인지 알지못함.
		else if((Ratio)a.second == -1) o << "-_/" << a.first;
		else o << a.second << "_/" << a.first;
	}
	if(i == 0) o << '0';
	return o;
}
	
