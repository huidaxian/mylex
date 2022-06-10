#pragma once
#include "integrate_nfa.h"
#include <set>
class dfa {
public:
	//��ά������Ϊ״̬ת����
	vector<vector<int>> transitionTable;
	//��ʼ״̬
	 int start = 0;
	//����״̬���䶯��
	unordered_map<int, string> actionTable;
	//nfa��ıհ�״̬���ϱ�
	vector<unordered_set<int>> closureTable;
	//Ҫת����nfa
	Integratednfa nfa;
	
	dfa(Integratednfa& nfa);
	dfa(vector<vector<int>>& transitionTable, unordered_map<int, string>& actionTable, int start);
	//��ȡ��ʼ״̬�ıհ�״̬��
	unordered_set<int> getClosureOfStart();
	//��ȡ���״̬���ϵıհ�״̬��
	unordered_set<int> getClosures(unordered_set<int>& states);
	//��ȡ���״̬���Ͻ����ַ�ch���״̬����
	unordered_set<int> transition(unordered_set<int>& states, int ch);
	//���״̬state�����µ�״̬��ӵ�transitionTable
	void fillTransitionTable(int state);
	//����nfaΪdfa�����µ�״̬��������ӳ�䣬��dfa�����״̬�������nfa��Ľ���״̬ʱ��ѡ�������С�Ľ���״̬�Ķ�����Ϊdfa��״̬�Ķ���
	void fillActionTable();
	//����ջ���Ƿ����ĳһ��Ԫ��
	bool searchStack(stack<int> s, int i);

};
//��С��dfa
dfa minimize(dfa& d);
//���ԭʼdfa�ͼ���������µķָ���������dfa
dfa deleteStatesDuplicated(dfa& d, vector<unordered_set<int>>& divides);
//���ݶ�����Ե�index��״̬���Ͻ��зָ�
int divideAt(vector<unordered_set<int>>& divides, int index, unordered_map<int, string>& actionTable);
//����״̬ת����Ե�index��״̬���Ͻ��зָ�
int divideAt(vector<unordered_set<int>>& divides, int index, vector<vector<int>>& transitionTable);
//�ж�����״̬�Ƿ�ȼ�
bool equal(vector<int> s1, vector<int> s2, vector<unordered_set<int>>& divides);
//����״̬state�������ڵڼ���״̬������
int getIndex(int state, vector<unordered_set<int>>& divides);