#/bin/bash
datestring=$(date +%Y-%m-%d.%H:%M:%S)
sourcefiles=$(find ./src -name "*.h" -or -name "*.cpp")
name="Zachary_Lohner"
progtitle="DatalogInterpreter"
filename="zips/"$datestring"_"$name"_"$progtitle".zip"

numzips=$(ls -1 ./zips/ | wc -l)

if [ "$numzips" -ge 10 ]; then
	oldestzip=$(ls zips | sort -n | head -1)
fi

rm zips/"$oldestzip"

zip "$filename" $sourcefiles
scp "$filename" cs236ta@schizo.cs.byu.edu:passoff/ZacTestArea 
