# Tests for ChocoPy compiler
This directory contains testcases and testcase generator for each component of the compiler(lexer, parser, etc.). `grammarinator` automized testcase generation coming soon.


```python
grammarinator-process grammar.g4 -o testers 
grammarinator-generate  -l ChocoPyUnlexer.py -p ChocoPyUnparser.py  -n 100 -o tests/ -j 1 --sys-recursion-limit 20900 -d 10 
```
## Directories
```
in  -> input source files for compilers
out -> reference lexer output
```

## Usage
Put your source files in `in`, run `make` and you're good to go.

## Testing Method

`python3 ./duipai.py --pa [num]`