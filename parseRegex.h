#pragma once
#include "nfa.h"

#include <unordered_map>
#include <vector>
#include <stack>

//原子式，用于正则表达式的分割，当type为0时，代表特殊字符 * ？+ ( )，为1时，代表nfa
class AtomExp {
public:
	int ch;
	nfa n;
	bool type;
	AtomExp(nfa n) {
		this->n = n;
		type = 1;
	}

	AtomExp(int ch) {
		this->ch = ch;
		type = 0;
	}


};
//中缀转后缀时的一个哨兵，方便运算
const int BOTTOM = 10000;
//自定义连接符
const int CONCAT = 10001;
//将一个正则表达式转化为nfa
nfa regExpToNFA(string regExp);
//正则表达式解析为原子表达式的一个数组，用于后续的运算
vector<AtomExp> regExpToSubNFAs(string regExp, unordered_map<int, int>& priorityTable);
//针对[]里的字符生成代表nfa的AtomExp
AtomExp square(vector<int>& chs);
//针对[^]里的字符生成代表nfa的AtomExp
AtomExp nnot(vector<int>& chs);
//查找一个栈是否含有某一个元素
static bool searchElement(stack<int> s, int ch);

//连接两个nfa
nfa concat(nfa& a, nfa& b);
//为通配符.生成nfa
nfa dot();
//按照规则在AtomExp之间添加连接符
vector<AtomExp> addConcatForNFAs(vector<AtomExp>& v);
//针对*生成nfa
nfa star(nfa n);
//针对？生成nfa
nfa question(nfa n);
//针对+生成nfa
nfa pluus(nfa n);
////针对|生成nfa
nfa oor(nfa n1, nfa n2);
