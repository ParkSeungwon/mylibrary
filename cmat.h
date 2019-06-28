#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<sstream>
#include<iomanip>
#include<cassert>
#include<sstream>
#include<string>
#include<cmath>
#include<type_traits>
#include<cxcore.hpp>
#include<map>
//one file version
constexpr int ct_stoi(const char* p, int value = 0) {
	return *p == ' ' ? ct_stoi(p+1, (*p-'0') + value*10) : value;
}
constexpr int ct_stoi2(const char* p) {
	return *p == ' ' ? ct_stoi(p+1) : ct_stoi2(p+1);
}

template<class T, unsigned W, unsigned H> struct Cmat;
template<class T, unsigned W, unsigned H> struct CmatBase
{//compile time matrix base class
	std::array<std::array<T, H>, W> arr_;//if fundamental type no init
	CmatBase() = default;
	CmatBase(std::initializer_list<T> li) {
		//assert(li.size() == W * H);
		auto it = li.begin();
		for(int j=0; j<H; j++) for(int i=0; i<W; i++) (*this)[i][j] = *it++;
	}
	CmatBase(cv::Mat r) {
		assert(W == r.cols && H == r.rows);
		assert((std::is_same<T, std::array<unsigned, 3>>::value));
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) for(int k=0; k<3; k++)
			(*this)[i][j][k] = r.at<cv::Vec3b>(j, i)[2 - k];
	}
	int width() const { return width_; }
	int height() const { return height_; }
	operator cv::Mat() {
		cv::Mat r{H, W, CV_8UC3};
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) for(int k=0; k<3; k++)
			r.at<cv::Vec3b>(j, i)[2 - k] = (*this)[i][j][k];
		return r;
	}

	CmatBase& O() {
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) arr_[i][j] = 0;
		return *this;
	}

	const std::array<T, H>& operator[](unsigned n) const { return arr_[n]; } 
	std::array<T, H>& operator[](unsigned n) { return arr_[n]; }//need arr_ public
	template<unsigned R> Cmat<T,R,H> operator*(const CmatBase<T,R,W>& r) const {
		Cmat<T, R, H> m;
		m.O();
		for(int i=0; i<R; i++) for(int j=0; j<H; j++) 
			for(int k=0; k<W; k++) m[i][j] += (*this)[k][j] * r[i][k];
		return m;
	}
	Cmat<T, W, H> operator+(const CmatBase<T, W, H>& r) const {
		Cmat<T, W, H> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			m[i][j] = (*this)[i][j] + r[i][j];
		return m;
	}
	Cmat<T, W, H> operator-(const CmatBase<T, W, H>& r) const {
		Cmat<T, W, H> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			m[i][j] = (*this)[i][j] - r[i][j];
		return m;
	}
	CmatBase<T, W, H>& operator+=(const CmatBase<T, W, H>& r) {
		return *this = *this + r;
	}
	CmatBase<T, W, H>& operator-=(const CmatBase<T, W, H>& r) {
		return *this = *this - r;
	}
	bool operator==(const CmatBase<T, W, H>& r) const {
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) 
			if((*this)[i][j] != r[i][j]) return false;
		return true;
	}
	bool operator!=(const CmatBase<T, W, H>& r) const{
		return !(*this == r);
	}
	Cmat<T, H, W> transpose() const {
		Cmat<T, H, W> m;
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[j][i] = (*this)[i][j];
		return m;
	}
	Cmat<T, W, H> operator*(T mul) const {
		Cmat<T, W, H> m{*this};
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[i][j] = m[i][j] * mul;
		return m;
	}
	CmatBase<T, W, H>& operator*=(T mul) {
		return *this = *this * mul;
	}
	Cmat<T, W, H> operator/(T div) const {
		Cmat<T, W, H> m{*this};
		for(int i=0; i<W; i++) for(int j=0; j<H; j++) m[i][j] = m[i][j] / div;
		return m;
	}
	CmatBase<T, W, H>& operator/=(T div) {
		return *this = *this / div;
	}
	friend Cmat<T, W, H> operator*(T mul, const CmatBase<T, W, H>& r) {
		return r * mul;
	}
private:
	int width_ = W;
	int height_ = H;
};

template<class T, unsigned W, unsigned H> struct Cmat : public CmatBase<T,W,H>
{//Cmat template, made base class to avoid repeatition in specialized class definition
	Cmat() = default;
	Cmat(std::initializer_list<T> li) : CmatBase<T,W,H>{li} {}
	Cmat(CmatBase<T, W, H> r) { this->arr_ = r.arr_; }
	Cmat(const Cmat<T, W, H>& r) = default;
};

template<const char* X> struct CmatXpm
		: public CmatBase<std::array<unsigned char, 4>, ct_stoi(X), ct_stoi2(X)> 
{
	static std::array<unsigned char, 4> change(const char* p)
	{///change color code 7B543E -> uchar 232,12,122
		unsigned char c[6];
		if(*p == '#') {
			for(int i=0; i<6; i++) {
				if(p[i+1] <= '9') c[i] = p[i+1] - '0';
				else c[i] = p[i+1] - 'A' + 10;
			}
			for(int i=0; i<6; i+=2) c[i] = c[i] * 16 + c[i+1];
			return {c[0], c[2], c[4], 0xff};
		} else return {0,0,0,0};
	}

	CmatXpm(const char** xpm) {
		std::map<std::string, std::array<unsigned char, 4>> color_map;
		std::stringstream ss;
		ss << xpm[0];
		int ar[4];
		for(int i=0; i<4; i++) ss >> ar[4];
		for(int i=0; i<ar[2]; i++) 
			color_map[std::string(xpm[i+1], ar[3])] = change(xpm[i+1] + ar[3] + 3);
		for(int y=0; y<ar[1]; y++) for(int x=0; x<ar[0]; x++) for(int k=0; k<4; k++)
			(*this)[x][y][k] = color_map[std::string(xpm[ar[3]+1+y] + x*ar[3], ar[3])][k];
	}
};

template <class T, unsigned W, unsigned H>
std::ostream& operator<<(std::ostream& o, const CmatBase<T, W, H>& r)
{//stream output
	int gap[W] {0,};
	for(int y=0; y<H; y++) for(int x=0; x<W; x++) {
		std::stringstream ss;
		ss << r[x][y];
		int sz = ss.str().length();
		if(gap[x] < sz) gap[x] = sz;
	}

	o << "\u23a1" << ' ';
	for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][0] << ' ';
	o << "\u23a4" << std::endl;
	for(int y=1; y<H-1; y++) {
		o << "\u23a2" << ' ';
		for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][y] << ' ';
		o << "\u23a5" << std::endl;
	}
	if(H > 1) {
		o << "\u23a3" << ' ';
		for(int x=0; x<W; x++) o << std::setw(gap[x]) << r[x][H-1] << ' ';
		o << "\u23a6" << std::endl;
	}
	return o;
}


//vector
template<class T, unsigned N> struct Cmat<T, 1, N> : public CmatBase<T, 1, N>
{//vector matrix specialization
	Cmat() = default;
	Cmat(std::initializer_list<T> li) : CmatBase<T, 1, N>{li} {}
	Cmat(CmatBase<T, 1, N> r) { this->arr_ = r.arr_; }
	T operator,(const Cmat<T, 1, N>& r) const{
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * r[0][i];
		return sum;
	}
	Cmat<T, 1, N+1> add_tail() const {//x,y,z -> x,y,z,1
		Cmat<T, 1, N+1> m;
		for(int i=0; i<N; i++) m[0][i] = (*this)[0][i];
		m[0][N] = 1;
		return m;
	}
	Cmat<T, 1, N-1> remove_tail() const {
		Cmat<T, 1, N-1> m;
		for(int i=0; i<N-1; i++) m[0][i] = (*this)[0][i];
		return m;
	}
	T abs() const {
		T sum = 0;
		for(int i=0; i<N; i++) sum += (*this)[0][i] * (*this)[0][i];
		return sqrt(sum);
	}
	Cmat<T, 1, N> normalize() {
		*this /= abs();//why return *this /= abs() not working??
		return *this;
	}
	T angle(const Cmat<T, 1, N>& r) const {
		return acos(operator,(r) / (abs() * r.abs()));
	}
	Cmat<T, 1, 3> operator^(const Cmat<T, 1, 3>& r) {//cross product not normalized
		static_assert(N == 3, "is vec3?");
		return {(*this)[0][1] * r[0][2] - (*this)[0][2] * r[0][1],
				(*this)[0][2] * r[0][0] - (*this)[0][0] * r[0][2],
				(*this)[0][0] * r[0][1] - (*this)[0][1] * r[0][0]};
	}
};


//square
template<unsigned N> struct CmatSquare : public CmatBase<float, N, N>
{//middle base template class, not specialization, I() not possible <- recursion
	CmatSquare(std::initializer_list<float> li) : CmatBase<float,N,N>{li} {}
	CmatSquare() = default;
	CmatSquare<N>& E() {
		for(int i=0; i<N; i++) for(int j=0; j<N; j++) 
			if(i == j) (*this)[i][j] = 1;
			else (*this)[i][j] = 0;
		return *this;
	}
	CmatSquare<N> I() const {
		float ad_bc = det();
		if(!ad_bc) throw "no inverse";
		CmatSquare<N> m;
		for(int i=0; i<N; i++) for(int j=0; j<N; j++)
			m[j][i] = ((i+j) % 2 ? -1 : 1) * M(i, j).det() / ad_bc;//transpose!!
		return m;
	}
	CmatSquare<N-1> M(int x, int y) const
	{//v = linearized nXn size matrix, return a matrix except row x, col y 
		float ar[N * N];
		for(int j=0,k=0; j<N; j++) for(int i=0; i<N; i++) 
			if(x != i && y != j) ar[k++] = (*this)[i][j];
		CmatSquare<N-1> m;
		for(int j=0,k=0; j<N-1; j++) for(int i=0; i<N-1; i++) m[i][j] = ar[k++];
		return m;
	}
	float det() const//private static == f() const
	{//ad - bc
		float sum = 0;
		for(int i=0,j=1; i<N; i++, j*=-1) sum += j * (*this)[i][0] * M(i, 0).det();
		return sum;
	}
};

//template member function specialization, static inline to avoid multiple definition
//inline changes linkage behavior of compiler
template<> inline float CmatSquare<1>::det() const { return (*this)[0][0]; }

/* this part is not needed any more due to upper line
template<> struct CmatSquare<1> : public CmatBase<float, 1, 1>
{//specialization for recursive det() terminator
	CmatSquare() = default;
	CmatSquare(std::initializer_list<float> li) : CmatBase<float, 1, 1>{li} {}
	float det() const { return (*this)[0][0]; }
};*/	

//template specializations
template<unsigned N> struct Cmat<float, N, N> : public CmatSquare<N>
{//partial square specialization for same interface
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<N>{li} {}
};

//4X4
template<> struct Cmat<float, 4, 4> : public CmatSquare<4>
{//specialization for opengl
	Cmat() = default;
	Cmat(std::initializer_list<float> li) : CmatSquare<4>{li} {}
	Cmat(CmatBase<float, 4, 4> r) { this->arr_ = r.arr_; }
	Cmat<float, 4, 4> gltranslate(float x, float y, float z) {
		E();
		(*this)[3][0] = x;
		(*this)[3][1] = y;
		(*this)[3][2] = z;
		return *this;
	}
	Cmat<float, 4, 4> glrotateZ(float th) {
		E();
		(*this)[0][0] = cos(th);
		(*this)[1][0] = -sin(th);
		(*this)[0][1] = sin(th);
		(*this)[1][1] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glrotateX(float th) {
		E();
		(*this)[1][1] = cos(th);
		(*this)[2][1] = -sin(th);
		(*this)[1][2] = sin(th);
		(*this)[2][2] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glrotateY(float th) {
		E();
		(*this)[0][0] = cos(th);
		(*this)[2][0] = -sin(th);
		(*this)[0][2] = sin(th);
		(*this)[2][2] = cos(th);
		return *this;
	}
	Cmat<float, 4, 4> glscale(float x, float y, float z) {
		E();
		(*this)[0][0] = x;
		(*this)[1][1] = y;
		(*this)[2][2] = z;
		return *this;
	}
	Cmat<float, 4, 4> glortho(float left, float right,
			float bottom, float top, float far, float near) {
		E();
		(*this)[0][0] = 2 / (right - left);
		(*this)[1][1] = 2 / (top - bottom);
		(*this)[2][2] = -2 / (far - near);
		(*this)[3][0] = -(left + right) / (right - left);
		(*this)[3][1] = -(top + bottom) / (top - bottom);
		(*this)[3][2] = -(far + near) / (far - near);
		return *this;
	}
	Cmat<float, 4, 4> glprojection(float left, float right, 
			float bottom, float top, float far, float near) {
		E();
		(*this)[0][0] = 2 * near / (right - left);
		(*this)[1][1] = 2 * near / (top - bottom);
		(*this)[2][2] = -(far + near) / (far - near);
		(*this)[2][0] = (left + right) / (right - left);
		(*this)[2][1] = (top + bottom) / (top - bottom);
		(*this)[3][2] = -2 * far * near / (far - near);
		(*this)[2][3] = -1;
		(*this)[3][3] = 0;
		return *this;
	}
	void gltransfer(unsigned shader_program, const char* var_name) const{
		int fd = glGetUniformLocation(shader_program, var_name);
		if(fd != -1) glUniformMatrix4fv(fd, 1, GL_FALSE, arr_.data()->data());
	}
//	Cmat<float, 4, 4> glrotate(Cmat<float, 1, 3> to, Cmat<float, 1, 3> from={0,0,1}) {
//		Cmat<float, 1, 4> st_norm = {
};
