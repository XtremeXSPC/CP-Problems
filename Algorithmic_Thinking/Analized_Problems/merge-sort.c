/**
 * @file Problem_ecna05b.c
 * @author Costantino Lombardi
 * @brief Implementation of MergeSort algorithm.
 * This is a recursive solution.
 * @version 0.1
 * @date 2025-05-21
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//===------------------------ Functions Prototypes ----------------------------===//
void merge(int arr[], int left, int mid, int right);
void mergeSort(int arr[], int left, int right);
bool isSorted(int arr[], int size);
void printArray(int arr[], int size);
void generateRandomArray(int arr[], int size, int max_val);

//===------------------------ Functions Definitions ---------------------------===//
// Merge two subarrays
void merge(int arr[], int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Create temp arrays
  int* L = (int*)malloc(n1 * sizeof(int));
  int* R = (int*)malloc(n2 * sizeof(int));

  // Copy data to temp arrays
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  // Merge the temp arrays back
  i = 0;
  j = 0;
  k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy remaining elements
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  free(L);
  free(R);
}

// MergeSort implementation
void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }
}

// Function to check if array is sorted
bool isSorted(int arr[], int size) {
  for (int i = 1; i < size; i++) {
    if (arr[i] < arr[i - 1])
      return false;
  }
  return true;
}

// Function to print array
void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

// Generate random array
void generateRandomArray(int arr[], int size, int max_val) {
  for (int i = 0; i < size; i++)
    arr[i] = rand() % max_val;
}

//===------------------------ main() function ---------------------------------===//

int main(void) {
  srand(time(NULL));

  printf("===== MERGE SORT TESTING PROGRAM =====\n\n");

  // Test 1: Small array
  int arr1[] = {12, 11, 13, 5, 6, 7};
  int n1     = sizeof(arr1) / sizeof(arr1[0]);
  printf("Original array: ");
  printArray(arr1, n1);
  mergeSort(arr1, 0, n1 - 1);
  printf("Sorted array: ");
  printArray(arr1, n1);
  printf("Is sorted: %s\n\n", isSorted(arr1, n1) ? "Yes" : "No");

  // Test 2: Reversed array
  int arr2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  int n2     = sizeof(arr2) / sizeof(arr2[0]);
  printf("Original array: ");
  printArray(arr2, n2);
  mergeSort(arr2, 0, n2 - 1);
  printf("Sorted array: ");
  printArray(arr2, n2);
  printf("Is sorted: %s\n\n", isSorted(arr2, n2) ? "Yes" : "No");

  // Test 3: Random array
  int  size = 20;
  int* arr3 = (int*)malloc(size * sizeof(int));
  generateRandomArray(arr3, size, 100);
  printf("Random array: ");
  printArray(arr3, size);
  mergeSort(arr3, 0, size - 1);
  printf("Sorted array: ");
  printArray(arr3, size);
  printf("Is sorted: %s\n\n", isSorted(arr3, size) ? "Yes" : "No");
  free(arr3);

  // Test 4: Performance test
  int  large_size = 50000;
  int* arr4       = (int*)malloc(large_size * sizeof(int));
  generateRandomArray(arr4, large_size, 10000);
  printf("Sorting large array with %d elements...\n", large_size);

  clock_t start = clock();
  mergeSort(arr4, 0, large_size - 1);
  clock_t end = clock();

  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken: %.6f seconds\n", time_spent);
  printf("Is sorted: %s\n", isSorted(arr4, large_size) ? "Yes" : "No");
  free(arr4);

  return 0;
}
