#/bin/bash

NORM="\033[0m"
HEAD="\033[35m"
INFO="\033[34m"
PASS="\033[32m"
FAIL="\033[31m"

function testComplexity() {

	echo "${BLUE}TESTING COMPLEXITY ... ${NORM}"

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
		echo "${FAIL}FAILED COMPLEXITY${NORM}"
		return 1
	else
		echo "${PASS}PASSED COMPLEXITY${NORM}"
		return 0
	fi
}

function test() {
	inFile="test/Key/p$1/in$1$2.txt"
	outFile="test/outputs/out$2.txt"
	expectedFile="test/Key/p$1/out$1$2.txt"
	diffFile="test/diff.txt"

	echo "TEST $2 ... "
	time build/DatalogInterpreter.o "$inFile" > "$outFile"
	diff -b "$outFile" "$expectedFile" > "$diffFile"

	diffOutput=""

	while read -r line; do
		diffOutput+="$line"
	done < "$diffFile"

	if [ -z "$diffOutput" ]; then
		echo "${PASS}PASSED${NORM}"
	else
		echo "${FAIL}FAILED${NORM}"
		vimdiff -b "$outFile" "$expectedFile"
	fi
}

function runTests() {
	if testComplexity; then
		echo ""

		if [ -e "build/DatalogInterpreter.o" ]; then
			echo "${INFO}CLEANING ... ${NORM}"
			make clean
			echo ""
		fi

		echo "${INFO}COMPILING ... ${NORM}"
		make
		echo ""

		if [ -e "build/DatalogInterpreter.o" ]; then
			echo "${INFO}RUNNING TESTS ... ${NORM}"
			lab=5
			fileNums=(1 2 3 4 5 6 7 8 9)
			for i in ${fileNums[*]}; do
				test $lab "$i"
			done
		else
			echo "${FAIL}COMPILATION FAILED${NORM}"
		fi
	fi
}

echo ""
echo "${HEAD}TEST BUILD${NORM}"
echo ""
runTests
echo "${NORM}"
