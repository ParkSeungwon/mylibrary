#include<vector>
#include<string>
#include<utility>

class Tokenizer 
{//inside quote separator is invalid, quote has priority. "[ ]" -> " "
public:
	std::vector<std::string> tokenize(std::string s);
	void quote(std::string open, std::string close);
	void separate(std::string sep);
	void escape(char esc);
	void clear();
	
protected:
	std::vector<std::string> separate_;
	std::vector<std::pair<std::string, std::string>> quote_;// v open, close
	char escape_ = '\\';

private:
	std::vector<std::pair<size_t, size_t>> quote_pos_, sep_pos_;//open,close / pos,size
	std::vector<size_t> escape_pos_;
	std::vector<std::pair<size_t, size_t>> quo_find(std::string s, size_t begin,
			size_t end, std::string open, std::string close) const;
	std::vector<std::pair<size_t, size_t>> quo_find(std::string s,
			std::string open, std::string close) const;
	size_t find_match(std::string s, size_t o_pos, std::string o, std::string c,
			int sub_level) const;//find matching quote consisidering sub level
};
