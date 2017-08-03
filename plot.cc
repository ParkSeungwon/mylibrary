#include<iostream>
#include<fcntl.h>//O_CRATE
#include<unistd.h>//ftruccate
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<cmath>
#include<cassert>
#include"plot.h"
using namespace std;

valarray<float> linspace(float start, float stop, int size)
{
	valarray<float> v(size);
	for(int i=0; i<size; i++) v[i] = start + i * (stop-start)/size;
	return v;
}

std::valarray<float> arange(float start, float step, float stop)
{
	int size = (stop - start) / step;
	valarray<float> v(size);
	for(int i=0; i<size; i++) v[i] = start + step * i;
	return v;
}

string psstm(string command)
{//return system call output as string
	string s;
	char tmp[1000];
	FILE* f = popen(command.c_str(), "r");
	while(fgets(tmp, sizeof(tmp), f)) s += tmp;
	pclose(f);
	return s;
}

string plot(const valarray<float>& x, const valarray<float>& y)
{
	int sz = x.size();
	assert(sz == y.size());
	int bytes = sz * sizeof(float) * 2;
	const char* name = "plot1";
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, bytes);
	float* ptr = (float*)mmap(0, bytes, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	for(int i=0; i<sz; i++) {
		*ptr++ = x[i];
		*ptr++ = y[i];
	}
	
	string command = "plot.py ";
	string s = psstm(command + to_string(sz));
	shm_unlink(name);
	return s;
}

complex<float> DFT(const valarray<float>& x, float w)
{
	complex<float> im = 0;
	for(float i=0; i<x.size(); i++) im += x[i] * exp(-1if * w * i);//sampling rate
	return im;
}

complex<float> IDFT(const std::valarray<std::complex<float>>& Xw, int n)
{
	float dw = 2 * M_PI / Xw.size();
	complex<float> im = 0;
	float i = 0;
	for(float w=-M_PI; w<M_PI; w+=dw) im += Xw[i++] * exp(1if * w * (float)n) * dw;
	return im / 2.f / (float)M_PI;
}
