#include "integrate_nfa.h"


Integratednfa::Integratednfa(vector<pair<string, string>>& regs) {
	transitionTable.push_back(nfa::initStateRow());
	for (auto p : regs) {
		integrate(regExpToNFA(p.first),p.second);
	}
}
void Integratednfa::integrate(nfa n, string act) {
	nfa::addTransition(transitionTable.at(0), 128, transitionTable.size());
	vector<vector<unordered_set<int>*>*> table = n.increasedTable(transitionTable.size());
	transitionTable.insert(transitionTable.end(), table.begin(), table.end());
	action.insert({ transitionTable.size() - 1,act });
}