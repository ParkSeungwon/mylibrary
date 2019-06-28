#pragma once
#include"base.h"

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

