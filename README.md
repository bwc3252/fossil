# fossil

The Fossil programming language compiler

## Language features

- C-like syntax (but semicolons are not required)
- Garbage collection
- Static typing
- Types:
  - int
  - float
  - str
  - bool
  - fn
  - array
  - struct
- LLVM backend produces assembly code for target platform

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

## To do

- [x] tokenization
- [ ] strip comments
- [ ] categorize tokens by type
- [ ] generate AST from tokens
- [ ] convert AST to LLVM IR
- [ ] pass LLVM IR to LLVM, compile to executable
- [ ] error handling (generate helpful messages)
- [ ] add support for structs
- [ ] add support for arrays
- [ ] design and implement import system
- [ ] implement basic standard library in Fossil
