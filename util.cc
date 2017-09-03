#include<vector>
#include<fstream>
using namespace std;

void serialize(const vector<string>& v, string filename) 
{
	ofstream f(filename);
	f << v.size() << ' ';
	for(const string& s : v) f << s.size() << ' ';
	for(const string& s : v) f << s;
}

vector<string> deserialize(string filename)
{
	ifstream f(filename);
	int n;
	f >> n;
	int sz[n];
	for(int i=0; i<n; i++) f >> sz[i];
	char c;
	f >> noskipws >> c;//consume trailing ' '

	vector<string> v;
	for(int i=0; i<n; i++) {
		string s;
		for(int j=0; j<sz[i]; j++) {
			f >> noskipws >> c;
			s += c;
		}
		v.push_back(s);
	}
	return v;
}		

string psstm(string command)
{//return system call output as string
	string s;
	char tmp[1000];
	FILE* f = popen(command.c_str(), "r");
	while(fgets(tmp, sizeof(tmp), f)) s += tmp;
	pclose(f);
	return s;
}
