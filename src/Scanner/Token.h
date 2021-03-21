#pragma once

#include <iostream>
#include <string>
#include <map>

using namespace std;

enum TokType {
	COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, ADD, MULTIPLY,
	SCHEMES, FACTS, RULES, QUERIES,
	STRING, COMMENT, ID, EXPRESSION, EoF, UNDEFINED
};

class Token {
private:
	static map<TokType, string> typeString;

	TokType type_;
	string value_;
	int lineNum_;

public:
	Token(TokType type, string value, int lineNum) : type_(type), value_(value), lineNum_(lineNum) {}
	Token() {}
	virtual ~Token() {}

	TokType type() const { return type_; }
	string value() const { return value_; }
	int lineNum() const { return lineNum_; }

	friend ostream& operator<< (ostream& out, const Token& t) {
		out << "(" << typeString[t.type()] << ",\"" << t.value() << "\"," << t.lineNum() << ")";
		return out;
	}
};
