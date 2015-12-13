#pragma once

#include <string>
#include <vector>

#include "Predicate.h"

using namespace std;

class Rule {
private:
	static int numRules;
	Predicate headPredicate_;
	vector<Predicate> predList_;
	int ruleNum_;

public:
	Rule(Predicate headPredicate, vector<Predicate> predList)
		: headPredicate_(headPredicate), predList_(predList), ruleNum_(numRules++) {}
	virtual ~Rule() {}

	const Predicate* headPredicate() const { return &headPredicate_; }
	const vector<Predicate>* predList() const { return &predList_; }
	int ruleNum() const { return ruleNum_; }

	friend ostream& operator<< (ostream& out, const Rule& r) {
		out << (*r.headPredicate()) << " :- " << r.predList()->at(0);
		for (unsigned int i = 1; i < r.predList()->size(); i++) {
			out << "," << r.predList()->at(i);
		}
		return out;
	}
};

int Rule::numRules = 0;
