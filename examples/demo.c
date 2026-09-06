/**
 * @file demo.c
 * @brief Demonstration of Binary Min/Max Heap and HeapSort.
 * @author i7modes
 * @license MIT
 */

#include "heap.h"
#include <stdio.h>

static void print_banner(const char *title)
{
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

int main(void)
{
    print_banner("1. Max-Heap Priority Queue & 2D ASCII Visualizer");
    Heap *max_heap = Heap_Create(HEAP_MAX, 8);
    int tasks[] = {4, 10, 3, 5, 1, 15, 8};
    size_t num_tasks = sizeof(tasks) / sizeof(tasks[0]);

    printf("Pushing tasks with priorities: 4, 10, 3, 5, 1, 15, 8...\n");
    for (size_t i = 0; i < num_tasks; i++)
    {
        Heap_Push(max_heap, tasks[i]);
    }

    printf("Max-Heap Array Storage: ");
    Heap_Print(max_heap, stdout);
    printf("Is valid Max-Heap invariant? %s\n", Heap_IsValid(max_heap) ? "YES" : "NO");

    printf("\n2D ASCII Tree Visualizer:\n");
    Heap_PrintTree(max_heap);

    print_banner("2. Extracting by Highest Priority (Max-Heap)");
    while (!Heap_IsEmpty(max_heap))
    {
        int priority = 0;
        Heap_Pop(max_heap, &priority);
        printf("  Processed task with priority: %d (Remaining size: %zu)\n",
               priority, Heap_Size(max_heap));
    }
    Heap_Destroy(&max_heap);

    print_banner("3. Min-Heap Demonstration (Shortest Job First)");
    Heap *min_heap = Heap_Create(HEAP_MIN, 8);
    printf("Pushing values: 4, 10, 3, 5, 1, 15, 8...\n");
    for (size_t i = 0; i < num_tasks; i++)
    {
        Heap_Push(min_heap, tasks[i]);
    }

    printf("Min-Heap Array Storage: ");
    Heap_Print(min_heap, stdout);
    printf("Is valid Min-Heap invariant? %s\n", Heap_IsValid(min_heap) ? "YES" : "NO");

    int min_val = 0;
    Heap_Peek(min_heap, &min_val);
    printf("Peek root (minimum element): %d\n", min_val);

    printf("Popping elements in ascending order: ");
    while (!Heap_IsEmpty(min_heap))
    {
        int val = 0;
        Heap_Pop(min_heap, &val);
        printf("%d ", val);
    }
    printf("\n");
    Heap_Destroy(&min_heap);

    print_banner("4. Floyd's Linear-Time O(n) Heap Construction");
    int raw_data[] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
    size_t raw_n = sizeof(raw_data) / sizeof(raw_data[0]);

    printf("Building Max-Heap from raw 10-element array in O(n) time...\n");
    Heap *built = Heap_Build(HEAP_MAX, raw_data, raw_n);
    Heap_Print(built, stdout);
    printf("Tree representation:\n");
    Heap_PrintTree(built);
    Heap_Destroy(&built);

    print_banner("5. In-Place HeapSort Demonstration");
    int sort_arr[] = {64, 25, 12, 22, 11, 90, 33, 47};
    size_t sort_n = sizeof(sort_arr) / sizeof(sort_arr[0]);

    printf("Unsorted array: ");
    for (size_t i = 0; i < sort_n; i++) printf("%d ", sort_arr[i]);
    printf("\n");

    Heap_Sort(sort_arr, sort_n, true);
    printf("Sorted (Ascending):  ");
    for (size_t i = 0; i < sort_n; i++) printf("%d ", sort_arr[i]);
    printf("\n");

    Heap_Sort(sort_arr, sort_n, false);
    printf("Sorted (Descending): ");
    for (size_t i = 0; i < sort_n; i++) printf("%d ", sort_arr[i]);
    printf("\n");

    print_banner("6. Legacy Academic API Execution");
    int B[MAX_SIZE + 1] = {0, 4, 10, 3, 5, 1};
    int length = 5;

    MaxHeapBuilding(B, length);
    printf("Legacy MaxHeapBuilding: root = %d\n", B[1]);

    Insert(B, &length, 15);
    Insert(B, &length, 8);
    printf("Legacy after insertions: root = %d, length = %d\n", B[1], length);

    int max_deleted = DeleteMax(B, &length);
    printf("Legacy DeleteMax: extracted = %d, new root = %d, length = %d\n",
           max_deleted, B[1], length);

    print_banner("7. Clean Memory Teardown");
    printf("All heap pointers safely freed with 0 memory leaks.\n");
    printf("\nHeap demonstrations completed successfully!\n");

    return 0;
}
