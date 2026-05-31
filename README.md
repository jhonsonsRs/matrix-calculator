# Matrix Calculator

A matrix calculator built with C++ and Qt, developed as a university project.

## Features

- **Sum** — A + B
- **Subtraction** — A - B
- **Multiplication** — A × B
- **Scalar multiplication** — k × A
- **Transpose** — Aᵀ
- **Determinant** — det(A) via Gaussian Elimination

## Requirements

- Qt 6.x
- CMake 3.x
- GCC or Clang

## How to build

```bash
mkdir build && cd build
cmake ..
make
./MatrixCalculator
```

## How to use

1. Set the number of rows and columns using the spinboxes
2. Fill in the matrix values
3. Click the desired operation button
4. The result appears in the Result table or in the status label
