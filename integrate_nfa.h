#pragma once
#include "parseRegex.h"


using namespace std;
//�ɶ��nfa��ϳ�һ��nfa
class Integratednfa {
public:
	//nfa��״̬ת����
	vector<vector<unordered_set<int>*>*> transitionTable;
	//��ʼ״̬
	const static int start = 0;
	//�洢����״̬��Ӧ�Ķ���
	unordered_map<int, string> action;
	Integratednfa() {}
	//��������ʽ���䶯������nfa
	Integratednfa(vector<pair<string, string>>& regs);
	//��һ��nfa���䶯���������
	void integrate(nfa n,string action);
	
};
