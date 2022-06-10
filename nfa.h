#pragma once
#include <vector>
#include <unordered_set>
using namespace std;
class nfa {
public:
	//nfa的状态转换表
	vector<vector<unordered_set<int>*>*> transitionTable;

	//起始状态
	const static int start = 0;

	//接受状态
	int accept;

	const static int colomns = 129;
	//初始化只有一个状态
	nfa();
	
	//初始化有两个状态，第一个状态接收一个字符ch跳转到第二个状态

	nfa(int ch);
	//初始化有两个状态，第一个状态接收多个字符chs跳转到第二个状态
	nfa(vector<int>& chs);
	
	



	//初始化一个状态
	static vector<unordered_set<int>*>* initStateRow() ;

	//初始化一个状态，它接收一个字符ch,并转移到第transition号状态
	vector<unordered_set<int>*>* initStateRow(int ch, int transition) ;
	//初始化一个状态，它接收多个字符chars,并转移到第transition号状态
	vector<unordered_set<int>*>* initStateRow(vector<int>& chars, int transition) ;

	//添加转移规则，row状态接收ch跳转到第transition个状态
	static void addTransition(vector<unordered_set<int>*>* row, int ch, int transition) ;
	//添加转移规则，第row个状态接收一个字符ch跳转到第transition个状态
	void addTransition(int row, int ch, int transition) ;
	//添加转移规则，第row个状态接收多个字符chars跳转到第transition个状态
	void addTransition(int row, vector<int>& chars, int transition) ;
	//状态转换表transitionTable的每一个状态行的元素加value，用于nfa的合并
	 vector<vector<unordered_set<int>*>*> increasedTable(int value) ;
};