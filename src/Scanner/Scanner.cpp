#include "Scanner.h"

map<string, TokType> Scanner::keywords = { 
	{"Schemes", SCHEMES},
	{"Facts", FACTS},
	{"Rules", RULES},
	{"Queries", QUERIES}
};