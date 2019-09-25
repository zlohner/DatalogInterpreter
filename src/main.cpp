#include <iostream>
#include <fstream>
#include "Scanner/Scanner.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

using namespace std;

int main(int argc, char* argv[]) {

	// input file
	ifstream inFile(argv[1]);
	streambuf* cinbuf = cin.rdbuf();
	// redirect cin to read from input file
	cin.rdbuf(inFile.rdbuf());
	if (!inFile.is_open())
		throw "ERROR: unable to open input file";

	// output file
	ofstream* outFile = NULL;
	streambuf* coutbuf = cout.rdbuf();
	if (argc > 2) {
		outFile = new ofstream(argv[2]);
		// redirect cout to write to output file
		cout.rdbuf(outFile->rdbuf());
	}

	try {
		// create interpreter objects
		Scanner s;
		Parser p;
		Interpreter i;

		// interpret input
		deque<Token>* tokens = s.scan(false);
		DatalogProgram* program = p.parse(tokens, false);
		i.interpret(program);
	} catch (string message) {
		cout << message << endl;
	}

	// make sure files are closed
	if (inFile.is_open())
		inFile.close();
	if (outFile != NULL && outFile->is_open())
		outFile->close();
	if (outFile != NULL)
		delete outFile;

	// redirect cin/cout buffers
	cin.rdbuf(cinbuf);
	cout.rdbuf(coutbuf);

	return 0;
}
