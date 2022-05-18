#!/bin/bash
# install the requirements
pip3 install -r requirements.txt

# generate the recursion test for parser
if [ -f recursion ]; then 
    rm -rf recursion 
fi
mkdir recursion
grammarinator-process ./grammar.g4 -o ./  && grammarinator-generate -r program  -l ./ChocoPyUnlexer.py -p ./ChocoPyUnparser.py  -n 100 -o recursion/ -j 1 --sys-recursion-limit 20900 -d 10
python3 ./indent.py