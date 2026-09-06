/**
 * @file heap.h
 * @brief High-performance Binary Min/Max Heap and Priority Queue in C.
 * @author i7modes
 * @license MIT
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Heap ordering invariant type.
 */
typedef enum {
    HEAP_MAX = 0,   /**< Max-Heap: parent >= children (root is maximum) */
    HEAP_MIN = 1    /**< Min-Heap: parent <= children (root is minimum) */
} HeapType;

/**
 * @brief Binary Heap structure.
 * Backed by a 1-indexed dynamic array for textbook index arithmetic:
 *   Left child:  2 * i
 *   Right child: 2 * i + 1
 *   Parent:      i / 2
 */
typedef struct Heap {
    int *data;          /**< Backing storage array (allocated size: capacity + 1) */
    size_t size;        /**< Current number of elements stored */
    size_t capacity;    /**< Maximum element capacity before reallocation */
    HeapType type;      /**< Heap ordering variant (HEAP_MAX or HEAP_MIN) */
} Heap;

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Creates an empty dynamic binary heap.
 * @param type HEAP_MAX for Max-Heap or HEAP_MIN for Min-Heap.
 * @param initial_capacity Initial capacity (defaults to 16 if 0).
 * @return Pointer to new Heap, or NULL on allocation failure.
 */
Heap* Heap_Create(HeapType type, size_t initial_capacity);

/**
 * @brief Builds a binary heap in linear O(n) time using Floyd's algorithm.
 * @param type HEAP_MAX or HEAP_MIN.
 * @param array Source array of integers.
 * @param n Number of elements in source array.
 * @return Pointer to new Heap, or NULL on failure.
 */
Heap* Heap_Build(HeapType type, const int *array, size_t n);

/**
 * @brief Clears all elements from the heap without freeing the heap struct.
 * @param heap Pointer to the heap.
 */
void Heap_Clear(Heap *heap);

/**
 * @brief Deallocates the heap and its backing storage, setting *heap_ptr to NULL.
 * @param heap_ptr Pointer to the heap pointer variable.
 */
void Heap_Destroy(Heap **heap_ptr);

/* -------------------------------------------------------------------------- */
/*                              Core Operations                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Inserts an element into the heap (percolate up).
 * Automatically doubles capacity if the array is full. Runs in O(log n) time.
 * @param heap Pointer to the heap.
 * @param value Value to insert.
 * @return true on success, false on memory failure or NULL heap.
 */
bool Heap_Push(Heap *heap, int value);

/**
 * @brief Extracts and removes the root element (maximum or minimum).
 * Restores heap invariant via percolate down. Runs in O(log n) time.
 * @param heap Pointer to the heap.
 * @param out_value Pointer where the extracted value is written (can be NULL).
 * @return true if an element was extracted, false if heap is empty or NULL.
 */
bool Heap_Pop(Heap *heap, int *out_value);

/**
 * @brief Returns the root element without removing it in O(1) time.
 * @param heap Pointer to the heap.
 * @param out_value Pointer where the root value is written.
 * @return true if root exists, false if heap is empty or NULL.
 */
bool Heap_Peek(const Heap *heap, int *out_value);

/**
 * @brief Checks if the heap contains a specific value in O(n) time.
 * @param heap Pointer to the heap.
 * @param value Value to find.
 * @return true if found, false otherwise.
 */
bool Heap_Contains(const Heap *heap, int value);

/* -------------------------------------------------------------------------- */
/*                               Inspection & Stats                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Returns the number of elements in the heap.
 */
size_t Heap_Size(const Heap *heap);

/**
 * @brief Returns the current allocated capacity.
 */
size_t Heap_Capacity(const Heap *heap);

/**
 * @brief Checks if the heap is empty.
 */
bool Heap_IsEmpty(const Heap *heap);

/**
 * @brief Returns the heap type (HEAP_MAX or HEAP_MIN).
 */
HeapType Heap_GetType(const Heap *heap);

/**
 * @brief Validates that the entire array strictly satisfies the heap property.
 * @return true if valid, false if corrupted or violated.
 */
bool Heap_IsValid(const Heap *heap);

/**
 * @brief Prints the heap elements in array order: [ 10 8 9 3 5 ]
 */
void Heap_Print(const Heap *heap, FILE *stream);

/**
 * @brief Prints a 2D ASCII horizontal representation of the binary heap tree.
 */
void Heap_PrintTree(const Heap *heap);

/* -------------------------------------------------------------------------- */
/*                               Heap Algorithms                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Sorts an array of integers in-place using HeapSort in O(n log n) time
 * and O(1) auxiliary space.
 * @param array Array of integers to sort.
 * @param n Number of elements.
 * @param ascending If true, sorts ascending (1, 2, 3..); if false, descending.
 */
void Heap_Sort(int *array, size_t n, bool ascending);

/* -------------------------------------------------------------------------- */
/*                       Legacy API Compatibility Layer                       */
/* -------------------------------------------------------------------------- */

#define MAX_SIZE 100

void swap(int *a, int *b);
void MaxHeapify(int B[], int s, int length);
void MaxHeapBuilding(int B[], int length);
void Insert(int B[], int *length, int value);
int  DeleteMax(int B[], int *length);

#ifdef __cplusplus
}
#endif

#endif /* HEAP_H */
