#pragma once
#include "parseRegex.h"


using namespace std;
//由多个nfa组合成一个nfa
class Integratednfa {
public:
	//nfa的状态转换表
	vector<vector<unordered_set<int>*>*> transitionTable;
	//起始状态
	const static int start = 0;
	//存储接收状态对应的动作
	unordered_map<int, string> action;
	Integratednfa() {}
	//由正则表达式及其动作生成nfa
	Integratednfa(vector<pair<string, string>>& regs);
	//把一个nfa及其动作加入进来
	void integrate(nfa n,string action);
	
};
