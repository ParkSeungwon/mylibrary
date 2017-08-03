#pragma once
#include<iostream>
#include<set>

struct Range
{//[a ~ b)의 범위를 나타내는 클래스
	Range(int a=0, int b=0);
	bool contain(const Range& r) const;
	Range& operator-=(const Range& r);
	Range& operator+=(const Range& r);
	std::set<int> elems_;
};

std::ostream& operator<<(std::ostream& o, const Range& r);
int cal_min(int, int);//요일과 시간을 int로 변환 
