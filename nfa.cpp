#include <iostream>

#include "nfa.h"

	
	nfa::nfa() {
		accept = 0;
		transitionTable.push_back(initStateRow());
	}
	
	nfa::nfa(int ch) {
		vector<unordered_set<int>*>* startState = initStateRow(ch,1);
		vector<unordered_set<int>*>* acceptState = initStateRow();
		transitionTable.push_back(startState);
		transitionTable.push_back(acceptState);
		accept = 1;
	}
	nfa::nfa(vector<int>& chs) {
		vector<unordered_set<int>*>* startState = initStateRow(chs, 1);
		vector<unordered_set<int>*>* acceptState = initStateRow();
		transitionTable.push_back(startState);
		transitionTable.push_back(acceptState);
		accept = 1;
	}
	


	
	vector<unordered_set<int>*>* nfa::initStateRow() {
		vector<unordered_set<int>*>* row = new vector<unordered_set<int>*>();
		for (int i = 0;i < colomns;i++) {
			row->push_back(new unordered_set<int>);
		}
		return row;
	}

	
	vector<unordered_set<int>*>* nfa::initStateRow(int ch, int transition) {
		vector<unordered_set<int>*>* row = new vector<unordered_set<int>*>();
		for (int i = 0;i < colomns;i++) {
			row->push_back(new unordered_set<int>);
		}
		addTransition(row, ch, transition);
		return row;
	}

	

	vector<unordered_set<int>*>* nfa::initStateRow(vector<int>& chars, int transition) {
		vector<unordered_set<int>*>* row = new vector<unordered_set<int>*>();
		for (int i = 0;i < colomns;i++) {
			row->push_back(new unordered_set<int>);
		}
		for (int ch : chars) {
			addTransition(row, ch, transition);
		}
		return row;
	}

	
	 void nfa::addTransition(vector<unordered_set<int>*>* row, int ch, int transition) {
		row->at(ch)->insert(transition);
	}
	void nfa::addTransition(int row, int ch, int transition) {
		transitionTable.at(row)->at(ch)->insert(transition);
	}
	void nfa::addTransition(int row, vector<int>& chars, int transition) {
		for (int ch : chars) {
			transitionTable.at(row)->at(ch)->insert(transition);
		}
	}

	vector<vector<unordered_set<int>*>*> nfa::increasedTable(int value) {
		vector<vector<unordered_set<int>*>*> newTable;
		int rows = transitionTable.size();
		for (int i = 0;i < rows;i++) {
			newTable.push_back(initStateRow());
			
			for (int j = 0;j < colomns;j++) {
				for (int k: *transitionTable.at(i)->at(j)) {
					newTable.at(i)->at(j)->insert(k+value);
				}
			}
		}
		return newTable;
	}

	





	



