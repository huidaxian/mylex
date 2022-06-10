
#include <iostream>
#include "parseRegex.h"

using namespace std;


nfa regExpToNFA(string regExp) {
	unordered_map<int,int> priorityTable;
	priorityTable.insert({ BOTTOM, 0 });
	priorityTable.insert({ '(',1 });
	priorityTable.insert({ '|', 2 });
	priorityTable.insert({ CONCAT, 3 });
	priorityTable.insert({ '*', 4 });
	priorityTable.insert({ '+', 4 });
	priorityTable.insert({ '?', 4 });
	priorityTable.insert({ ')', 5 });

	
	vector<AtomExp> nfaQueue_infix = regExpToSubNFAs(regExp, priorityTable);
	nfaQueue_infix = addConcatForNFAs(nfaQueue_infix);
	vector<AtomExp> nfaQueue_sufix;
	stack<int> stk;
	stk.push(BOTTOM);
	for (AtomExp a : nfaQueue_infix) {
		if (a.type) {
			nfaQueue_sufix.push_back(a);
		}
		else {
			int ch = a.ch;
			if (ch == '(') {
				stk.push(ch);
			}
			else if (ch == ')') {
				while (stk.top() != '(') {
					nfaQueue_sufix.push_back(stk.top());
					stk.pop();
				}
				stk.pop();

			}
			
			else if (priorityTable.at(ch) <= priorityTable.at(stk.top())) {
				nfaQueue_sufix.push_back(stk.top());
				stk.pop();
				stk.push(ch);
			}
			else {
				stk.push(ch);
			}
		}
	}
	while (!stk.empty()) {
		nfaQueue_sufix.push_back(stk.top());
		stk.pop();
	}
	stack<nfa> nfastack;
	for (AtomExp a : nfaQueue_sufix) {
		if (a.type) {
			nfastack.push(a.n);
		}
		else {
			int op = a.ch;
			if (op == CONCAT) {
				nfa right = nfastack.top();
				nfastack.pop();
				nfa left = nfastack.top();
				nfastack.pop();
				nfastack.push(concat(left, right));
			}
			else {
				switch (op) {
				case '*': {
					nfa cur = nfastack.top();
					nfastack.pop();
					nfastack.push(star(cur));
					break;
				}
				case '?': {
					nfa cur = nfastack.top();
					nfastack.pop();
					nfastack.push(question(cur));
					break;
				}
				case '+': {
					nfa cur = nfastack.top();
					nfastack.pop();
					nfastack.push(pluus(cur));
					break;
				}
				case '|': {
					nfa cur1 = nfastack.top();
					nfastack.pop();
					nfa cur2 = nfastack.top();
					nfastack.pop();
					nfastack.push(oor(cur1,cur2));
					break;
				}
				}
			}
		}
	}
	if (nfastack.size()!=1) {
		throw("Lex syntax error - Wrong regular expression");
	}

	return nfastack.top();
}

vector<AtomExp> regExpToSubNFAs(string regExp, unordered_map<int, int>& priorityTable) {
	stack<int> charStack ;
	vector<AtomExp> nfaQueue ;

	//"
	const int QUOTE = -1;
	// [] 
	const int SQUARE = -2;
	//[^]
	const int NOT = -3;
	 

	const int* lock = nullptr;
	for (int i = 0;i < regExp.size();i++) {
		int cur = regExp[i];
		if (lock != nullptr) {
			if (*lock == QUOTE) {
				if (cur != '"' && cur != '\\') {
					charStack.push(cur);
					continue;
				}
			}

			if (*lock == SQUARE) {
				if (cur != '^' && cur != '\\' && cur != ']') {
					charStack.push(cur);
					continue;
				}
			}
			if (*lock == NOT) {
				if (cur != '\\' && cur != ']') {
					charStack.push(cur);
					continue;
				}
			}
		}
		switch (cur) {
		case '\\': {
			i++;
			int ch = regExp[i];
			if (lock != nullptr) {
				if (ch == 'n') {
					charStack.push('\n');
				}
				else if (ch == 'r') {
					charStack.push('\r');
				}
				else if (ch == 't') {
					charStack.push('\t');
				}
				else charStack.push(ch);
			}
			else {
				nfa a(ch);
				nfaQueue.push_back(a);
			}
			break;
		}
		case '[': {
			charStack.push(SQUARE);
			lock = &SQUARE;
			break;
		}
		case '^':{
			if (charStack.top() == SQUARE) {
				charStack.pop();
				lock = &NOT;
				charStack.push(NOT);
			}
			else {
				charStack.push('^');
			}
			
			break;
		}
		case ']': {
			if (lock == nullptr) {
				throw("Syntax error : [] mismatch");
			}
			vector<int> chars;
			while (charStack.top() != *lock) {
				int ch = charStack.top();
				charStack.pop();
				if (!charStack.empty()&&charStack.top() == '-') {
					charStack.pop();
					if (charStack.top() == *lock) {
						chars.push_back('-');
					}
					else {
						int left = charStack.top();
						charStack.pop();
						if (left > ch) {
							throw("Syntax error : - ");
						}
						for (int p = left;p <= ch;p++) {
							chars.push_back(p);
						}
					}

				}
				else {
					chars.push_back(ch);
				}
			}
			int sign = charStack.top();
			charStack.pop();
			if (sign == SQUARE) {
				nfaQueue.push_back(square(chars));
			}
			else {
				nfaQueue.push_back(nnot(chars));
			}
			lock = nullptr;
			break;
		}
		case '"': {
			if (searchElement(charStack, QUOTE)) {
				nfa n;
				while (charStack.top() != QUOTE) {
					nfa temp(charStack.top());
					charStack.pop();
					n = concat(temp, n);
				}
				charStack.pop();
				nfaQueue.push_back(n);
				lock = nullptr;
			}
			else {
				charStack.push(QUOTE);
				lock = &QUOTE;
			}
			
			break;
		}
		case '.': {
			nfaQueue.push_back(dot());
			break;
		}
		default: {
			if (priorityTable.find(cur) == priorityTable.end()) {
				nfaQueue.push_back(nfa(cur));
			}
			else {
				nfaQueue.push_back(cur);
			}
			break;
		}
		}
	}
	return nfaQueue;
}

//查找栈里是否有某一个元素
bool searchElement(stack<int> s,int ch) {
	while (!s.empty()) {
		if (s.top() == ch) {
			return true;
		}
		s.pop();
	}
	return false;

}

//为[]里的字符生成原子式
AtomExp square(vector<int>& chs) {
	nfa a(chs);
	AtomExp atom(a);
	return atom;
}
//为[^]里的字符生成原子式
AtomExp nnot(vector<int>& chs) {
	nfa res;
	for (int i = 0;i < res.colomns-1;i++) {
		if (find(chs.begin(), chs.end(), i) != chs.end()) {
			continue;
		}
		res.addTransition(0, i, 1);

	}
	res.transitionTable.push_back(res.initStateRow());
	res.accept = 1;
	return res;
}


//连接两个nfa
nfa concat(nfa& a,nfa& b ) {
	a.transitionTable.pop_back();
	for (auto row : b.increasedTable(a.transitionTable.size())) {
		a.transitionTable.push_back(row);
	}
	a.accept += b.accept;
	return a;
	
}
//为通配符.生成nfa
nfa dot() {
	nfa res;
	for (int i = 0;i < res.colomns-1;i++) {
		if(i!=10)
			res.addTransition(0, i, 1);
	}
	res.transitionTable.push_back(res.initStateRow());
	res.accept = 1;
	return res;
}

vector<AtomExp> addConcatForNFAs(vector<AtomExp>& v) {
	vector<AtomExp> res;
	bool flag = false;
	for (vector<AtomExp>::iterator it = v.begin();it!=v.end();it++) {
		if ((*it).type) {
			if (flag) {
				
				res.push_back(CONCAT);
			}
			res.push_back(*it);
			flag = true;
		}
		else {
			int ch = (*it).ch;
			if (flag&&ch == '(') {
				res.push_back(CONCAT);
				
			}
			flag = ch=='*' || ch=='?' || ch=='+' || ch==')';
			res.push_back(*it);
		}
	}
	return res;
}
nfa star(nfa n) { 
	nfa result;
	n.transitionTable = n.increasedTable(1);
	result.transitionTable.insert(result.transitionTable.end(), n.transitionTable.begin(),n.transitionTable.end());
	result.accept += n.accept + 2;
	int accept = result.accept;
	result.transitionTable.at(0)->at(128)->insert(1);
	result.transitionTable.at(0)->at(128)->insert(result.accept);
	result.transitionTable.at(accept - 1)->at(128)->insert(1);
	result.transitionTable.at(accept - 1)->at(128)->insert(result.accept);
	result.transitionTable.push_back(result.initStateRow());
	return result;
}
nfa question(nfa n) { 
	nfa result;
	n.transitionTable = n.increasedTable(1);
	result.transitionTable.insert(result.transitionTable.end(), n.transitionTable.begin(), n.transitionTable.end());
	result.accept += n.accept + 2;
	int accept = result.accept;
	result.transitionTable.at(0)->at(128)->insert(1);
	result.transitionTable.at(0)->at(128)->insert(result.accept);
	result.transitionTable.at(accept - 1)->at(128)->insert(result.accept);
	result.transitionTable.push_back(result.initStateRow());
	return result;
	
}
nfa pluus(nfa n) { 
	nfa temp = star(n);
	nfa result = concat(n, temp);
	return result; 
};
nfa oor(nfa n1, nfa n2) {
	nfa result;
	int next = 1;
	result.addTransition(0, 128, next);
	n1.transitionTable = n1.increasedTable(next);
	next += n1.accept + 1;
	result.addTransition(0, 128, next);
	n2.transitionTable = n2.increasedTable(next);
	next += n2.accept + 1;
	result.transitionTable.insert(result.transitionTable.end(), n1.transitionTable.begin(), n1.transitionTable.end());
	int accept1 = result.transitionTable.size() - 1;
	result.transitionTable.insert(result.transitionTable.end(), n2.transitionTable.begin(), n2.transitionTable.end());
	int accept2 = result.transitionTable.size() - 1;
	result.transitionTable.push_back(result.initStateRow());
	result.accept = next;
	result.addTransition(accept1, 128, next);
	result.addTransition(accept2, 128, next);
	return result; 
};
