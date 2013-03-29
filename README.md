#A WFF interpreter
##Checks for validity, consitency and creates the logic table
<br>

There are two files **parse.c** and **wff.c** of which the latter is the actual file.
**parse.c** is a simple recursive descent parser for checking if a given expression
is a wff, and it exists just for educational purposes.

**wff.c** extends on parse.c, and does the actual job of interpreting.

---
####Supported logical connectives:
* And ^
* Or v
* Not ~
* Implies >
* Equivalent =


####Variable names: [A-Z]
####Output format: CSV

##TODO
- [x] Standard csv format
- [ ] Prettify on display. 
- [ ] Strings as variable names 
- [ ] Something else(?)
