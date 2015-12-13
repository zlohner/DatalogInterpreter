#pragma once

#include <vector>

#include "Tuple.h"

using namespace std;

class Relation {
private:
	vector<string> schema_;
	set<Tuple> tuples_;

public:
	Relation() {}
	Relation(const vector<string>& schema) : schema_(schema) {}
	Relation(const vector<Token>& schemaTokens) { setSchema(schemaTokens); }
	Relation(const vector<string>& schema, const set<Tuple>& tuples) : schema_(schema), tuples_(tuples) {}
	virtual ~Relation() {}

	const vector<string>* schema() const { return &schema_; }
	const set<Tuple>* tuples() const { return &tuples_; }
	int size() const { return tuples_.size(); }

	void setSchema(const vector<string>& schema) { schema_ = schema; }
	void setSchema(const vector<Token>& schemaTokens) {
		schema_.clear();
		for (Token t : schemaTokens) schema_.push_back(t.value());
	}
	void setTuples(const set<Tuple>& tuples) { tuples_ = tuples; }
	void addTuple(const Tuple& t) { tuples_.insert(t); }
	void addTuples(const set<Tuple>& newTuples) { for(Tuple t : newTuples) addTuple(t); }

	void selectEqual(const vector<pair<int, int> > &equals) {
        set<Tuple> newTuples;
		for (const Tuple &t : tuples_) {
            bool add = true;
            for (unsigned int i = 0; i < equals.size(); i++) {
                if (t[equals[i].first] != t[equals[i].second]) {
                    add = false;
					break;
				}
			}
            if (add) newTuples.insert(t);
		}
        setTuples(newTuples);
	}

	void select(const vector<pair<int, string> > &values) {
        set<Tuple> newTuples;
		for (const Tuple &t : tuples_) {
            bool add = true;
            for (unsigned int i = 0; i < values.size(); i++) {
                if (t[values[i].first] != values[i].second) {
                    add = false;
					break;
				}
			}
            if (add) newTuples.insert(t);
		}
        setTuples(newTuples);
	}

	void project(const vector<int> &indices) {
        // Schema
        vector<string> newSchema;
		for (unsigned int i = 0; i < indices.size(); i++)
            newSchema.push_back(schema_[indices[i]]);
        setSchema(newSchema);

        // Tuples
		set<Tuple> newTuples;
		for (const Tuple &t : tuples_) {
            Tuple newT;
			for (unsigned int i = 0; i < indices.size(); i++)
                newT.push_back(t[indices[i]]);
			newTuples.insert(newT);
		}
		setTuples(newTuples);
	}

	void rename(const vector<Token> &params) {
		int idCounter = 0;
		for (const Token &t : params) {
			if (t.type() == ID) {
				schema_[idCounter] = t.value();
				idCounter++;
			}
		}

        vector<pair<int, int> > equals;
		for (unsigned int i = 0; i < schema_.size(); i++) {
			for (unsigned int j = i + 1; j < schema_.size(); j++) {
				if (schema_[i] == schema_[j])
					equals.push_back(pair<int, int>(i, j));
			}
		}
        selectEqual(equals);
	}

    vector<pair<int, int> > findRestrictions(const Relation& r) {
        vector<pair<int, int> > restrictions;
        for (unsigned int i = 0; i < this->schema()->size(); i++) {
            for (unsigned int j = 0; j < r.schema()->size(); j++) {
                if (this->schema()->at(i) == r.schema()->at(j)) {
                   restrictions.push_back(pair<int, int>(i, j));
                }
            }
        }
        return restrictions;
    }

    vector<string> joinSchema(const Relation &r, const vector<pair<int, int> > &restrictions) {
        vector<string> newSchema = *(this->schema());
        for (unsigned int i = 0; i < r.schema()->size(); i++) {
            bool add = true;
            for (const pair<int, int> &p : restrictions) {
                if (i == p.second) {
                    add = false;
                    break;
                }
            }
            if (add) {
                newSchema.push_back(r.schema()->at(i));
            }
        }
        return newSchema;
    }

    void join(const Relation &r) {
        Relation joined;

        vector<pair<int, int> > restrictions = findRestrictions(r);

        setSchema(joinSchema(r, restrictions));

        set<Tuple> joinedTuples;
        for (const Tuple &t1 : *(this->tuples())) {
            for (const Tuple &t2 : *(r.tuples())) {
                if (t1.isJoinable(t2, restrictions)) {
                    joinedTuples.insert(t1.join(t2, restrictions));
                }
            }
        }

        setTuples(joinedTuples);
    }

	friend ostream& operator<< (ostream &out, const Relation &r) {
		if (r.schema()->size() > 0) {
			set<string> uniqueVals;
			vector<int> printVals;

			for (unsigned int i = 0; i < r.schema()->size(); i++) {
				if (!uniqueVals.count(r.schema()->at(i))) {
					uniqueVals.insert(r.schema()->at(i));
					printVals.push_back(i);
				}
			}

			for (set<Tuple>::iterator it = r.tuples()->begin(); it != r.tuples()->end(); it++) {
					out << endl << "  " << r.schema()->at(0) << "=" << (*it)[0];
					for (unsigned int i = 1; i < printVals.size(); i++)
						out << ", " << r.schema()->at(printVals[i]) << "=" << (*it)[printVals[i]];
			}
		}

		return out;
	}

};
