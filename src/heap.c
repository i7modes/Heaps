/**
 * @file heap.c
 * @brief Implementation of Binary Min/Max Heap and Priority Queue in C.
 * @author i7modes
 * @license MIT
 */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_INITIAL_CAPACITY 16

/* -------------------------------------------------------------------------- */
/*                         Internal Helpers & Comparison                      */
/* -------------------------------------------------------------------------- */

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static inline bool has_higher_priority(int a, int b, HeapType type)
{
    return (type == HEAP_MAX) ? (a > b) : (a < b);
}

static void sift_up(Heap *heap, size_t index)
{
    size_t i = index;
    while (i > 1)
    {
        size_t parent = i / 2;
        if (has_higher_priority(heap->data[i], heap->data[parent], heap->type))
        {
            swap(&heap->data[i], &heap->data[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }
}

static void sift_down(Heap *heap, size_t index)
{
    size_t i = index;
    size_t n = heap->size;

    while (2 * i <= n)
    {
        size_t left = 2 * i;
        size_t right = 2 * i + 1;
        size_t best = i;

        if (left <= n && has_higher_priority(heap->data[left], heap->data[best], heap->type))
        {
            best = left;
        }

        if (right <= n && has_higher_priority(heap->data[right], heap->data[best], heap->type))
        {
            best = right;
        }

        if (best != i)
        {
            swap(&heap->data[i], &heap->data[best]);
            i = best;
        }
        else
        {
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

Heap* Heap_Create(HeapType type, size_t initial_capacity)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL)
    {
        return NULL;
    }

    size_t cap = (initial_capacity < 4) ? DEFAULT_INITIAL_CAPACITY : initial_capacity;
    heap->data = (int *)malloc((cap + 1) * sizeof(int));
    if (heap->data == NULL)
    {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = cap;
    heap->type = type;

    return heap;
}

Heap* Heap_Build(HeapType type, const int *array, size_t n)
{
    if (array == NULL && n > 0)
    {
        return NULL;
    }

    size_t cap = (n < DEFAULT_INITIAL_CAPACITY) ? DEFAULT_INITIAL_CAPACITY : n;
    Heap *heap = Heap_Create(type, cap);
    if (heap == NULL)
    {
        return NULL;
    }

    if (n > 0)
    {
        memcpy(&heap->data[1], array, n * sizeof(int));
        heap->size = n;

        /* Floyd's bottom-up heap construction in linear O(n) time */
        for (size_t k = n / 2; k >= 1; k--)
        {
            sift_down(heap, k);
        }
    }

    return heap;
}

void Heap_Clear(Heap *heap)
{
    if (heap != NULL)
    {
        heap->size = 0;
    }
}

void Heap_Destroy(Heap **heap_ptr)
{
    if (heap_ptr == NULL || *heap_ptr == NULL)
    {
        return;
    }

    Heap *heap = *heap_ptr;
    if (heap->data != NULL)
    {
        free(heap->data);
        heap->data = NULL;
    }

    free(heap);
    *heap_ptr = NULL;
}

/* -------------------------------------------------------------------------- */
/*                              Core Operations                               */
/* -------------------------------------------------------------------------- */

bool Heap_Push(Heap *heap, int value)
{
    if (heap == NULL)
    {
        return false;
    }

    if (heap->size >= heap->capacity)
    {
        size_t new_cap = heap->capacity * 2;
        int *new_data = (int *)realloc(heap->data, (new_cap + 1) * sizeof(int));
        if (new_data == NULL)
        {
            return false;
        }
        heap->data = new_data;
        heap->capacity = new_cap;
    }

    heap->size++;
    heap->data[heap->size] = value;
    sift_up(heap, heap->size);

    return true;
}

bool Heap_Pop(Heap *heap, int *out_value)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    int root = heap->data[1];
    heap->data[1] = heap->data[heap->size];
    heap->size--;

    if (heap->size > 1)
    {
        sift_down(heap, 1);
    }

    if (out_value != NULL)
    {
        *out_value = root;
    }

    return true;
}

bool Heap_Peek(const Heap *heap, int *out_value)
{
    if (heap == NULL || heap->size == 0 || out_value == NULL)
    {
        return false;
    }

    *out_value = heap->data[1];
    return true;
}

bool Heap_Contains(const Heap *heap, int value)
{
    if (heap == NULL || heap->size == 0)
    {
        return false;
    }

    for (size_t i = 1; i <= heap->size; i++)
    {
        if (heap->data[i] == value)
        {
            return true;
        }
    }
    return false;
}

/* -------------------------------------------------------------------------- */
/*                               Inspection & Stats                           */
/* -------------------------------------------------------------------------- */

size_t Heap_Size(const Heap *heap)
{
    return (heap != NULL) ? heap->size : 0;
}

size_t Heap_Capacity(const Heap *heap)
{
    return (heap != NULL) ? heap->capacity : 0;
}

bool Heap_IsEmpty(const Heap *heap)
{
    return (heap == NULL || heap->size == 0);
}

HeapType Heap_GetType(const Heap *heap)
{
    return (heap != NULL) ? heap->type : HEAP_MAX;
}

bool Heap_IsValid(const Heap *heap)
{
    if (heap == NULL || heap->size == 0)
    {
        return true;
    }

    for (size_t i = 1; i <= heap->size / 2; i++)
    {
        size_t left = 2 * i;
        size_t right = 2 * i + 1;

        if (left <= heap->size && has_higher_priority(heap->data[left], heap->data[i], heap->type))
        {
            return false;
        }

        if (right <= heap->size && has_higher_priority(heap->data[right], heap->data[i], heap->type))
        {
            return false;
        }
    }

    return true;
}

void Heap_Print(const Heap *heap, FILE *stream)
{
    if (heap == NULL || stream == NULL)
    {
        return;
    }

    fprintf(stream, "%s-Heap [Size: %zu, Cap: %zu]: [ ",
            (heap->type == HEAP_MAX) ? "Max" : "Min",
            heap->size, heap->capacity);

    for (size_t i = 1; i <= heap->size; i++)
    {
        fprintf(stream, "%d ", heap->data[i]);
    }
    fprintf(stream, "]\n");
}

static void print_tree_util(const Heap *heap, size_t index, int space)
{
    if (index > heap->size)
    {
        return;
    }

    space += 8;

    /* Right child */
    print_tree_util(heap, 2 * index + 1, space);

    /* Current node */
    printf("\n");
    for (int i = 8; i < space; i++)
    {
        printf(" ");
    }
    printf("[%d]\n", heap->data[index]);

    /* Left child */
    print_tree_util(heap, 2 * index, space);
}

void Heap_PrintTree(const Heap *heap)
{
    if (heap == NULL || heap->size == 0)
    {
        printf("(empty heap)\n");
        return;
    }
    print_tree_util(heap, 1, 0);
}

/* -------------------------------------------------------------------------- */
/*                               Heap Algorithms                              */
/* -------------------------------------------------------------------------- */

static void heapify_0based(int *arr, size_t n, size_t i, bool max_heap)
{
    size_t largest_or_smallest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (max_heap)
    {
        if (left < n && arr[left] > arr[largest_or_smallest])
        {
            largest_or_smallest = left;
        }
        if (right < n && arr[right] > arr[largest_or_smallest])
        {
            largest_or_smallest = right;
        }
    }
    else
    {
        if (left < n && arr[left] < arr[largest_or_smallest])
        {
            largest_or_smallest = left;
        }
        if (right < n && arr[right] < arr[largest_or_smallest])
        {
            largest_or_smallest = right;
        }
    }

    if (largest_or_smallest != i)
    {
        swap(&arr[i], &arr[largest_or_smallest]);
        heapify_0based(arr, n, largest_or_smallest, max_heap);
    }
}

void Heap_Sort(int *array, size_t n, bool ascending)
{
    if (array == NULL || n <= 1)
    {
        return;
    }

    /*
     * Ascending sort uses a Max-Heap (swapping max element to the end).
     * Descending sort uses a Min-Heap (swapping min element to the end).
     */
    bool use_max = ascending;

    /* Build heap bottom-up in O(n) */
    for (size_t k = n / 2; k > 0; k--)
    {
        heapify_0based(array, n, k - 1, use_max);
    }

    /* Extract elements one by one from heap in O(n log n) */
    for (size_t end = n - 1; end > 0; end--)
    {
        swap(&array[0], &array[end]);
        heapify_0based(array, end, 0, use_max);
    }
}

/* -------------------------------------------------------------------------- */
/*                       Legacy API Compatibility Layer                       */
/* -------------------------------------------------------------------------- */

void MaxHeapify(int B[], int s, int length)
{
    int left = 2 * s;
    int right = 2 * s + 1;
    int largest = s;

    if (left <= length && B[left] > B[largest])
    {
        largest = left;
    }

    if (right <= length && B[right] > B[largest])
    {
        largest = right;
    }

    if (largest != s)
    {
        swap(&B[s], &B[largest]);
        MaxHeapify(B, largest, length);
    }
}

void MaxHeapBuilding(int B[], int length)
{
    for (int k = length / 2; k >= 1; k--)
    {
        MaxHeapify(B, k, length);
    }
}

void Insert(int B[], int *length, int value)
{
    if (length == NULL || *length >= MAX_SIZE)
    {
        printf("Heap overflow\n");
        return;
    }

    (*length)++;
    int i = *length;
    B[i] = value;

    while (i > 1 && B[i / 2] < B[i])
    {
        swap(&B[i], &B[i / 2]);
        i = i / 2;
    }
}

int DeleteMax(int B[], int *length)
{
    if (length == NULL || *length < 1)
    {
        printf("Heap underflow\n");
        return -1;
    }

    int max = B[1];
    B[1] = B[*length];
    (*length)--;

    if (*length > 0)
    {
        MaxHeapify(B, 1, *length);
    }

    return max;
}
