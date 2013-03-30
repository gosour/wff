# WFF Interpreter
## A Well Formed Formula(WFF) interpreter


parse.c is a simple recursive descent parser for checking if a given expression
is a wff


wff.c extends on parse.c, and does the actual job of interpreting, by creating parse trees.


-e expression:		   Where expression is the actual expression
					   expression:


					   1.variable names [A-Z]
					   2.logical connectives: ^(and), v(or), ~, =(<->equivalent) ,>(->)(implies)


[-p]:				   For printing the table in csv format in stdout.
					   Pipe the output to column. Example:
					   ```
						wff -e 'AvB' | column -t -s ','
					   ```


[-o filename]:		   For output to file 


##TODO
- [x] Standard csv format
- [ ] Prettify on display. 
- [ ] Strings as variable names 
- [ ] Something else(?)
