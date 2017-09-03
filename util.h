#include<vector>
#include<string>

void serialize(const std::vector<std::string>& v, std::string filename);
std::vector<std::string> deserialize(std::string filename);
std::string psstm(std::string command);
std::string get_url(std::string);
