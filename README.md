# Fossil

The Fossil programming language interpreter

## Language features

- C-like syntax (but semicolons are not required)
- Static typing
- Types:
  - int
  - float
  - str
  - bool
  - fn
  - array
  - struct

See the [language reference](https://github.com/bwc3252/fossil/blob/master/LANGUAGE_REFERENCE.md) for a full specification of all language features.

## Example code

```
fn fib(int n) => int {
    if (n <= 1) {
        return n
    }
    return fib(n - 1) + fib(n - 2)
}

fn main() {
    print fib(20)
}
```

## Implementation details

See `notes/grammar.txt` for a specification of the grammar.
The source code is first tokenized in `src/lexer.c`.
The `tokenize(char *str)` function takes the entire source code as a string; it then constructs a linked list of tokens by iteratively splitting the string (i.e. we start with a single node and split that node's text until we're left with a linked list where each node is a token).
Comments are stripped at this point and "implied" semicolons are inserted where appropriate.

Next, a recursive descent parser consumes the linked list produced by the lexer and produces an AST as defined by the grammar specification; the grammar is specified in a way that should:
- be unambiguous, i.e. there is only one set of rules that could produce a given piece of source code
- handle operator precedence automatically

The interpreter will now evaluate the AST, using the `main()` function as its entry point.
The interpreter will require a data structure to map variable names to values while handling scope.
To accomplish this I am thinking of building a stack that contains hashmaps -- each time a function is called we push a new symbol table to the stack, which will hold local variables.
When that function returns, we will pop that table from the stack and add its return value to the symbol table that is now the top of the stack, i.e. the calling function.

## Short-term plan

To make development easier, I plan to implement a "bare bones" version of the language first.
The only types I will implement to begin with are int and bool, and only a single main function will be used.
A very minimal `print` will be implemented for testing.

## Medium/long-term plan

With this minimal version of the language working, I will begin to expand it.
The first major addition will be to fully support functions; after that, I will implement the remaining primitive types, as well as for loops.
Next, I will implement arrays, which must (a) have a fixed length, and (b) contain only primitive types, and then structs, which are the basic method of creating composite types in fossil.
Finally, I will design and implement an import system to allow programs to be built from multiple source files.

These additions will complete the core language.
The long-term goal is to build a standard library in fossil, which will provide functions for file I/O, better printing, and some useful composite types (e.g. a hashmap, a more general "list" type, etc.).
At this point, fossil should be a fully-functional, minimal scripting language.

I don't have a particular use in mind for the language (this is primarily a learning project), but it should be well-suited to quick-and-dirty command line tools, etc.
