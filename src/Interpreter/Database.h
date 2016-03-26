#pragma once

#include <map>

#include "Relation.h"

using namespace std;

class Database {
private:
	map<string, Relation> relations_;

public:
	Database() {}
	~Database() {}

	map<string, Relation>* relations() { return &relations_; }
	Relation relation(string name) { return relations_[name]; }

	void addRelation(string name, const Relation &r) { relations_[name] = r; }
	void addTuple(string relationName, const Tuple &t) { relations_[relationName].addTuple(t); }
	void addTuples(string relationName, const set<Tuple> &newTuples) { relations_[relationName].addTuples(newTuples); }

	void clear() { relations_.clear(); }

	int size() {
		int size = 0;
		for (const pair<string, Relation> &p : relations_) {
			size += p.second.size();
		}
		return size;
	}

};
