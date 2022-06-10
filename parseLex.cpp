#include "parseLex.h"
#include <string>
#include <algorithm>
#include <functional>
void split(const string& s, vector<string>& tokens, const string& delimiters = "\t")
{
	string::size_type lastPos = s.find_first_not_of(delimiters, 0);
	string::size_type pos = s.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}

inline string & lTrim(string & ss)
{
	    string::iterator   p = find_if(ss.begin(), ss.end(), not1(ptr_fun(isspace)));
	    ss.erase(ss.begin(), p);
	    return  ss;
	}

inline  string & rTrim(string & ss)
{
	   string::reverse_iterator  p = find_if(ss.rbegin(), ss.rend(), not1(ptr_fun(isspace)));
	    ss.erase(p.base(), ss.end());
	   return   ss;
	}

inline   string & trim(string & st)
{
	    lTrim(rTrim(st));
	    return   st;
	}
LexFile::LexFile(string path) {
	readFile.open(path);
	if (!readFile) {
		throw "文件不能打开！";
	}
	readMacros();
	readRegExps();
	readUserSeg();
}
void LexFile::readMacros() {
	while (true) {
		string line;
		getline(readFile,line);
		if (line.compare("%%") == 0) {
			
			return;
		}
		else if (line.compare("%{") == 0) {
			
			readHeaders();
		}
		else {
			vector<string> regs;
			split(line, regs);
			if (regs.size() != 2) {
				throw("Macro definitions not normative!");
			}
			regs[0] = trim(regs[0]);
			regs[1] = trim(regs[1]);
			macros[regs[0]] = expandMacro(regs[1]);
		}
	}
}
void LexFile::readHeaders() {
	while (true) {
		string line;
		getline(readFile, line);
		if (line.compare("%}")==0) {
			return;
		}
		headers += line+'\n';
	}
}
void LexFile::readRegExps() {
	while (true) {
		string line;
		getline(readFile, line);
		 if (line.compare("%%")==0) return;
		else {
			 vector<string> regs;
			 split(line, regs);
			 if (regs.size() != 2) {
				 continue;
			 }
			 regs[0] = expandMacro(trim(regs[0]));
			 regs[1] = trim(regs[1]);
			 pair<string, string> reg(regs[0], regs[1]);
			 regExps.push_back(reg);
		}
	}
}
void LexFile::readUserSeg() {
	while (!readFile.eof()) {
		string line;
		getline(readFile, line);
		
			userSegment += line+'\n';
		
	}
}
string LexFile::expandMacro(string expWithMacro) {
	int left = -1, right = -1;
	for (int i = 0;i < expWithMacro.size();i++) {
		if (expWithMacro[i] == '{') {
			left = i;
		}
		if (expWithMacro[i] == '}') {
			right = i;
			if (left != -1) {
				string macro = expWithMacro.substr(left + 1, right - left - 1);
				string newExp = expWithMacro.substr(0, left) + macros[macro] + expWithMacro.substr(right + 1);
				expWithMacro = newExp;
			}
			left = -1;
		}
	}
	return expWithMacro;
}
