#include<iostream>
#include"matrix.h"
using namespace std;

template <typename T>
Matrix<T>::Matrix(int w, int h)
{
	width = w;
	height = h;
	arr = new T[width * height];
	for(int i=0; i<width*height; i++) arr[i] = 0;
}

template <typename T>
Matrix<T>::Matrix(vector<vector<T>> v)
{
	height = v.size();
	width = v[0].size();
	arr = new T[width * height];
	for(int y=0, i=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			arr[i++] = v[y][x];
		}
	}
}

template <typename T>
Matrix<T>::~Matrix() {
	delete [] arr;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& r) const
{
	if(width != r.width || height != r.height) return false;
	for(int i=0; i<width*height; i++) if(arr[i] != r.arr[i]) return false;
	return true;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& r) 
{
	if(width != r.width || height != r.height) delete [] arr;
	width = r.width;
	height = r.height;
	arr = new T[width * height];
	for(int i=0; i<width*height; i++) arr[i] = r.arr[i];
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& r) const
{
	if(width != r.width || height != r.height) throw MatrixException("w, h does not match");
	Matrix<T> m(width, height);
	for(int i=0; i<width*height; i++) m.arr[i] = arr[i] + r.arr[i];
	return m;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& r) const
{
	if(width != r.width || height != r.height) throw MatrixException("w, h does not match");
	Matrix<T> m(width, height);
	for(int i=0; i<width*height; i++) m.arr[i] = arr[i] - r.arr[i];
	return m;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& r) const
{
	if(width != r.height) throw MatrixException("w, h does not match");
	Matrix<T> m(r.width, height);
	for(int y=0; y<height; y++) {
		for(int x=0; x<r.width; x++) {
			for(int i=0; i<width; i++) m.arr[y*width+x] = m.arr[y*width+x] + arr[y*width+i] * r.arr[i*width+x];
		}
	}
	return m;
}

template <typename T>
void Matrix<T>::show()
{
	for(int y=0, i=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			cout << arr[i++] << ' ';
		}
		cout << endl;
	}
}
	
template <typename T>
Matrix<T> Matrix<T>::surround(T wall) const
{
	Matrix<T> m(width+2, height+2);
	for(int i=0; i<(width+2)*(height+2); i++) m.arr[i] = wall;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			m.arr[(y+1)*(width+2)+x+1] = arr[y*width+x];
		}
	}
	return m;
}

template <typename T>
Matrix<T> Matrix<T>::inverse() const
{
	if(!((width == 3 && height == 3) || (width == 2 && height == 2)))
		throw MatrixException("only 2X2 3X3 Matrix is supported!");
	Matrix<T> m(width, height);
	if(width == 2) {
		T a = arr[0];
		T b = arr[1];
		T c = arr[2];
		T d = arr[3];
		if(a*d - b*c == 0) throw MatrixException("No inverse ");
		T e = 1/(a*d-b*c);
		m.arr[0] = d*e;
		m.arr[3] = a*e;
		m.arr[2] = 0-b*e;
		m.arr[1] = 0-c*e;
	} else {
		T a = arr[0];
		T b = arr[1];
		T c = arr[2];
		T d = arr[3];
		T e = arr[4];
		T f = arr[5];
		T g = arr[6];
		T h = arr[7];
		T i = arr[8];
		T j = a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g;
		if(j == 0) throw MatrixException("No inverse ");
		j = 1 / j;
		m.arr[0] = e*i - f*h;
		m.arr[1] = c*h - b*i;
		m.arr[2] = b*f - c*e;
		m.arr[3] = f*g - d*i;
		m.arr[4] = a*i - c*g;
		m.arr[5] = c*d - a*f;
		m.arr[6] = d*h - e*g;
		m.arr[7] = b*g - a*h;
		m.arr[8] = a*e - b*d;
		for(int i=0; i<9; i++) m.arr[i] = m.arr[i]*j;
	}
	return m;
}

template <typename T>
Matrix<T> Matrix<T>::E() const
{
	if(width != height) throw MatrixException("w, h does not match");
	Matrix<T> m(width, height);
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			if(x == y) m.arr[y*width+x] = 1;
			else m.arr[y*width+x] = 0;
		}
	}
	return m;
}

template <typename T>
Matrix<T> Matrix<T>::One() const
{
	Matrix<T> m(width, height);
	for(int i=0; i<width*height; i++) m.arr[i] = 1;
	return m;
}


