# Binary Min/Max Heap & Priority Queue in C

[![CI](https://github.com/i7modes/Heaps/actions/workflows/ci.yml/badge.svg)](https://github.com/i7modes/Heaps/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Standard](https://img.shields.io/badge/C-C99-00599C.svg?logo=c)](https://en.wikipedia.org/wiki/C99)
[![Memory Safety](https://img.shields.io/badge/Valgrind-0%20Leaks-brightgreen.svg)]()

A high-performance, production-grade **Binary Heap and Priority Queue** implementation in ISO C99. Supporting both **Max-Heap** and **Min-Heap** ordering variants, linear-time bottom-up construction (Floyd's algorithm), and in-place $\mathcal{O}(n \log n)$ **HeapSort**.

---

## Features

- **Dual-Mode Priority Variants**: Seamlessly create either a `HEAP_MAX` (largest element at root) or `HEAP_MIN` (smallest element at root).
- **Linear-Time Construction**: Bottom-up heap building via Floyd's algorithm (`Heap_Build`) runs in $\mathcal{O}(n)$ time rather than $\mathcal{O}(n \log n)$.
- **Dynamic Geometric Resizing**: Transparently doubles backing array capacity on overflow, supporting arbitrary element volumes.
- **In-Place HeapSort**: Cache-friendly sorting (`Heap_Sort`) running in $\mathcal{O}(n \log n)$ time with zero additional heap allocations ($\mathcal{O}(1)$ auxiliary space).
- **2D ASCII Visualizer**: Terminal tree visualizer (`Heap_PrintTree`) displaying complete binary tree hierarchies.
- **Invariant Validator**: Runtime structural checker (`Heap_IsValid`) ensuring parental dominance holds across all nodes.
- **Zero Memory Leaks**: Verified clean under Valgrind and AddressSanitizer (ASan) on every commit.
- **Legacy Compatibility Layer**: Seamless drop-in compatibility for academic coursework function signatures (`MaxHeapify`, `MaxHeapBuilding`, `Insert`, `DeleteMax`, `MAX_SIZE`).

---

## Asymptotic Complexity

| Operation | Time Complexity | Space Complexity |
| :--- | :---: | :---: |
| **Peek Root (`Heap_Peek`)** | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| **Insert / Push (`Heap_Push`)** | $\mathcal{O}(\log n)$ (Amortized) | $\mathcal{O}(1)$ |
| **Extract Root (`Heap_Pop`)** | $\mathcal{O}(\log n)$ | $\mathcal{O}(1)$ |
| **Build Heap (`Heap_Build`)** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |
| **In-Place HeapSort (`Heap_Sort`)** | $\mathcal{O}(n \log n)$ | $\mathcal{O}(1)$ |
| **Contains Search (`Heap_Contains`)** | $\mathcal{O}(n)$ | $\mathcal{O}(1)$ |

---

## Architecture & Array Representation

A binary heap is a **complete binary tree** compactly stored in a contiguous array without explicit child pointers:

```
            [15] (index 1)
           /    \
        [10]    [8]  (indices 2, 3)
       /   \    /  \
     [5]   [4] [3] [1] (indices 4, 5, 6, 7)

Backing Array (1-Indexed):
+-------+----+----+----+----+----+----+----+
| Index |  1 |  2 |  3 |  4 |  5 |  6 |  7 |
+-------+----+----+----+----+----+----+----+
| Value | 15 | 10 |  8 |  5 |  4 |  3 |  1 |
+-------+----+----+----+----+----+----+----+

Formulas for node at index i:
  - Parent:      i / 2
  - Left Child:  2 * i
  - Right Child: 2 * i + 1
```

---

## Project Structure

```
Heaps/
├── .github/
│   └── workflows/
│       └── ci.yml              # Multi-platform CI (Valgrind, ASan, MinGW)
├── examples/
│   └── demo.c                 # Visual demo (Priority queues, HeapSort, visualizer)
├── include/
│   └── heap.h                 # Public API & legacy compatibility wrappers
├── src/
│   └── heap.c                 # Binary heap engine & HeapSort implementation
├── tests/
│   └── test_heap.c            # Unit test suite covering all invariants
├── .gitignore
├── compile_flags.txt
├── LICENSE                    # MIT License
├── Makefile                   # Cross-platform build system
└── README.md
```

---

## Getting Started

### Prerequisites

- GCC or Clang with C99 support
- GNU Make (or `mingw32-make` on Windows)
- Valgrind (optional, for Linux memory checks)

### Building and Running

```bash
# Build test runner and demo binaries
make all

# Run the automated unit test suite
make check

# Run the interactive demonstration
make demo

# Clean build artifacts
make clean
```

---

## API Reference

### Lifecycle & Memory
```c
Heap* Heap_Create(HeapType type, size_t initial_capacity);
Heap* Heap_Build(HeapType type, const int *array, size_t n);
void  Heap_Clear(Heap *heap);
void  Heap_Destroy(Heap **heap_ptr);
```

### Core Operations
```c
bool     Heap_Push(Heap *heap, int value);
bool     Heap_Pop(Heap *heap, int *out_value);
bool     Heap_Peek(const Heap *heap, int *out_value);
bool     Heap_Contains(const Heap *heap, int value);
size_t   Heap_Size(const Heap *heap);
size_t   Heap_Capacity(const Heap *heap);
bool     Heap_IsEmpty(const Heap *heap);
HeapType Heap_GetType(const Heap *heap);
bool     Heap_IsValid(const Heap *heap);
```

### Algorithms & Visualization
```c
void Heap_Sort(int *array, size_t n, bool ascending);
void Heap_Print(const Heap *heap, FILE *stream);
void Heap_PrintTree(const Heap *heap);
```

### Legacy Compatibility Layer
```c
void swap(int *a, int *b);
void MaxHeapify(int B[], int s, int length);
void MaxHeapBuilding(int B[], int length);
void Insert(int B[], int *length, int value);
int  DeleteMax(int B[], int *length);
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
