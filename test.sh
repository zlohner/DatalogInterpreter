#/bin/bash

function testComplexity() {
	
	echo -n -e "\033[34mTESTING COMPLEXITY ... \033[0m"
	
	pmccabeFile="test/pmccabe.txt"
	rm "$pmccabeFile"
	
	for file in "`find ./src -name "*.h"`"; do
		pmccabe $file >> "$pmccabeFile"
	done

	for file in "`find ./src -name "*.cpp"`"; do
		pmccabe $file >> "$pmccabeFile"
	done

	
	failedComplexity=false;
	
    while read -r line; do
		functionstats=($line)
		complexity=${functionstats[0]}
		if [ "$complexity" -gt 8 ]; then
			echo "$line";
			failedComplexity=true;
		fi
	done < "$pmccabeFile"

	if $failedComplexity; then
		echo -e "\033[31mFAILED COMPLEXITY\033[0m"
		return 1
	else
		echo -e "\033[32mPASSED COMPLEXITY\033[0m"
		return 0
	fi
}

function test() {
	inFile="test/Key/Project$1/passoffInput$1$2.txt"
	outFile="test/outputs/out$2.txt"
	expectedFile="test/Key/Project$1/passoffAnswer$1$2.txt"
	diffFile="test/diff.txt"
	
	echo -n "TEST $2 ... "
	time build/DatalogInterpreter "$inFile" > "$outFile"
	diff -b "$outFile" "$expectedFile" > "$diffFile"
	
	diffOutput=""
	
	while read -r line; do
	    diffOutput+="$line"
	done < "$diffFile"
	
	if [ -z "$diffOutput" ]; then
		echo -e "\033[32mPASSED\033[0m"
	else
		echo -e "\033[31mFAILED\033[0m"
		vimdiff -b "$outFile" "$expectedFile"
	fi
}

function runTests() {
	if testComplexity; then
		echo ""

		if [ -e "build/DatalogInterpreter" ]; then
			echo -e "\033[34mCLEANING ... \033[0m"
			make clean
			echo ""
		fi

		echo -e "\033[34mCOMPILING ... \033[0m"
		make
		echo ""

		echo -e "\033[34mRUNNING TESTS ... \033[0m"
		if [ -e "build/DatalogInterpreter" ]; then
			fileNums=(1 2 3 4 5 6 7 8 9 0)
			for i in ${fileNums[*]}; do
				test $1 "$i"
			done
		fi
	fi
}

if [ -z $1 ]; then
	echo -e "\033[31mPLEASE SPECIFY PROJECT NUMBER\033[0m"
	exit 1
fi

echo ""
echo -e "\033[35mTEST BUILD\033[0m"
echo ""
runTests $1
echo -e "\033[0m"
