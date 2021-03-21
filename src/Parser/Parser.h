#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include "../Scanner/Token.h"
#include "DatalogProgram.h"

using namespace std;

class Parser {
private:
	enum PredicateType { SCHEME, FACT, RULE, QUERY };

	deque<Token>* tokens;
	DatalogProgram* program;
	bool parsingFact;

	Token match(TokType type) {
		Token t = tokens->front();
		if (t.type() == type)
			tokens->pop_front();
		else
			throw t;

		if (parsingFact && type == STRING)
			program->addDomain(t.value());

		return t;
	}

	void parseDatalogProgram() {
		match(SCHEMES);
		match(COLON);
		parseScheme();
		parseSchemeList();
		match(FACTS);
		match(COLON);
		parseFactList();
		match(RULES);
		match(COLON);
		parseRuleList();
		match(QUERIES);
		match(COLON);
		parseQuery();
		parseQueryList();
	}

	Predicate parsePredicate(PredicateType type) {
		vector<Token> parameterList;
		string name = match(ID).value();

		match(LEFT_PAREN);
		parameterList.push_back(parseParameter(type));
		parseParameterList(parameterList, type);
		match(RIGHT_PAREN);

		return Predicate(name, parameterList);
	}

	void parsePredicateList(vector<Predicate>& predList, PredicateType type) {
		if (tokens->front().type() == COMMA) {
			match(COMMA);
			predList.push_back(parsePredicate(type));
			parsePredicateList(predList, type);
		}
	}

	void parseScheme() {
		program->addScheme(parsePredicate(SCHEME));
	}

	void parseSchemeList() {
		if (tokens->front().type() == ID) {
			parseScheme();
			parseSchemeList();
		}
	}

	void parseFact() {
		parsingFact = true;
		program->addFact(parsePredicate(FACT));
		match(PERIOD);
		parsingFact = false;
	}

	void parseFactList() {
		if (tokens->front().type() == ID) {
			parseFact();
			parseFactList();
		}
	}

	void parseRule() {
		Predicate headPredicate = parsePredicate(RULE);
		vector<Predicate> predList;

		match(COLON_DASH);
		predList.push_back(parsePredicate(RULE));
		parsePredicateList(predList, RULE);
		match(PERIOD);

		program->addRule(Rule(headPredicate, predList));
	}

	void parseRuleList() {
		if (tokens->front().type() == ID) {
			parseRule();
			parseRuleList();
		}
	}

	void parseQuery() {
		program->addQuery(parsePredicate(QUERY));
		match(Q_MARK);
	}

	void parseQueryList() {
		if (tokens->front().type() == ID) {
			parseQuery();
			parseQueryList();
		}
	}

	Token parseParameter(PredicateType type) {
		switch(tokens->front().type()) {
			case ID:
				if (type == SCHEME || type == RULE || type == QUERY)
					return match(ID);
			case STRING:
				if (type == FACT || type == RULE || type == QUERY)
					return match(STRING);
			default: throw tokens->front();
		}
	}

	void parseParameterList(vector<Token>& parameterList, PredicateType type) {
		if (tokens->front().type() == COMMA) {
			match(COMMA);
			parameterList.push_back(parseParameter(type));
			parseParameterList(parameterList, type);
		}
	}

	string parseOperator() {
		if (tokens->front().type() == ADD)
			return match(ADD).value();
		else if (tokens->front().type() == MULTIPLY)
			return match(MULTIPLY).value();
		else throw tokens->front();
	}

public:
	Parser() : tokens(NULL), program(NULL), parsingFact(false) {}
	virtual ~Parser() {
		if (tokens != NULL)
			delete tokens;
		if (program != NULL)
			delete program;
	}

	DatalogProgram* parse(deque<Token>* tokens_in, bool print) {
		tokens = tokens_in;
		program = new DatalogProgram();

		try {
			parseDatalogProgram();
		}
		catch (Token t) {
			stringstream ss;
			ss << "Failure!\n  " << t;
			throw ss.str();
		}

		DatalogProgram* complete_program = program;

		if (print) cout << "Success!\n" << *complete_program;

		program = NULL;
		tokens = NULL;
		delete tokens_in;

		return complete_program;
	}
};
