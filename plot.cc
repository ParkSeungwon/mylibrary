#include<iostream>
#include<fcntl.h>//O_CRATE
#include<unistd.h>//ftruccate
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<cmath>
#include<cassert>
#include<thread>
#include"autothread.h"
#include"plot.h"
#include"util.h"
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
	
	string command = R"(python -c "
import sys, posix_ipc, os, struct
import matplotlib.pyplot as plt

sz = int(sys.argv[1])
f = posix_ipc.SharedMemory('plot1')
x = [0] * sz
y = [0] * sz
for i in range(sz):
    x[i], y[i] = struct.unpack('ff', os.read(f.fd, 8))
os.close(f.fd)
plt.plot(x, y)
plt.show()
" )";
	string s = psstm(command + to_string(sz));
	shm_unlink(name);
	return s;
}

string plot(const valarray<float>& x, const valarray<complex<float>>& y)
{
	valarray<float> z(y.size());
	for(int i=0; i<z.size(); i++) z[i] = y[i].real();
	return plot(x, z);
}

complex<float> DFT1(const valarray<float>& x, float w)
{//discrete fourier transform
	complex<float> im = 0;
	for(float i=0; i<x.size(); i++) im += x[i] * exp(-1if * w * i);//sampling rate
	return im;
}

valarray<complex<float>> DFT(const valarray<float>& x, const valarray<float>& w)
{
	AutoThread at;
	vector<future<complex<float>>> vf;
	valarray<complex<float>> v(w.size());
	for(int i=0; i<v.size(); i++) vf.push_back(at.add_thread(bind(DFT1, x, w[i])));
	for(int i=0; i<v.size(); i++) v[i] = vf[i].get();
//	for(int i=0; i<v.size(); i++) v[i] = DFT(x, w[i]);
	return v;
}

complex<float> IDFT(const std::valarray<std::complex<float>>& Xw, int n)
{//inverse DFT
	float dw = 2 * M_PI / Xw.size();
	complex<float> im = 0;
	float i = 0;
	for(float w=-M_PI; w<M_PI; w+=dw) im += Xw[i++] * exp(1if * w * (float)n) * dw;
	return im / 2.f / (float)M_PI;
}
