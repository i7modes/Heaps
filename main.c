#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100 // Assuming the maximum size of the heap

int main()
{
    int B[MAX_SIZE + 1] = {0, 4, 10, 3, 5, 1}; // Index 0 is not used
    int length = 5;

    MaxHeapBuilding(B, length);

    // Print the initial heap
    printf("Initial max heap: ");
    for (int i = 1; i <= length; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    // Insert elements into the heap
    Insert(B, &length, 15);
    Insert(B, &length, 8);

    // Print the heap after insertion
    printf("Heap after insertions: ");
    for (int i = 1; i <= length; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    // Delete the maximum element
    int max = DeleteMax(B, &length);
    printf("Deleted max element: %d\n", max);

    // Print the heap after deletion
    printf("Heap after deletion: ");
    for (int i = 1; i <= length; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    return 0;
}


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void MaxHeapify(int B[], int s, int length) {
    int left = 2 * s;
    int right = 2 * s + 1;
    int largest;

    if (left <= length && B[left] > B[s]) {
        largest = left;
    } else {
        largest = s;
    }

    if (right <= length && B[right] > B[largest]) {
        largest = right;
    }

    if (largest != s) {
        swap(&B[s], &B[largest]);
        MaxHeapify(B, largest, length);
    }
}

void MaxHeapBuilding(int B[], int length) {
    for (int k = length / 2; k >= 1; k--) {
        MaxHeapify(B, k, length);
    }
}

void Insert(int B[], int *length, int value) {
    if (*length >= MAX_SIZE) {
        printf("Heap overflow\n");
        return;
    }
    (*length)++;
    int i = *length;
    B[i] = value;
    while (i > 1 && B[i / 2] < B[i]) {
        swap(&B[i], &B[i / 2]);
        i = i / 2;
    }
}

int DeleteMax(int B[], int *length) {
    if (*length < 1) {
        printf("Heap underflow\n");
        return -1;
    }
    int max = B[1];
    B[1] = B[*length];
    (*length)--;
    MaxHeapify(B, 1, *length);
    return max;
}
