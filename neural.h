#pragma once
#include<random>
#include<functional>
#include<fstream>
#include<algorithm>
#include"cmat.h"
#include"cmatstream.h"

template<unsigned Idx, unsigned W, unsigned H> class Layer {
protected://weight * input = output ->activation -> input
	Cmat<float, W+1, H> weight;//+1 = bias
	Cmat<float, 1, W> input;
	Cmat<float, 1, W+1> delta;
	Cmat<float, 1, H> output;//before activation
};
template<unsigned Idx, unsigned... Ns> struct Neural {};
template<unsigned Idx, unsigned N1, unsigned N2, unsigned... Ns>
struct Neural<Idx, N1, N2, Ns...> : Neural<Idx + 1, N2, Ns...>, Layer<Idx, N1, N2>
{//recursive specialization
	auto& weight() { return Layer<Idx, N1, N2>::weight; }
	auto& input() { return Layer<Idx, N1, N2>::input; }
	auto& delta() { return Layer<Idx, N1, N2>::delta; }
	auto& output() { return Layer<Idx, N1, N2>::output; }
};
template<unsigned Idx, unsigned N> struct Neural<Idx, N> {//termination
	Cmat<float, 1, N> result;
};

//tuple like getters
template<unsigned Idx, unsigned... Ns> auto& weight(Neural<Idx, Ns...>& r) {
	return r.weight();
}
template<unsigned Idx, unsigned... Ns> auto& input(Neural<Idx, Ns...>& r) {
	return r.input();
}
template<unsigned Idx, unsigned... Ns> auto& delta(Neural<Idx, Ns...>& r) {
	return r.delta();
}
template<unsigned Idx, unsigned... Ns> auto& output(Neural<Idx, Ns...>& r) {
	return r.output();
}

template<unsigned... Ns> class NeuralNet : public Neural<0, Ns...> {
public:
	template<class Di, unsigned Idx = 0> void init(Di di) {//di = random distribution
		if constexpr(Idx < sizeof...(Ns) - 1) {
			auto& a = weight<Idx>(*this);
			for(int i=0; i<a.width(); i++) for(int j=0; j<a.height(); j++) 
				a[i][j] = di(rd_);
			init<Di, Idx + 1>(di);
		}
	}
	void activation_function(std::function<float(float)> f) {
		activation_function_ = f;
	}
	void diff_activation(std::function<float(float)> f) { diff_activation_ = f; }
	auto feed_forward(std::vector<float> v) {
		for(int i=0; i<v.size(); i++) input<0>(*this)[0][i] = v[i];
		feed_forward();
		return this->result;
	}
	float back_propagation(std::vector<float> v, float alpha = 0.1) {
		float err = 0;
		auto& d = delta<sizeof...(Ns) - 2>(*this);
		for(int i = 0; i < d.height()-1; i++) {
			float o = this->result[0][i];
			float out = output<sizeof...(Ns) - 2>(*this)[0][i];
			d[0][i] = (v[i] - o) * diff_activation_(out);//??? (o)
			err += (v[i] - o ) * (v[i] - o);
		}
		calc_delta();
		update_weight(alpha);
		return 0.5 * err;
	}
	void save_weights(std::string filename) {
		std::ofstream f(filename);
		save_weights(f);
	}
	void load_weights(std::string filename) {
		std::ifstream f(filename);
		load_weights(f);
	}
protected:
	std::function<float(float)>
		activation_function_ = sigmoid, diff_activation_ = diff_sigmoid;
private:
	std::random_device rd_;
	static float sigmoid(float x) { return 1 / (1 + exp(-x)); }
	static float diff_sigmoid(float x) { return sigmoid(x) * (1 - sigmoid(x)); }
	template<unsigned Idx = 0> void save_weights(std::ostream& o) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			o << weight<Idx>(*this) << std::endl;
			save_weights<Idx + 1>(o);
		}
	}
	template<unsigned Idx = 0> void load_weights(std::istream& is) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			std::string s; float f;
			auto& w = weight<Idx>(*this);
			for(int j=0; j<w.height(); j++) {
				is >> s;
				for(int i=0; i<w.width(); i++) is >> w[i][j];
				is >> s;
			}
			load_weights<Idx + 1>(is);
		}
	}
	template<unsigned H> static auto add_bias(Cmat<float, 1, H> m) {
		Cmat<float, 1, H+1> r;
		for(int i=0; i<H; i++) r[0][i] = m[0][i];
		r[0][H] = 1;
		return r;
	}
	template<unsigned Idx = 0> void feed_forward() {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			output<Idx>(*this) = weight<Idx>(*this) * add_bias(input<Idx>(*this));
			if constexpr(Idx < sizeof...(Ns) - 2) {//activation
				auto& o = output<Idx>(*this).arr_[0];
				auto& i = input<Idx + 1>(*this).arr_[0];
				transform(o.begin(), o.end(), i.begin(), activation_function_);
			} 
			if constexpr(Idx == sizeof...(Ns) - 2) {
				auto& o = output<Idx>(*this).arr_[0];
				auto& r = this->result.arr_[0];
				transform(o.begin(), o.end(), r.begin(), activation_function_);
			}
			feed_forward<Idx + 1>();
		}
	}
	template<unsigned Idx = sizeof...(Ns) - 2> void calc_delta() {
		if constexpr(Idx > 0) {
			const auto& w = weight<Idx>(*this);
			for(int i=0; i<w.width(); i++) {
				float sum = 0;
				float o = output<Idx-1>(*this)[0][i];
				for(int j=0; j<w.height(); j++) 
					sum += w[i][j] * delta<Idx>(*this)[0][j];
				delta<Idx - 1>(*this)[0][i] = sum * diff_activation_(o);
			}
			calc_delta<Idx - 1>();
		}
	}
	template<unsigned Idx = 0> void update_weight(float a) {
		if constexpr(Idx < sizeof...(Ns) - 1) {
			auto& w = weight<Idx>(*this);
			auto in = add_bias(input<Idx>(*this));
			for(int i=0; i<w.width(); i++) for(int j=0; j<w.height(); j++) 
				w[i][j] += a * delta<Idx>(*this)[0][j] * in[0][i];
			update_weight<Idx + 1>(a);
		}
	}
};
