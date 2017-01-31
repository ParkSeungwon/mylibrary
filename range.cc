#include"range.h"
using namespace std;

Range::Range(int a, int b)
{
	start_ = a < b ? a : b; 
	end_ = a < b ? b : a;
}

Range::POSITION Range::overlap_with(const Range& r) const{
	if(start_ <= r.start_ && end_ >= r.end_) return Contain;
	if(end_ <= r.start_ || start_ >= r.end_) return None;
	if(start_ == r.start_ && end_ == r.end_) return Identical;
	if(r.start_ <= start_) return Left;
	if(end_ <= r.end_) return Right;
}

bool Range::operator<(const Range& r) const{
	return start_ < r.start_;
}

int Range::start() const {return start_;} 		
void Range::start(int s) {start_ = s;}
int Range::end() const {return end_;}
void Range::end(int e) {end_ = e;}

std::ostream& operator<<(std::ostream& o, const Range& r) {
	o << '[' << r.start() << ',' << r.end() << ']';
	return o;
}


Ranges& Ranges::operator-=(const Range& r) {
	for(auto a : rg) {//do not use & erase will deallocate the memory
		auto pos = a.overlap_with(r);
		if(pos) rg.erase(a);
		switch(pos) {
			case Range::Contain: 
				if(a.start_ != r.start_) rg.insert(Range(a.start_, r.start_));
				if(a.end_ != r.end_) rg.insert(Range(r.end_, a.end_)); 
				break;
			case Range::Left: 
				rg.insert(Range(r.end_ , a.end_));
				break;
			case Range::Right: 
				rg.insert(Range(r.start_, a.start_)); 
				break;
			default:;
		}
	}
	return *this;
}

Ranges& Ranges::operator+=(const Range& r) {
	bool overlap = false;
	for(auto& a : rg) {
		if(auto pos = a.overlap_with(r)) {
			if(pos == Range::Contain) return *this;
			else {
				rg.erase(a);
				return this->operator+=(Range(std::min(a.start_, r.start_), 
							std::max(a.end_, r.end_)));
			}
			overlap = true;
		}
	}
	if(!overlap) {
		int s = r.start_, e = r.end_;
		for(auto a : rg) {
			if(a.start_ == e) {
				rg.erase(a);
				e = a.end_;
			} else if(a.end_ == s) {
				rg.erase(a);
				s = a.start_;
			}
		}
		rg.insert({s, e});
	}
	return *this;
}

bool Ranges::contain(const Range& r) {
	for(auto& a : rg) if(a.overlap_with(r) == Range::Contain) return true;
	return false;
}

bool Ranges::contain(const Ranges& r) {
	for(auto& a : r.rg) if(!contain(a)) return false;
	return true;
}

Ranges& Ranges::operator-=(const Ranges& r) {
	for(auto& a : r.rg) *this -= a;
	return *this;
}

Ranges& Ranges::operator+=(const Ranges& r) {
	for(auto& a : r.rg) *this += a;
	return *this;
}
set<Range>::iterator Ranges::begin() { return rg.begin(); }
set<Range>::iterator Ranges::end() { return rg.end(); }

std::ostream& operator<<(std::ostream& o, const Ranges& r) {
	for(auto& a : r.rg) o << a << ' ';
	o << std::endl;
	return o;
}

int cal_min(int day, int hour) {
	return (day-1) * 24 * 100 + hour;
}
