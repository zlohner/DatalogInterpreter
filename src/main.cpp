#include <iostream>
#include <fstream>
#include "Scanner/Scanner.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

using namespace std;

int main(int argc, char* argv[]) {
	
	// Input and Output File
	ifstream inFile(argv[1]);
    // ifstream outFile(argv[2]);
	
	// save cin & cout buffer
	streambuf* cinbuf = cin.rdbuf(); 
    // streambuf* coutbuf = cout.rdbuf();
	
	// create Interpreter objects
	Scanner s;
	Parser p;
	Interpreter i;
	
	try {
		// check if file is valid
		if (!inFile.is_open())
			throw "ERROR: unable to open input file";
		
		// redirect cin to input file
		cin.rdbuf(inFile.rdbuf());
        // cout.rdbuf(outFile.rdbuf());
		
		// interpret input
		deque<Token>* tokens = s.scan(false);
		DatalogProgram* program = p.parse(tokens, false);
		i.interpret(program);
	}
	catch (string message) {
		cout << message << endl;
	}
	
	// make sure files are closed
	if (inFile.is_open())
		inFile.close();
    // if (outFile.is_open())
    //     outFile.close();
	
	// redirect cin buffer
	cin.rdbuf(cinbuf);
    // cout.rdbuf(coutbuf);
	
	return 0;
}
