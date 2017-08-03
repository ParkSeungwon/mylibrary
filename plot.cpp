#include<iostream>
#include"plot.h"
using namespace std;

int main()
{
	auto x = arange(-7, 1, 8);
	auto y = valarray<float>{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0};
	plot(x, y);
	auto w = linspace(-M_PI, M_PI, 100);
	valarray<complex<float>> z(100);
	valarray<float> q(15), z1(100);
	for(int i=0; i<100; i++) z[i] = DFT(y, w[i]);
	int k=0;
	for(const auto& a : z) z1[k++] = abs(a);
	plot(w[slice{0, 25, 4}], z1[slice{0,25,4}]);
	k=0;
	for(auto i : x) q[k++] = abs(IDFT(z, i));
	plot(x,q);
}
