#pragma once
#include "nfa.h"

#include <unordered_map>
#include <vector>
#include <stack>

//ԭ��ʽ������������ʽ�ķָ��typeΪ0ʱ�����������ַ� * ��+ ( )��Ϊ1ʱ������nfa
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
//��׺ת��׺ʱ��һ���ڱ�����������
const int BOTTOM = 10000;
//�Զ������ӷ�
const int CONCAT = 10001;
//��һ��������ʽת��Ϊnfa
nfa regExpToNFA(string regExp);
//������ʽ����Ϊԭ�ӱ��ʽ��һ�����飬���ں���������
vector<AtomExp> regExpToSubNFAs(string regExp, unordered_map<int, int>& priorityTable);
//���[]����ַ����ɴ���nfa��AtomExp
AtomExp square(vector<int>& chs);
//���[^]����ַ����ɴ���nfa��AtomExp
AtomExp nnot(vector<int>& chs);
//����һ��ջ�Ƿ���ĳһ��Ԫ��
static bool searchElement(stack<int> s, int ch);

//��������nfa
nfa concat(nfa& a, nfa& b);
//Ϊͨ���.����nfa
nfa dot();
//���չ�����AtomExp֮��������ӷ�
vector<AtomExp> addConcatForNFAs(vector<AtomExp>& v);
//���*����nfa
nfa star(nfa n);
//��ԣ�����nfa
nfa question(nfa n);
//���+����nfa
nfa pluus(nfa n);
////���|����nfa
nfa oor(nfa n1, nfa n2);
