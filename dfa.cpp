#include "dfa.h"
#include <algorithm>



dfa::dfa(vector<vector<int>>& transitionTable, unordered_map<int, string>& actionTable, int start) {
	this->transitionTable = transitionTable;
	this->actionTable = actionTable;
	this->start = start;
}
dfa::dfa(Integratednfa& nfa) {
	this->nfa = nfa;
	closureTable.push_back(getClosureOfStart());
	while (closureTable.size() > transitionTable.size()) {
		fillTransitionTable(transitionTable.size());
	}
	fillActionTable();

}
unordered_set<int> dfa::getClosureOfStart() {
	unordered_set<int> startStates;
	startStates.insert(nfa.start);
	return getClosures(startStates);

}
unordered_set<int> dfa::getClosures(unordered_set<int>& states) {
	stack<int> statesToCal;
	unordered_set<int> result(states);
	for (int i : states) {
		statesToCal.push(i);
	}
	while (!statesToCal.empty()) {
		int stateToCal = statesToCal.top();
		statesToCal.pop();
		for (int i : *nfa.transitionTable.at(stateToCal)->at(128)) {
			if (!searchStack(statesToCal,i)) {
				result.insert(i);
				statesToCal.push(i);
			}
		}
	}
	return result;
}
unordered_set<int> dfa::transition(unordered_set<int>& states, int ch) {
	unordered_set<int> res;
	for (int i : states) {
		auto targetStates = nfa.transitionTable.at(i)->at(ch);
		res.insert(targetStates->begin(), targetStates->end());
	}
	return getClosures(res);
}
void dfa::fillTransitionTable(int state) {
	vector<int> stateRow;
	for (int i = 0;i < 128;i++) {
		unordered_set<int> target = transition(closureTable[state], i);
		if (target.empty()) {
			stateRow.push_back(-1);
			continue;
		}
		auto index = find(closureTable.begin(), closureTable.end(), target);
		if (index == closureTable.end()) {
			stateRow.push_back(closureTable.size());
			closureTable.push_back(target);
		}
		else {
			stateRow.push_back(index - closureTable.begin());
		}
	}
	transitionTable.push_back(stateRow);
}
void dfa::fillActionTable() {
	for (int i = 0;i < closureTable.size();i++) {
		unordered_set<int> states = closureTable[i];
		vector<int> ordered;
		for (int state : states) {
			ordered.push_back(state);
		}
		sort(ordered.begin(),ordered.end());
		for (int j = 0;j < ordered.size();j++) {
			if (nfa.action.find(ordered[j]) != nfa.action.end()) {
				actionTable[i] = nfa.action[ordered[j]];
				break;
			}
		}
	}
}
bool dfa::searchStack(stack<int> s, int i) {
	while (!s.empty()) {
		if (s.top() == i) {
			return true;
		}
		s.pop();
	}
	return false;
}


dfa minimize(dfa& d) {
	vector<vector<int>> initTable = d.transitionTable;
	vector<unordered_set<int>> divides;
	unordered_set<int> nonAcc;
	unordered_set<int> acc;
	for (int i = 0;i < initTable.size();i++) {
		if (d.actionTable.find(i) != d.actionTable.end()) {
			acc.insert(i);
		}
		else {
			nonAcc.insert(i);
		}
	}
	divides.push_back(nonAcc);
	divides.push_back(acc);
	int divide = 0;
	for (int i = 1;i < divides.size();i++) {
		divideAt(divides, i, d.actionTable);
	}
	while (divide < divides.size()) {
		divide = divides.size();
		for (int i = 0;i < divides.size();i++) {
			divideAt(divides, i, d.transitionTable);
		}
	}
	return deleteStatesDuplicated(d, divides);


}
dfa deleteStatesDuplicated(dfa& d, vector<unordered_set<int>>& divides) {
	unordered_map<int, int> newState;
	vector<int> remainedState;
	for (int i = 0;i < divides.size();i++) {
		for (int j : divides[i]) {
			newState[j] = i;
		}
		remainedState.push_back(*divides[i].begin());
	}
	vector<vector<int>> newTransitionTable;
	int newStart = newState[d.start];
	unordered_map<int, string> newActionTable;
	for (int state : remainedState) {
		vector<int> transition = d.transitionTable[state];
		vector<int> stateNew ;
		for (int i = 0;i < transition.size();i++) {
			if (transition[i] == -1) {
				stateNew.push_back(-1);
			}
			else {
				stateNew.push_back(newState[transition[i]]);
			}
		}
		newTransitionTable.push_back(stateNew);
		if (d.actionTable.find(state) != d.actionTable.end()) {
			newActionTable[newState[state]] = d.actionTable[state];
		}
	}
	return  dfa(newTransitionTable, newActionTable, newStart);

}
int divideAt(vector<unordered_set<int>>& divides,int index, unordered_map<int, string>& actionTable ) {
	unordered_set<int> states = divides[index];
	unordered_set<int> group1, group2;
	int first = *states.begin();
	string s1 = actionTable[first];
	for (int state : states) {
		if (actionTable[state].compare(s1)==0) {
			group1.insert(state);
		}
		else {
			group2.insert(state);
		}
	}
	if (group1.empty() || group2.empty()) {
		return -1;
	}
	divides[index] = group1;
	divides.push_back(group2);
	return divides.size();
}

int divideAt(vector<unordered_set<int>>& divides, int index, vector<vector<int>>& transitionTable) {
	unordered_set<int> states = divides[index];
	if (states.size() < 2) {
		return -1;
	}
	unordered_set<int> group1, group2;
	int first = *states.begin();
	vector<int> s1 = transitionTable[first];
	for (auto& i : states) {
		vector<int> s2 = transitionTable[i];
		if (equal(s1, s2,divides)) {
			group1.insert(i);
		}
		else {
			group2.insert(i);
		}

	}
	if (group1.empty() || group2.empty()) {
		return -1;
	}
	divides[index] = group1;
	divides.push_back(group2);
	return divides.size();
}

bool equal(vector<int> s1, vector<int> s2, vector<unordered_set<int>>& divides) {
	for (int i = 0;i < s1.size();i++) {
		if (getIndex(s1[i], divides) != getIndex(s2[i], divides)) {
			return false;
		}
	}
	return true;
}

int getIndex(int state, vector<unordered_set<int>>& divides) {
	if (state == -1)return -1;
	for (int i = 0;i < divides.size();i++) {
		if (divides[i].find(state) != divides[i].end()) {
			return i;
		}
	}
	return -1;
}

