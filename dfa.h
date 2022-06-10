#pragma once
#include "integrate_nfa.h"
#include <set>
class dfa {
public:
	//二维数组作为状态转换表
	vector<vector<int>> transitionTable;
	//起始状态
	 int start = 0;
	//接收状态及其动作
	unordered_map<int, string> actionTable;
	//nfa里的闭包状态集合表
	vector<unordered_set<int>> closureTable;
	//要转化的nfa
	Integratednfa nfa;
	
	dfa(Integratednfa& nfa);
	dfa(vector<vector<int>>& transitionTable, unordered_map<int, string>& actionTable, int start);
	//获取起始状态的闭包状态集
	unordered_set<int> getClosureOfStart();
	//获取多个状态集合的闭包状态集
	unordered_set<int> getClosures(unordered_set<int>& states);
	//获取多个状态集合接收字符ch后的状态集合
	unordered_set<int> transition(unordered_set<int>& states, int ch);
	//针对状态state生成新的状态添加到transitionTable
	void fillTransitionTable(int state);
	//根据nfa为dfa建立新的状态到动作的映射，当dfa里的新状态包含多个nfa里的接收状态时，选择序号最小的接收状态的动作作为dfa新状态的动作
	void fillActionTable();
	//搜索栈里是否存在某一个元素
	bool searchStack(stack<int> s, int i);

};
//最小化dfa
dfa minimize(dfa& d);
//针对原始dfa和计算出来的新的分割集合生成最简dfa
dfa deleteStatesDuplicated(dfa& d, vector<unordered_set<int>>& divides);
//根据动作表对第index个状态集合进行分割
int divideAt(vector<unordered_set<int>>& divides, int index, unordered_map<int, string>& actionTable);
//根据状态转换表对第index个状态集合进行分割
int divideAt(vector<unordered_set<int>>& divides, int index, vector<vector<int>>& transitionTable);
//判断两个状态是否等价
bool equal(vector<int> s1, vector<int> s2, vector<unordered_set<int>>& divides);
//查找状态state被包含在第几个状态集合里
int getIndex(int state, vector<unordered_set<int>>& divides);