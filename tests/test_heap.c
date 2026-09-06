/**
 * @file test_heap.c
 * @brief Automated unit test suite for Binary Min/Max Heap and HeapSort.
 * @author i7modes
 * @license MIT
 */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int total_tests = 0;
static int passed_tests = 0;

#define TEST_ASSERT(expr, msg) do { \
    total_tests++; \
    if (expr) { \
        passed_tests++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
    } \
} while (0)

static void test_empty_heap_safety(void)
{
    printf("\n--- Test: Empty Heap Safety ---\n");
    Heap *heap = Heap_Create(HEAP_MAX, 0);
    TEST_ASSERT(heap != NULL, "Heap_Create succeeds");
    TEST_ASSERT(Heap_Size(heap) == 0, "Initial size is 0");
    TEST_ASSERT(Heap_Capacity(heap) >= 4, "Initial capacity is at least 4");
    TEST_ASSERT(Heap_IsEmpty(heap) == true, "Heap_IsEmpty returns true");
    TEST_ASSERT(Heap_IsValid(heap) == true, "Empty heap is valid");
    TEST_ASSERT(Heap_GetType(heap) == HEAP_MAX, "Type is HEAP_MAX");

    int val = 0;
    TEST_ASSERT(Heap_Pop(heap, &val) == false, "Pop on empty heap returns false");
    TEST_ASSERT(Heap_Peek(heap, &val) == false, "Peek on empty heap returns false");
    TEST_ASSERT(Heap_Contains(heap, 42) == false, "Contains returns false on empty heap");

    Heap_Clear(heap);
    TEST_ASSERT(Heap_Size(heap) == 0, "Clear on empty heap is safe");

    Heap_Destroy(&heap);
    TEST_ASSERT(heap == NULL, "Heap_Destroy sets pointer to NULL");

    Heap_Destroy(&heap); // Safe double destroy
    TEST_ASSERT(heap == NULL, "Double destroy on NULL pointer is safe");
}

static void test_max_heap_operations(void)
{
    printf("\n--- Test: Max-Heap Push, Pop & Priority Invariant ---\n");
    Heap *heap = Heap_Create(HEAP_MAX, 8);

    int values[] = {4, 10, 3, 5, 1, 15, 8};
    size_t n = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < n; i++)
    {
        TEST_ASSERT(Heap_Push(heap, values[i]) == true, "Push value into Max-Heap");
        TEST_ASSERT(Heap_IsValid(heap) == true, "Max-Heap property holds after push");
    }

    TEST_ASSERT(Heap_Size(heap) == n, "Size matches number of pushed items");
    TEST_ASSERT(Heap_Contains(heap, 15) == true, "Contains 15");
    TEST_ASSERT(Heap_Contains(heap, 99) == false, "Does not contain 99");

    int peeked = 0;
    TEST_ASSERT(Heap_Peek(heap, &peeked) == true && peeked == 15, "Peek returns root (15)");

    // Elements must pop in descending order
    int expected[] = {15, 10, 8, 5, 4, 3, 1};
    bool order_correct = true;
    for (size_t i = 0; i < n; i++)
    {
        int popped = 0;
        if (!Heap_Pop(heap, &popped) || popped != expected[i])
        {
            order_correct = false;
            break;
        }
        if (!Heap_IsValid(heap))
        {
            order_correct = false;
            break;
        }
    }
    TEST_ASSERT(order_correct == true, "Max-Heap pops in strictly descending priority order");
    TEST_ASSERT(Heap_IsEmpty(heap) == true, "Heap is empty after popping all elements");

    Heap_Destroy(&heap);
}

static void test_min_heap_operations(void)
{
    printf("\n--- Test: Min-Heap Push, Pop & Priority Invariant ---\n");
    Heap *heap = Heap_Create(HEAP_MIN, 8);

    int values[] = {4, 10, 3, 5, 1, 15, 8};
    size_t n = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < n; i++)
    {
        TEST_ASSERT(Heap_Push(heap, values[i]) == true, "Push value into Min-Heap");
        TEST_ASSERT(Heap_IsValid(heap) == true, "Min-Heap property holds after push");
    }

    int peeked = 0;
    TEST_ASSERT(Heap_Peek(heap, &peeked) == true && peeked == 1, "Peek returns min root (1)");

    // Elements must pop in ascending order
    int expected[] = {1, 3, 4, 5, 8, 10, 15};
    bool order_correct = true;
    for (size_t i = 0; i < n; i++)
    {
        int popped = 0;
        if (!Heap_Pop(heap, &popped) || popped != expected[i])
        {
            order_correct = false;
            break;
        }
    }
    TEST_ASSERT(order_correct == true, "Min-Heap pops in strictly ascending priority order");
    TEST_ASSERT(Heap_IsEmpty(heap) == true, "Heap is empty after popping all elements");

    Heap_Destroy(&heap);
}

static void test_linear_time_heap_build(void)
{
    printf("\n--- Test: Floyd's O(n) Heap Build Algorithm ---\n");
    int raw[] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
    size_t n = sizeof(raw) / sizeof(raw[0]);

    Heap *max_h = Heap_Build(HEAP_MAX, raw, n);
    TEST_ASSERT(max_h != NULL, "Heap_Build Max-Heap created");
    TEST_ASSERT(Heap_Size(max_h) == n, "Size matches input array");
    TEST_ASSERT(Heap_IsValid(max_h) == true, "Floyd build satisfies Max-Heap invariant");
    int max_val = 0;
    Heap_Peek(max_h, &max_val);
    TEST_ASSERT(max_val == 16, "Max element 16 is at root");
    Heap_Destroy(&max_h);

    Heap *min_h = Heap_Build(HEAP_MIN, raw, n);
    TEST_ASSERT(min_h != NULL, "Heap_Build Min-Heap created");
    TEST_ASSERT(Heap_IsValid(min_h) == true, "Floyd build satisfies Min-Heap invariant");
    int min_val = 0;
    Heap_Peek(min_h, &min_val);
    TEST_ASSERT(min_val == 1, "Min element 1 is at root");
    Heap_Destroy(&min_h);
}

static void test_dynamic_resizing(void)
{
    printf("\n--- Test: Dynamic Reallocation on High Element Volume ---\n");
    Heap *heap = Heap_Create(HEAP_MAX, 4); // small initial capacity
    size_t initial_cap = Heap_Capacity(heap);

    for (int i = 1; i <= 100; i++)
    {
        Heap_Push(heap, i);
    }

    TEST_ASSERT(Heap_Size(heap) == 100, "100 elements inserted");
    TEST_ASSERT(Heap_Capacity(heap) > initial_cap, "Capacity reallocated dynamically");
    TEST_ASSERT(Heap_IsValid(heap) == true, "Heap property intact after multiple reallocations");

    int root = 0;
    Heap_Peek(heap, &root);
    TEST_ASSERT(root == 100, "Root is largest element (100)");

    Heap_Destroy(&heap);
}

static void test_in_place_heapsort(void)
{
    printf("\n--- Test: In-Place HeapSort (Ascending & Descending) ---\n");
    int arr[] = {29, -5, 12, 0, 44, -18, 12, 7, 3, 99, 1};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    // Ascending sort
    Heap_Sort(arr, n, true);
    bool is_asc = true;
    for (size_t i = 0; i < n - 1; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            is_asc = false;
            break;
        }
    }
    TEST_ASSERT(is_asc == true, "Heap_Sort accurately sorts array in ascending order");

    // Descending sort
    Heap_Sort(arr, n, false);
    bool is_desc = true;
    for (size_t i = 0; i < n - 1; i++)
    {
        if (arr[i] < arr[i + 1])
        {
            is_desc = false;
            break;
        }
    }
    TEST_ASSERT(is_desc == true, "Heap_Sort accurately sorts array in descending order");
}

static void test_legacy_coursework_api(void)
{
    printf("\n--- Test: Legacy Academic API Compatibility ---\n");
    int B[MAX_SIZE + 1] = {0, 4, 10, 3, 5, 1};
    int length = 5;

    MaxHeapBuilding(B, length);
    TEST_ASSERT(B[1] == 10, "Legacy MaxHeapBuilding put 10 at root");

    Insert(B, &length, 15);
    Insert(B, &length, 8);
    TEST_ASSERT(length == 7, "Length incremented to 7 after insertions");
    TEST_ASSERT(B[1] == 15, "Root is 15 after inserting 15");

    int max = DeleteMax(B, &length);
    TEST_ASSERT(max == 15, "DeleteMax returned 15");
    TEST_ASSERT(length == 6, "Length decremented to 6");
    TEST_ASSERT(B[1] == 10, "New root is 10 after DeleteMax");
}

int main(void)
{
    printf("========================================\n");
    printf("  Binary Min/Max Heap Automated Tests\n");
    printf("========================================\n");

    test_empty_heap_safety();
    test_max_heap_operations();
    test_min_heap_operations();
    test_linear_time_heap_build();
    test_dynamic_resizing();
    test_in_place_heapsort();
    test_legacy_coursework_api();

    printf("\n========================================\n");
    printf("  Results: %d/%d assertions passed\n", passed_tests, total_tests);
    printf("========================================\n");

    return (passed_tests == total_tests) ? 0 : 1;
}
