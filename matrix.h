#pragma once
#include<vector>
#include<string>
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
	Matrix(int w, int h);
	Matrix(std::vector<std::vector<T>> v);
	template <typename T2>
	Matrix(Matrix<T2>& r);
	virtual ~Matrix();
	Matrix<T> operator+(const Matrix<T>& r) const;
	Matrix<T> operator-(const Matrix<T>& r) const;
	Matrix<T> operator*(const Matrix<T>& r) const;
	Matrix<T>& operator=(const Matrix<T>& r);
	Matrix<T> operator*(const T& r) const {return r * *this;}
	bool operator==(const Matrix<T>& r) const;
	friend Matrix<T> operator*(const T l, const Matrix<T>& r) {
		Matrix<T> m(r.width, r.height);
		for(int y=0; y<r.height; y++) {
			for(int x=0; x<r.width; x++) {
				m.arr[y*r.width+x] = l * r.arr[y*r.width+x];
			}
		}
		return m;
	}
	T& operator()(int x, int y) { return arr[y*width+x]; }
	Matrix<T> inverse() const;
	Matrix<T> E() const;
	Matrix<T> One() const;
	Matrix<T> surround(T wall = 0) const;
	T* ptr() {return arr;}
	void show();
	
protected:
	T* arr;
	int width, height;

private:
	void alloc();
	void release();
};
