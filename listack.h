#include<cstdlib>
#include<functional>
#include<iostream>

template <typename T> class Listack
{
public:
	T pop() {
		if(next) {
			data = next->data;
			del_next(this);
			return data;
		} else throw "popping empty List";
	}
	void push(T n) {
		insert(this, n);
	}
	bool is_empty() {
		return !next;
	}
	virtual ~Listack() {
		while(next) pop();
	}
	void push_back(T n) {
		Listack<T>* prev = this;
		for(auto p = next; p; p = p->next) prev = p;
		insert(prev, n);
	}
	T pop_back() {
		if(next) {
			Listack<T> *prev = this, *pprev = this;
			for(auto p = next; p; p = p->next) {
				pprev = prev;
				prev = p;
			}
			data = prev->data;
			del_next(pprev);
			return data;
		} else throw "popping back empty";
	}
	void del_if(std::function<bool(T)> fn) {
		Listack<T>* prev = this;
		for(auto p = next; p; p = p->next) {
			if(fn(p->data)) {
				del_next(prev);
				p = prev;
			}
			prev = p;
		}
	}
	void insert(T n) {
		Listack<T>* prev = this;
		for(auto p = next; p && p->data < n; p = p->next) prev = p;
		insert(prev, n);
	}
	void show() {
		for(auto p = next; p; p = p->next) std::cout << p->data << ' ';
		std::cout << std::endl;
	}

protected:
	T data;
	Listack<T>* next = nullptr;

private:
	void insert(Listack<T>* pos, T n) {//insert behind pos
		if(pos) {
			Listack<T>* o = (Listack*)malloc(sizeof(Listack<T>));
			o->data = n;
			o->next = pos->next;
			pos->next = o;
		} else throw "inserting at null terminationg pointer";
	}
	void del_next(Listack<T>* pos) {
		if(pos->next) {
			auto tmp = pos->next;
			pos->next = pos->next->next;
			free(tmp);
		} else throw "no next to delete";
	}
};
