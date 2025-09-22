# 1-CPSC323
Project 1 for CPSC323, a lexical analyzer using lexical conversions and syntax rules.

1) Lexical Conventions:  
- The lexical units of a program are identifiers, keywords, integers, reals, operators and other
separators.
- Blanks, tabs and newlines (collectively, "white space") as described below are ignored except as they serve to separate tokens.
- Some white space is required to separate otherwise adjacent identifiers, keywords, reals and integers.
- **Identifier** is a sequence of letters, digits, or “$”. However, the first character must be a letter. Upper and
lower cases are same.
- **Integer** is an unsigned decimal integer i.e., a sequence of decimal digits.
- **Real** is an optional integer followed by “.” and Integer, e.g., 123.00 or .001 but not 123.
- Some identifiers are reserved for use as keywords, and may not be used otherwise:  e.g., integer, if, else, fi, while, return, get, put etc.
Comments are enclosed in “ “ and should be entirely ignored by the LA, SA etc.

***Check PDF for more info***
