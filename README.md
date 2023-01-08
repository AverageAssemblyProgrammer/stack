# stack 

Warning!! stack is not complete yet

Stack is a stack-based programming language emulator

## Installation
```terminal
$ make
```

## Quick Start
```terminal
- ./stack sim
```

```terminal
- ./stack sim 
- 50 20 +
```

The code above pushes 50 and 20 onto the data stack and sums them up with `+` operation.

#### Stack Manipulation

| Name    | Signature        | Description                                                                                  |
| ---     | ---              | ---                                                                                          |
| `dup`   | `a -- a a`       | duplicate an element on top of the stack.                                                    |
| `swap`  | `a b -- b a`     | swap 2 elements on the top of the stack.                                                     |
| `drop`  | `a b -- a`       | drops the top element of the stack.                                                          |
| `print` | `a b -- a`       | print the element on top of the stack in a free form to stdout and remove it from the stack. |
| `rot`   | `a b c -- b c a` | rotate the top three stack elements.                                                         |
#### Arithmetic

| Name     | Signature                                        | Description                                                                                                              |
| ---      | ---                                              | ---                                                                                                                      |
| `+`      | `[a: int] [b: int] -- [a + b: int]`              | sums up two elements on the top of the stack.                                                                            |
| `-`      | `[a: int] [b: int] -- [a - b: int]`              | subtracts two elements on the top of the stack    

# TBD: 
Document Arithmetic operations 
