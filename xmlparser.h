#pragma once
#include<map>
#include<string>
#include<memory>
#include<regex>
#include"tgraph.h"

//typedef std::shared_ptr<std::map<std::string, std::string>> sh_map;
using sh_map = std::shared_ptr<std::map<std::string, std::string>>;
//<meta href="http://~~" name="" />
//{{"Mono":"meta"}, {"href":"http://~~"}, {"name":""}}
//first can be Mono, Text, HeadTail

class Parser : public Graph<sh_map>
{//html -> graph structure
public:
	std::string to_str(sh_map sh = nullptr) const;//translate child nodes to string
	void read_html(std::istream& is);//construct graph by reading html stream
	void read_html(std::string s);
	sh_map find_parent(sh_map child) const;
	std::vector<sh_map> regex_find(std::string a, std::string b, sh_map parent = nullptr);

private:
	std::vector<sh_map> vec;
	std::vector<std::string> scripts_;
	int script_count_;

	void read_stream(std::istream& is);
	void find_all(std::regex first, std::regex second, sh_map parent);
	void insert_edge(sh_map shp, std::istream& is);
	std::map<std::string, std::string> parse_bracket(std::istream& is);
	std::string get_bracket(std::istream& is);
};


