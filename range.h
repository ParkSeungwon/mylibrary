#pragma once
#include<iostream>
#include<set>
#include<algorithm>
class Ranges;
class Range
{//[a ~ b]의 범위를 나타내는 클래스
public:
	Range(int a, int b);
	//position relation with other range
	enum POSITION {None, Left, Right, Contain, Identical};

	POSITION overlap_with(const Range& r) const;
	bool operator<(const Range& r) const;
	friend Ranges;

	int start() const;  	void start(int s);
	int end() const;		void end(int e);

private:
	int start_;
	int end_;
};

std::ostream& operator<<(std::ostream& o, const Range& r);

class Ranges
{
public:
	Ranges& operator-=(const Range& r);
	Ranges& operator+=(const Range& r);
	Ranges& operator-=(const Ranges& r);
	Ranges& operator+=(const Ranges& r);
	bool contain(const Ranges& r);//r이 부분집합임.
	bool contain(const Range& r);//r이 부분집합
	friend std::ostream& operator<<(std::ostream& o, const Ranges& r);
	std::set<Range>::iterator begin();
	std::set<Range>::iterator end();

private:
	std::set<Range> rg;
};

std::ostream& operator<<(std::ostream& o, const Ranges& r);
int cal_min(int, int);//요일과 시간을 int로 변환 
