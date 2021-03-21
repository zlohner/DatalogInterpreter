#pragma once

#include <string>
#include <vector>
#include <set>

#include "Predicate.h"
#include "Rule.h"

using namespace std;

class DatalogProgram {
private:
	vector<Predicate> schemes_;
	vector<Predicate> facts_;
	vector<Rule> rules_;
	vector<Predicate> queries_;
	set<string> domain_;
	
public:
	DatalogProgram() {}
	virtual ~DatalogProgram() {}

	const vector<Predicate>* schemes() const { return &schemes_; }
	const vector<Predicate>* facts() const { return &facts_; }
	const vector<Rule>* rules() const { return &rules_; }
	const vector<Predicate>* queries() const { return &queries_; }
	const set<string>* domain() const { return &domain_; }

	void addScheme(Predicate newScheme) { schemes_.push_back(newScheme); }
	void addFact(Predicate newFact) { facts_.push_back(newFact); }
	void addRule(Rule newRule) { rules_.push_back(newRule); }
	void addQuery(Predicate newQuery) { queries_.push_back(newQuery); }
	void addDomain(string value) { domain_.insert(value); }

	friend ostream& operator<< (ostream& out, const DatalogProgram& program) {
		out << "Schemes(" << program.schemes()->size() << "):" << endl;
		for (unsigned int i = 0; i < program.schemes()->size(); i++)
			out << "  " << program.schemes()->at(i) << endl;
		out << "Facts(" << program.facts()->size() << "):" << endl;
		for (unsigned int i = 0; i < program.facts()->size(); i++)
			out << "  " << program.facts()->at(i) << "." << endl;
		out << "Rules(" << program.rules()->size() << "):" << endl;
		for (unsigned int i = 0; i < program.rules()->size(); i++)
			out << "  " << program.rules()->at(i) << "." << endl;
		out << "Queries(" << program.queries()->size() << "):" << endl;
		for (unsigned int i = 0; i < program.queries()->size(); i++)
			out << "  " << program.queries()->at(i) << "?" << endl;
		out << "Domain(" << program.domain()->size() << "):" << endl;
		for (string s : (*(program.domain())))
			out << "  " << s << endl;
		return out;
	}
};
