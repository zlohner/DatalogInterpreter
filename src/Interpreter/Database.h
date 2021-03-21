#pragma once

#include <vector>
#include <map>

#include "Relation.h"

using namespace std;

class Database {
private:
	vector<string> relationNames_;
	map<string, Relation> relations_;

public:
	Database() {}
	~Database() {}

	Relation relation(string name) { return relations_[name]; }

	void addRelation(string name, const Relation &r) { relationNames_.push_back(name); relations_[name] = r; }
	void addTuple(string relationName, const Tuple &t) { relations_[relationName].addTuple(t); }
	void addTuples(string relationName, const set<Tuple> &newTuples) { relations_[relationName].addTuples(newTuples); }

	void clear() { relationNames_.clear(); relations_.clear(); }

	int size() {
		int size = 0;
		for (string name : relationNames_) {
			size += relations_[name].size();
		}
		return size;
	}

};
