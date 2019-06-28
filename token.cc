#include<algorithm>
#include"token.h"
using namespace std;

void Tokenizer::quote(string open, string close)
{
	quote_.push_back({open, close});
}
void Tokenizer::escape(char c)
{
	escape_ = c;
}
void Tokenizer::separate(string s)
{
	separate_.push_back(s);
}
void Tokenizer::clear()
{
	quote_.clear();
	separate_.clear();
}

size_t Tokenizer::find_match(string s, size_t pos, string o, string c, int sub) const
{//o : open quote, c : closing quote, sub : sub level, pos : begin search
	size_t i, j;
	do i = s.find(o, pos);
	while(find(escape_pos_.begin(), escape_pos_.end(), i -1) != escape_pos_.end());
	do j = s.find(c, pos);
	while(find(escape_pos_.begin(), escape_pos_.end(), j -1) != escape_pos_.end());

	if(i < j) return find_match(s, i + o.size(), o, c, sub + 1);
	else if(!sub) return j + c.size() - 1;//if 2char quote return last closing one
	else return find_match(s, j + c.size(), o, c, sub - 1);
}
vector<pair<size_t, size_t>> Tokenizer::quo_find(string s, string o, string c) const
{
	vector<pair<size_t, size_t>> r;
	for(size_t begin=0, end=0; ;) {
		do begin = s.find(o, end ? end + c.size() : 0);
		while(find(escape_pos_.begin(), escape_pos_.end(), begin - 1)
				!= escape_pos_.end());//except escaped quote
		if(begin != string::npos) end = find_match(s, begin + o.size(), o, c, 0);
		if(begin != string::npos && end != string::npos) r.push_back({begin, end});
		else break;
	}
	return r;
}
vector<pair<size_t, size_t>>
Tokenizer::quo_find(string s, size_t beg, size_t end, string o, string c) const
{//find quotes between [beg, end] of string s
	s = s.substr(beg, end - beg + 1);
	auto r = quo_find(s, o, c);
	for(auto &[a, b] : r) a += beg, b += beg;
	return r;
}
vector<string> Tokenizer::tokenize(string s)
{
	for(size_t it = s.find(escape_); it != string::npos;) {
		escape_pos_.push_back(it);
		it = s.find(escape_, it + 2);// case esc+esc
	}
	for(const auto &[open, close] : quote_) {
		size_t start = 0;
		vector<pair<size_t, size_t>> tmp;
		for(const auto &[o, c] : quote_pos_) {
			auto quotes = quo_find(s, start, o - 1, open, close);
			start = c + close.size();
			tmp.insert(tmp.end(), quotes.begin(), quotes.end());
		}
		auto quotes = quo_find(s, start, s.size() - 1, open, close);
		tmp.insert(tmp.end(), quotes.begin(), quotes.end());
		quote_pos_.insert(quote_pos_.end(), tmp.begin(), tmp.end());
		sort(quote_pos_.begin(), quote_pos_.end());
	}
	for(const string &sep : separate_)
	for(size_t it=s.find(sep); it != string::npos; it=s.find(sep, it+sep.size()))
		if(find_if(quote_pos_.begin(), quote_pos_.end(), [it](pair<size_t, size_t> a)
					{return a.first < it && it < a.second;}) == quote_pos_.end() &&
			find(escape_pos_.begin(), escape_pos_.end(), it -1) == escape_pos_.end())
			sep_pos_.push_back({it, sep.size()});//insert if not inside quote
	sort(sep_pos_.begin(), sep_pos_.end());

	vector<string> r; size_t start = 0;
	for(auto [pos, sz] : sep_pos_) {
		r.push_back(s.substr(start, pos - start));
		start = pos + sz;
	}
	r.push_back(s.substr(start));
	escape_pos_.clear();
	sep_pos_.clear();
	quote_pos_.clear();
	return r;
}
