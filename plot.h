#pragma once
#include<valarray>
#include<string>
#include<complex>

std::valarray<float> linspace(float start, float stop, int size);
std::valarray<float> arange(float start, float step, float stop);
std::string plot(const std::valarray<float>& x, const std::valarray<float>& y);
std::valarray<std::complex<float>> DFT(const std::valarray<float>& x, const std::valarray<float>& w);
std::string plot(const std::valarray<float>& x, const std::valarray<std::complex<float>>& y);
std::complex<float> IDFT(const std::valarray<std::complex<float>>& Xw, int n);

