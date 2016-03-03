#include "Token.h"

map<TokType, string> Token::typeString = {
	{COMMA, "COMMA"},
	{PERIOD, "PERIOD"},
	{Q_MARK, "Q_MARK"},
	{LEFT_PAREN, "LEFT_PAREN"},
	{RIGHT_PAREN, "RIGHT_PAREN"},
	{COLON, "COLON"},
	{COLON_DASH, "COLON_DASH"},
	{ADD, "ADD"},
	{MULTIPLY, "MULTIPLY"},
	{SCHEMES, "SCHEMES"},
	{FACTS, "FACTS"},
	{RULES, "RULES"},
	{QUERIES, "QUERIES"},
	{STRING, "STRING"},
	{COMMENT, "COMMENT"},
	{ID, "ID"},
	{EoF, "EOF"},
	{UNDEFINED, "UNDEFINED"}
};
