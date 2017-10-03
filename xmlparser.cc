#include<iostream>
#include<regex>
#include<cassert>
#include<exception>
#include"xmlparser.h"
using namespace std;

string Parser::get_bracket(istream& is)
{//cut < ~ > or text_between brackets
	string s;
	char c;
	if(Graph::root == nullptr) is >> skipws;//until insert html
	else is >> noskipws;
	if(is >> c) {//if not eof
		if(c == '<') {
			int open = 1;
			while(open) {
				s += c;
				if(!(is >> noskipws >> c)) throw "reached EOF without closing bracket";
				if(c == '<') open++;
				else if(c == '>') open--;
			}
			s += '>';
			if(s[1] == '!') return get_bracket(is);//pass comment
		} else {
			while(c != '<') {
				s += c;
				if(!(is >> noskipws >> c)) break;//deal with ending text
			}
			is.seekg(-1, is.cur);
			if(Graph::root == nullptr) return get_bracket(is);//for text before html
		}
	}
	return s;
}

static const char* void_element[] = {//elements that are always Mono
	"area", "base", "br", "col", "command", "embed", "hr", "img", "input",
	"keygen", "link", "meta", "param", "source", "track", "wbr"
};

map<string, string> Parser::parse_bracket(std::istream& is)
{//parse <tag></tag>, <tag />, Text
	string s;
	try {
		s = get_bracket(is);
	} catch(const char* e) { cerr << e;}
	map<string, string> t;
	if(s == "") return t;//eof
	if(s[0] == '<') {
		if(s[1] == '/') return t;//return empty on close
		regex tag_e{R"li(<(\S+)[>\s])li"};
		regex opt_e{R"li((\S+)=['"]([^'"]*)["'])li"};
		smatch m;
		regex_search(s, m, tag_e);
		bool mono = false;
		for(auto a : void_element) if(a == m[1]) mono = true;
		if(s[s.length() -2] == '/') mono = true;
		t[mono ? "Mono" : "HeadTail"] = m[1];

		while(regex_search(s, m, opt_e)) {
			t[m[1]] = m[2];
			s = m.suffix();
		}
	} else if(regex_match(s, regex{"\\s*"})) return parse_bracket(is);//Text contetn empty
	else t["Text"] = s;
	return t;
}

void Parser::insert_edge(sh_map shp, std::istream& is)
{
	map<string, string> t = parse_bracket(is);
	if(t.empty()) return;
	auto sh = make_shared<map<string, string>>(t);
	insert_vertex(sh);
	if(shp) Graph::insert_edge(shp, sh);
	if(t.begin()->first == "HeadTail") {
		insert_edge(sh, is);
		if(t.begin()->second == "script") {
			map<string, string> m;
			if(script_count_ < scripts_.size()) m["Text"] = scripts_[script_count_++];
			auto scr = make_shared<map<string, string>>(m);
			if(m["Text"] != "") {
				Graph::insert_vertex(scr);
				Graph::insert_edge(sh, scr);
			}
		}
	}
	insert_edge(shp, is);
}

void Parser::read_html(istream& is)
{
	string s; char c;
	while(is >> noskipws >> c) s += c;
	read_html(s);
}

void Parser::read_stream(istream& is)
{
	cout << "parsing..." << flush;
	insert_edge(nullptr, is);
	cout << "done" << endl;
}

void Parser::read_html(string s)
{//preprocess java script
	regex e{"(<script.*?>)(.*?)(</script>)"};
	smatch m;
	string r;
	while(regex_match(s, m, e)) {
		scripts_.push_back(m[2]);
		cout << m[0] << endl;
		cout << "script is : " << m[2] << endl;
		s = m.suffix().str();
		r += m.prefix().str() + m[1].str() + m[3].str();
	}
	r += s;
	stringstream ss;
	ss << r;
	script_count_ = 0;
	read_stream(ss);
	scripts_.clear();
}

string Parser::to_str(sh_map shp) const
{//parse from root if null 
	if(!shp) shp = Graph::root->data;
	auto* v = Graph::find(Graph::root, shp);
	if(!v) return "";
	auto it = v->data->cbegin();
	string type = it->first, text = it->second;
	string s;
	if(type == "Text") return text;
	else {
		s = '<' + it->second;
		for(it++; it != v->data->cend(); it++)//options
			s += ' ' + it->first + "=\"" + it->second + "\"";
		if(type == "Mono") s += " /";
		s += '>';
	}
	//add inner brackets recursively
	for(Edge<sh_map>* e = v->edge; e; e = e->edge) s += to_str(e->vertex->data);
	if(type == "HeadTail") s += "</" + text + '>';//closing after inner
	return s;
}

void Parser::find_all(regex a, regex b, sh_map parent)
{//warn : stack smashing, ulimit -s unlimited
	auto* p = Graph::find(Graph::root, parent);
	if(!p) p = Graph::root;
	if(!p) return;
	for(Edge<sh_map>* e = p->edge; e; e = e->edge) {
		for(const auto& sNs : *e->vertex->data) {
			cout << sNs.first << " : " << sNs.second << endl;
			if(regex_match(sNs.first, a) && regex_match(sNs.second, b)) {
				vec.push_back(e->vertex->data);
				break;
			}
		}
		find_all(a, b, e->vertex->data);
	}
}	

vector<sh_map> Parser::regex_find(string a, string b, sh_map parent)
{//find from parent, like true -> map[a] contains b, like false -> map[a] == b
	assert(vec.empty());
	find_all(regex{a}, regex{b}, parent);
	return move(vec);
}

sh_map Parser::find_parent(sh_map child) const
{//return parent node
	for(Vertex<sh_map>* v = Graph::root; v; v = v->vertex) 
		for(Edge<sh_map>* e = v->edge; e; e = e->edge) 
			if(e->vertex->data == child) return v->data;
	return nullptr;
}

