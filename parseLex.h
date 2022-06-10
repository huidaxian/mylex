#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
using namespace std;
class LexFile {
public:
	//Ҫ��ȡ��lex�ļ�
	ifstream readFile;
	//%{  }%�������
	string headers;
	//������Ĵ洢
	unordered_map<string,string> macros;
	//������ʽ���䶯��
	vector<pair<string, string>> regExps;
	//�û������
	string userSegment;
	//��lex�ļ����н���
	LexFile(string path);
	//��ȡ�������
	void readMacros();
	//��ȡͷ�ļ�����
	void readHeaders();
	//��ȡ������ʽ
	void readRegExps();
	//��ȡ�û������
	void readUserSeg();
	//�������չ��
	string expandMacro(string expWithMacro);



};
//�ָ��ַ���
void split(const string& s, vector<string>& tokens, const string& delimiters );
//ȥ����β�հ��ַ�
inline  string& trim(string& st);