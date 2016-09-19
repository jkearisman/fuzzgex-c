# Fuzzgex (working name)

Calculates the number of insertions, deletions, and substitutions necessary for a string to match a regular expression.

Includes a parser that generates an abstract syntax tree.
From the AST, the edit distance is calculated

this is a work in progress

Syntax: (some of these are unimplemented)
An expr is one of:
char
expr, '*'
expr, '+'
expr, '?'
expr, expr
expr, '{', int, '}'
expr, '{', int, ',', int, '}'
expr,'|',expr
'(', expr, ')'


TODO:
fully implement all kinds of expression (in parser and solver)
cache results for speed
enable searching for low-edit substrings within a larger text
documentation

