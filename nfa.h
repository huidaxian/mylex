#pragma once
#include <vector>
#include <unordered_set>
using namespace std;
class nfa {
public:
	//nfa��״̬ת����
	vector<vector<unordered_set<int>*>*> transitionTable;

	//��ʼ״̬
	const static int start = 0;

	//����״̬
	int accept;

	const static int colomns = 129;
	//��ʼ��ֻ��һ��״̬
	nfa();
	
	//��ʼ��������״̬����һ��״̬����һ���ַ�ch��ת���ڶ���״̬

	nfa(int ch);
	//��ʼ��������״̬����һ��״̬���ն���ַ�chs��ת���ڶ���״̬
	nfa(vector<int>& chs);
	
	



	//��ʼ��һ��״̬
	static vector<unordered_set<int>*>* initStateRow() ;

	//��ʼ��һ��״̬��������һ���ַ�ch,��ת�Ƶ���transition��״̬
	vector<unordered_set<int>*>* initStateRow(int ch, int transition) ;
	//��ʼ��һ��״̬�������ն���ַ�chars,��ת�Ƶ���transition��״̬
	vector<unordered_set<int>*>* initStateRow(vector<int>& chars, int transition) ;

	//���ת�ƹ���row״̬����ch��ת����transition��״̬
	static void addTransition(vector<unordered_set<int>*>* row, int ch, int transition) ;
	//���ת�ƹ��򣬵�row��״̬����һ���ַ�ch��ת����transition��״̬
	void addTransition(int row, int ch, int transition) ;
	//���ת�ƹ��򣬵�row��״̬���ն���ַ�chars��ת����transition��״̬
	void addTransition(int row, vector<int>& chars, int transition) ;
	//״̬ת����transitionTable��ÿһ��״̬�е�Ԫ�ؼ�value������nfa�ĺϲ�
	 vector<vector<unordered_set<int>*>*> increasedTable(int value) ;
};