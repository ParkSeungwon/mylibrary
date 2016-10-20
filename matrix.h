#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<exception>

class MatrixException : public std::exception
{
public:
	MatrixException(std::string s) : err_msg(s) {}
	virtual const char* what() const throw() { return err_msg.c_str(); }

protected:
	std::string err_msg;
};

template <typename T>
class Matrix
{
public:
	Matrix(int w, int h) {
		width = w;
		height = h;
		arr = new T[width * height];
		for(int i=0; i<width*height; i++) arr[i] = 0;
	}

	Matrix(std::vector<std::vector<T>> v) {
		height = v.size();
		width = v[0].size();
		arr = new T[width * height];
		for(int y=0, i=0; y<height; y++) {
			for(int x=0; x<width; x++) {
				arr[i++] = v[y][x];
			}
		}
	}

	template <typename T2> Matrix(Matrix<T2>& r);
	virtual ~Matrix() {
		delete [] arr;
	}

	Matrix<T> operator+(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) throw MatrixException("w, h does not match");
		Matrix<T> m(width, height);
		for(int i=0; i<width*height; i++) m.arr[i] = arr[i] + r.arr[i];
		return m;
	}

	Matrix<T> operator-(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) throw MatrixException("w, h does not match");
		Matrix<T> m(width, height);
		for(int i=0; i<width*height; i++) m.arr[i] = arr[i] - r.arr[i];
		return m;
	}

	Matrix<T> operator*(const Matrix<T>& r) const {
		if(width != r.height) throw MatrixException("w, h does not match");
		Matrix<T> m(r.width, height);
		for(int y=0; y<height; y++) {
			for(int x=0; x<r.width; x++) {
				for(int i=0; i<width; i++) m(x,y) = m(x,y) + (*this)(i,y) * r(x,i);
			}
		}
		return m;
	}

	Matrix<T>& operator=(const Matrix<T>& r) {
		if(width != r.width || height != r.height) delete [] arr;
		width = r.width;
		height = r.height;
		arr = new T[width * height];
		for(int i=0; i<width*height; i++) arr[i] = r.arr[i];
		return *this;
	}

	Matrix<T> operator*(const T& r) const {return r * *this;}
	friend Matrix<T> operator*(const T l, const Matrix<T>& r) {
		Matrix<T> m(r.width, r.height);
		for(int y=0; y<r.height; y++) {
			for(int x=0; x<r.width; x++) {
				m.arr[y*r.width+x] = l * r.arr[y*r.width+x];
			}
		}
		return m;
	}

	bool operator==(const Matrix<T>& r) const {
		if(width != r.width || height != r.height) return false;
		for(int i=0; i<width*height; i++) if(arr[i] != r.arr[i]) return false;
		return true;
	}

	T& operator()(int x, int y) const { return arr[y*width+x]; }

	Matrix<T> inverse() const {
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

	Matrix<T> E() const {
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

	Matrix<T> One() const {
		Matrix<T> m(width, height);
		for(int i=0; i<width*height; i++) m.arr[i] = 1;
		return m;
	}

	Matrix<T> surround(T wall = 0) const {
		Matrix<T> m(width+2, height+2);
		for(int i=0; i<(width+2)*(height+2); i++) m.arr[i] = wall;
		for(int y=0; y<height; y++) {
			for(int x=0; x<width; x++) {
				m.arr[(y+1)*(width+2)+x+1] = arr[y*width+x];
			}
		}
		return m;
	}

	void show() {
		for(int y=0, i=0; y<height; y++) {
			for(int x=0; x<width; x++) {
				std::cout << arr[i++] << ' ';
			}
			std::cout << std::endl;
		}
	}

	void random(std::vector<T> v) {
		std::random_device rd;
		std::uniform_int_distribution<> di(0, v.size()-1);
		for(int i=0; i<width*height; i++) arr[i] = v[di(rd)];
	}

protected:
	T* arr;
	int width, height;
	T* ptr() {return arr;}

private:
	void alloc();
	void release();
};
