#pragma once

using namespace std;

class Tuple : public vector<string> {
public:
	Tuple() {}
	virtual ~Tuple() {}

    bool isJoinable(const Tuple &t, vector<pair<int, int> > &restrictions) const {
        for (const pair<int, int> &p : restrictions) {
            if (this->at(p.first) != t[p.second]) {
                return false;
            }
        }
        return true;
    }

    Tuple join(const Tuple &t, vector<pair<int, int> > &restrictions) const {
        Tuple joined = *this;
        for (unsigned int i = 0; i < t.size(); i++) {
            bool add = true;
            for (const pair<int, int> &p : restrictions) {
                if (i == p.second) {
                    add = false;
                    break;
                }
            }
            if (add) {
                joined.push_back(t[i]);
            }
        }
        return joined;
    }

    friend ostream& operator<< (ostream &out, const Tuple &r) {
        out << "( ";
        for (int i = 0; i < r.size(); i++) {
            out << r[i] << " ";
        }
        out << ")";
        return out;
    }
};
