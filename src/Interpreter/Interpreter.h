#pragma once

#include <utility>
#include <stack>

#include "../Parser/DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Tuple.h"
#include "Graph.h"

using namespace std;

class Interpreter {
private:
	Database db;

	Relation query(const Predicate& query) {
		Relation r = db.relation(query.name());

		// Select
		vector<pair<int,string> > values;

		for (unsigned int i = 0; i < query.parameterList()->size(); i++)
			if (query.parameterList()->at(i).type() == STRING)
				values.push_back(pair<int, string>(i, query.parameterList()->at(i).value()));
		r.select(values);

		// Project
		vector<int> indices;
		for (unsigned int i = 0; i < query.parameterList()->size(); i++)
			if (query.parameterList()->at(i).type() == ID)
				indices.push_back(i);
		r.project(indices);

		// Rename
		r.rename(*(query.parameterList()));

		return r;
	}

	vector<int> projectDelta(const Predicate* headPredicate, const vector<string> &schema) {
		vector<int> delta;
		for (unsigned int i = 0; i < headPredicate->parameterList()->size(); i++) {
			for (unsigned int j = 0; j < schema.size(); j++) {
				if (headPredicate->parameterList()->at(i).value() == schema[j]) {
					delta.push_back(j);
					break;
				}
			}
		}
		return delta;
	}

	void evaluateRule(int i, DatalogProgram* program) {
		const Predicate* headPredicate = program->rules()->at(i).headPredicate();
		const vector<Predicate>* predList = program->rules()->at(i).predList();

		Relation answer = query(predList->at(0));
		for (unsigned int i = 1; i < predList->size(); i++) {
			answer.join(query(predList->at(i)));
		}
		vector<int> delta = projectDelta(headPredicate, *(answer.schema()));
		answer.project(delta);
		db.addTuples(headPredicate->name(), *(answer.tuples()));
	}

	void evaluateRuleSet(const set<int>& SCC, DatalogProgram* program, bool simple) {
		int passes = 0;
		int dbsize = 0;
		do {
			passes++;
			dbsize = db.size();
			for (int i : SCC) {
				evaluateRule(i, program);
			}
		} while (dbsize < db.size() && !simple);
		cout << passes << " passes: ";
		bool first = true;
		for (int i : SCC) {
			if (!first) cout << ",";
			else first = false;
			cout << "R" << i;
		}
		cout << endl;
	}

	SCCGraph dependencyGraph(DatalogProgram* program) {
		SCCGraph g;
		for (unsigned int i = 0; i < program->rules()->size(); i++) {
			g.addNode();
		}
		for (const Rule &r : (*program->rules())) {
			for (const Predicate &p : (*r.predList())) {
				for (const Rule &produces : (*program->rules())) {
					if (produces.headPredicate()->name() == p.name()) {
						g.addEdge(r.ruleNum(), produces.ruleNum());
					}
				}
			}
		}
		return g;
	}

	vector<set<int> > findSCCs(DatalogProgram* program) {
		// Dependency Graph
		SCCGraph d = dependencyGraph(program);
		cout << "Dependency Graph" << endl;
		cout << d << endl;

		// Reverse Graph
		PostorderGraph reverse = d.reverse();

		// DFS (arbitrary order)
		stack<int> order;
		for (int i = d.nodes() - 1; i >= 0; i--) order.push(i);
		reverse.DFSForest(order);

		// DFS (reverse postorder number order)
		d.DFSForest(*reverse.postorderStack());
		return *d.SCCs();
	}

	bool dependsOnSelf(int i, DatalogProgram* program) {
		for (const Predicate &p : (*program->rules()->at(i).predList())) {
			if (p.name() == program->rules()->at(i).headPredicate()->name())
				return true;
		}
		return false;
	}

public:
	Interpreter() {}
	virtual ~Interpreter() {}

	void interpret(DatalogProgram* program) {

		// Schemes
		for (Predicate s : *(program->schemes()))
			db.addRelation(s.name(), Relation(*(s.parameterList())));

		// Facts
		for (Predicate f : *(program->facts())) {
			Tuple tup;
			for (Token t : *(f.parameterList())) tup.push_back(t.value());
			db.addTuple(f.name(), tup);
		}

		// Rules

		// All Rules (Change to SCCs)
		vector<set<int> > SCCs = findSCCs(program);

		cout << "Rule Evaluation" << endl;
		for (set<int>& SCC : SCCs) {
			evaluateRuleSet(SCC, program, SCC.size() == 1 && !dependsOnSelf(*SCC.begin(), program));
		}
		cout << endl;

		// Queries
		cout << "Query Evaluation" << endl;
		for (Predicate q : *(program->queries())) {
			Relation r = query(q);

			// Print
			cout << q << "?";
			if (r.size() > 0) cout << " Yes(" << r.size() << ")";
			else cout << " No";
			cout << r << endl;
		}

		db.clear();
		delete program;
	}

};
