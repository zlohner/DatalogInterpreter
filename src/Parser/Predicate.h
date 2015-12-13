#pragma once

#include <string>
#include <vector>

using namespace std;

class Predicate {
private:
	string name_;
	vector<Token> parameterList_;
	
public:
	Predicate(string name, vector<Token> parameterList) 
		: name_(name), parameterList_(parameterList) {}
	virtual ~Predicate() {}
	
	string name() const { return name_; }
	const vector<Token>* parameterList() const { return &parameterList_; }
	
	friend ostream& operator<< (ostream& out, const Predicate& p) {
		out << p.name() << "(" << p.parameterList()->at(0).value();
		for (unsigned int i = 1; i < p.parameterList()->size(); i++) {
			out << "," << p.parameterList()->at(i).value();
		}
		out << ")";
		return out;
	}
};