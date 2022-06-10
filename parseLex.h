#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
using namespace std;
class LexFile {
public:
	//要读取的lex文件
	ifstream readFile;
	//%{  }%里的内容
	string headers;
	//正则定义的存储
	unordered_map<string,string> macros;
	//正则表达式与其动作
	vector<pair<string, string>> regExps;
	//用户程序段
	string userSegment;
	//对lex文件进行解析
	LexFile(string path);
	//读取正则定义宏
	void readMacros();
	//读取头文件内容
	void readHeaders();
	//读取正则表达式
	void readRegExps();
	//读取用户程序段
	void readUserSeg();
	//正则定义宏展开
	string expandMacro(string expWithMacro);



};
//分割字符串
void split(const string& s, vector<string>& tokens, const string& delimiters );
//去除首尾空白字符
inline  string& trim(string& st);