#pragma once

#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <cctype>
#include "Token.h"

using namespace std;

class Scanner {
private:
	static map<string, TokType> keywords;

	char curr;
	int lineNum;

	/* scanString
	 * reads a string token from the file
	 * @return: the string token
	 * @throws: error message if EoF is found before end of string
	 */
	Token scanString() {
		int startLineNum = lineNum;
		string value;
		value += curr;

		bool end = false;
		while (!end) {
			if (!cin.get(curr))
				return Token(UNDEFINED, value, startLineNum); // throw "Input Error on line " + to_string(startLineNum);

			if (curr == '\n')
				lineNum++;
			else if (curr == '\'') {
				if (cin.peek() == '\'') {
					value += curr;
					cin.get(curr);
				}
				else end = true;
			}

			value += curr;
		}
		return Token(STRING, value, startLineNum);
	}

	/* scanComment
	 * reads a comment token from the file
	 * @return: the comment token
	 * @throws: error message if EoF is found before end of comment
	 */
	Token scanComment() {
		int startLineNum = lineNum;
		string value;
		value += curr;

		if (cin.peek() == '|') { // Multiline
			cin.get(curr);
			value += curr;

			bool end = false;
			while (!end) {
				if (!cin.get(curr))
					return Token(UNDEFINED, value, startLineNum); // throw "Input Error on line " + to_string(startLineNum);

				if (curr == '\n')
					lineNum++;
				else if (curr == '|' && cin.peek() == '#')
					end = true;

				value += curr;
			}

			cin.get(curr);
			value += curr;
		}
		else { // Single line
			while (cin.peek() != '\n') {
				cin.get(curr);
				value += curr;
			}
		}

		return Token(COMMENT, value, startLineNum);
	}

	/* scanID
	 * reads an ID token from the file
	 * If it is a keyword, a keyword token is created instead
	 * @return: the ID or Keyword token
	 */
	Token scanID() {
		string value;
		value += curr;
		while (isalnum(cin.peek())) {
			cin.get(curr);
			value += curr;
		}
		if (keywords.count(value))
			return Token(keywords[value], value, lineNum);
		else
			return Token(ID, value, lineNum);
	}

	Token getToken(bool& addToken) {
		Token t;
		switch(curr) {
			case ',': t = Token(COMMA, ",", lineNum); break;
			case '.': t = Token(PERIOD, ".", lineNum); break;
			case '?': t = Token(Q_MARK, "?", lineNum); break;
			case '(': t = Token(LEFT_PAREN, "(", lineNum); break;
			case ')': t = Token(RIGHT_PAREN, ")", lineNum); break;
			case ':':
				if (cin.peek() == '-') {
					cin.get(curr);
					t = Token(COLON_DASH, ":-", lineNum);
				}
				else t = Token(COLON, ":", lineNum);
				break;
			case '+': t = Token(ADD, "+", lineNum); break;
			case '*': t = Token(MULTIPLY, "*", lineNum); break;
			case '\'': t = scanString(); break;
			case '#': t = scanComment(); addToken = false; break;
			default:
				if (isalpha(curr)) {
					t = scanID();
				} else if (isspace(curr)) {
					if (curr == '\n') lineNum++;
					addToken = false;
				} else {
					string symbol;
					symbol += curr;
					t = Token(UNDEFINED, symbol, lineNum); // throw "Input Error on line " + to_string(lineNum);
				}
			break;
		}
		return t;
	}

public:
	Scanner() {}
	virtual ~Scanner() {}

	/* scan
	 * performs lexical analysis on a Datalog file, returning a list of tokens found in the file
	 * @return: a deque representing the list of tokens
	 */
	deque<Token>* scan(bool print) {
		deque<Token>* tokens = new deque<Token>();
		lineNum = 1;

		while (cin.get(curr)) {
			bool addToken = true;

			Token t = getToken(addToken);

			if (addToken) tokens->push_back(t);
			if (print) cout << t << endl;
		}
		Token end = Token(EoF, "", lineNum);
		tokens->push_back(end);

		if (print) {
			cout << end << endl;
			cout << "Total Tokens = " << tokens->size() << endl;
		}

		return tokens;
	}
};
