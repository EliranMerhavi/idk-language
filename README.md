
# the IDK programming language

this interpeter was created as an assignment at school

## basic info about the language

### data types
1. int - a 64 bit integer
2. float - a 64 bit float
3. bool - can contains true or false
4. string - an array of characters 
5. array - like a python list can contain multiple data types at the same time
6. object - similiar to an object in javascript but every value of a key value pair have a strict type and cannot be changed 
	```
	let obj: object = { 'e': false };
	obj['e'] = "hello";
	?? will throw a runtime error
	```
## syntax
the syntax of the language is a bit similar to typescript.
here is the syntax for some basic statements and expression:

#### comments
```	
	?? one line comment
	## multi line
		comment ##

```

#### variable declaration
```
	let mutable_var: int = 5;
	const immutable_var: string = "hello world";

	## immutable_var = "something"; will throw an error (cant change const variables)
```

#### function declaration
if the functions is not void then it must return a value that its datatype is the same
as the return type. 
```
function func_name(const arg0: int, let arg1: float) -> int
{
	## body of the function ##
	return ## expression ##;
}

function void_function(const arg0: int, let arg1: float) -> void
{
	## body of the function ##
}
```
#### if / else 
```
	if (## expression ##) 
	{

	} 
	else ?? else block can be omitted.
	{

	}
```
#### loops
the language supports the 'break' and 'continue' keywords inside loops. in case 
someone neeed a reminder:
* continue - skip the rest of the statements in the current block and jumps to the start of the 
			 loop's block. 
* break - exits the current loop

here are the supported loops in the language:
##### while loop
```
	while (## expression ##)
	{

	}
```
#### for loop
```
	for (let i: int = 0; i < 10; i = i + 1) {
		## statements ##
	}
```
### expressions

here are the supported operators in the language:
* the operators are ordered by precedence
1.  ~   - bitwise negate
2.  !   - logical negate
3.  \-   - subtraction
4.  \+   - addition
5.  \*   - multiplication
6.  /   - division
7.  ==  - equal
8.  !=  - not equal
9.  =   - assignment
10. ^   - bitwise xor
11. |   - bitwise or
12. &   - bitwise and
13. &&  - logical and
14. ||  - logical or
15. \>   - bigger than
16. <   - less than
17. <=  - less than equal
18. \>=	- bigger than equal
19. <<  - bitwise shift left
20. \>\>  - bitewise shift right

#### exmaples of expressions

```
	(1 + 5) << 2 + 4 == 384 ?? expression is equal to the bool value true
```
	"hello " * 3 ?? expression equal to "hello hello hello "
```
	arr[0] = 5 ?? assignment of the value 5 to first variable of arr 
```

## usage
compile the source code and then run the exe in terminal
#### modes
- -h for help mode
- -e \<filepath> for execution a file
