#include<cassert>
#include"range.h"
using namespace std;

Range::Range(int a, int b)
{
	assert(a <= b);
	assert(b - a < 1000000);
	for(int i=a; i<b; i++) elems_.insert(i);
}

Range& Range::operator-=(const Range& r) {
	for(int n : r.elems_) elems_.erase(n);
}

Range& Range::operator+=(const Range& r) {
	for(int n : r.elems_) elems_.insert(n);
}

bool Range::contain(const Range& r) const {
	for(int n : r.elems_) if(elems_.find(n) == elems_.end()) return false;
	return true;
}

std::ostream& operator<<(std::ostream& o, const Range& r) {
	auto it = r.elems_.begin();
	int prev = *it++;
	o << '[' << prev;
	for(; it != --r.elems_.end(); prev = *it++) {
		if(prev + 1 == *it) continue;
		else o << ',' << prev+1 << "),[" << *it;
	}
	o << ',' << *it+1 << ')' << endl;
	return o;
}

int cal_min(int day, int hour) {
	return (day-1) * 24 * 100 + hour;
}
