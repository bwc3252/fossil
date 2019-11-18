# fossil

The Fossil programming language interpreter

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
