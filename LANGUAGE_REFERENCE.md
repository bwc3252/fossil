# Fossil language reference

This document provides full, detailed descriptions of Fossil syntax and behavior; it will be updated incrementally as new features are added.

## Overview of language features and design

Fossil is a statically-typed, imperative, C-like language.
The syntax is simple, and should be familiar to anyone who knows a C-like language.
For now, the only types that have been implemented are `int`s and `bool`s.
Semicolons can be used to indicate the end of a statement, but are not required: line breaks are replaced with semicolons during parsing.
Whitespace is ignored (aside from line breaks indicating statment endings).

## Program structure

All fossil code exists inside functions (for now).
Functions are defined as follows:
```
fn [function name]([type] [parameter], ...) => [return type] {
    ...
}
```
Or if the function has no return:
```
fn [function name]([type] [parameter], ...) {
    ...
}
```
If the function takes no parameters:
```
fn [function name]() ...
```

The entry point for a program is the main function, which takes no parameters and has no return:
```
fn main() {
    ...
}
```

Functions are called using the standard syntax:
```
fn add_one(int i) => int {
    return i + 1
}

fn main() {
    int j = 0
    j = add_one(j)
}
```

## Declaring and assigning variables

Variables must be declared with their type.
A value must be assigned during declaration -- it is impossible for a variable to exist without an explicitly-assigned value.
Declarations are straightforward:
```
[type] [variable name] = [expression]
```
A variable that has been previously declared can then be assigned to:
```
[variable name] = [expression]
```

## `print` statements

`print` statement syntax is similar to Python 2.
To print the value of a variable `var`:
```
print var
```

## Control flow

Currently, the control flow features that have been implemented are:
- `while` loops
- `if`, `elif`, and `else` statements

`while` loop syntax is:
```
while ([condition]) {
    ...
}
```

`if`, `elif`, and `else` statements are similar:
```
if ([first condition]) {
    ...
}
elif ([second condition]) {
    ...
}
else {
    ...
}
```

## Future additions

- `for` loops
- Additional primitive types:
  - `float`
  - `str`
- Basic composite data types, such as arrays
- C-like structs
- An import system
