#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void BubbleSort(int A[], int n);
void BubbleSortOpt(int A[], int n);
void GnomeSort(int A[], int n);
int key(int x, int i);
void SortAux(int A[], int n, int i);
void RadixSort(int A[], int n, int k);
int partition(int A[], int p, int r);
void QuickSort(int A[], int p, int r);
void heapify(int A[], int n, int i);
void buildHeap(int A[], int n);
void HeapSort(int A[], int n);
void check_sorted(int A[], int n);

void RadixSortWrapper(int A[], int n)
{
    RadixSort(A, n, 5);
}

void QuickSortWrapper(int A[], int n)
{
    QuickSort(A, 0, n - 1);
}

void BubbleSort(int A[], int n)
{
    int change = 1;
    while (change)
    {
        change = 0;
        for (int i = 0; i < n - 1; i++)
        {
            if (A[i] > A[i + 1])
            {
                int temp = A[i];
                A[i] = A[i + 1];
                A[i + 1] = temp;
                change = 1;
            }
        }
    }
}

void BubbleSortOpt(int A[], int n)
{
    int m = n - 1;
    int change = 1;
    while (change && m > 0)
    {
        change = 0;
        for (int i = 0; i < m; i++)
        {
            if (A[i] > A[i + 1])
            {
                int temp = A[i];
                A[i] = A[i + 1];
                A[i + 1] = temp;
                change = 1;
            }
        }
        m--;
    }
}

void GnomeSort(int A[], int n)
{
    int index = 0;
    while (index < n)
    {
        if (index == 0 || A[index - 1] <= A[index])
            index++;
        else
        {
            int temp = A[index];
            A[index] = A[index - 1];
            A[index - 1] = temp;
            index--;
        }
    }
}

int key(int x, int i)
{
    return (x / (int)pow(10, i)) % 10;
}

void SortAux(int A[], int n, int i)
{
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};

    for (int j = 0; j < n; j++)
        count[key(A[j], i)]++;

    for (int j = 1; j < 10; j++)
        count[j] += count[j - 1];

    for (int j = n - 1; j >= 0; j--)
    {
        output[count[key(A[j], i)] - 1] = A[j];
        count[key(A[j], i)]--;
    }

    for (int j = 0; j < n; j++)
        A[j] = output[j];

    free(output);
}

void RadixSort(int A[], int n, int k)
{
    for (int i = 0; i < k; i++)
        SortAux(A, n, i);
}

int partition(int A[], int p, int r)
{
    int pivot = A[p];
    int i = p - 1;
    int j = r + 1;
    while (1)
    {
        do
        {
            j--;
        } while (A[j] > pivot);
        do
        {
            i++;
        } while (A[i] < pivot);
        if (i < j)
        {
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
        else
            return j;
    }
}

void QuickSort(int A[], int p, int r)
{
    if (p < r)
    {
        int q = partition(A, p, r);
        QuickSort(A, p, q);
        QuickSort(A, q + 1, r);
    }
}

void heapify(int A[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && A[left] > A[largest])
        largest = left;
    if (right < n && A[right] > A[largest])
        largest = right;

    if (largest != i)
    {
        int temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        heapify(A, n, largest);
    }
}

void buildHeap(int A[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(A, n, i);
}

void HeapSort(int A[], int n)
{
    buildHeap(A, n);
    for (int i = n - 1; i > 0; i--)
    {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heapify(A, i, 0);
    }
}

void check_sorted(int A[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        if (A[i] > A[i + 1])
        {
            printf("Array is not sorted correctly!\n");
            return;
        }
    }
}

void test_algorithm(const char *name, void (*sort_func)(int[], int), int n, int num_runs, FILE *fp)
{
    double total_time = 0;
    int *arr = (int *)malloc(n * sizeof(int));
    int *temp = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100000;
    }

    for (int run = 0; run < num_runs; run++)
    {
        memcpy(temp, arr, n * sizeof(int));

        clock_t start = clock();
        sort_func(temp, n);
        clock_t end = clock();

        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;

        check_sorted(temp, n);
    }

    double avg_time = total_time / num_runs;
    printf("%s - n=%d: %.6f seconds (average)\n", name, n, avg_time);
    fprintf(fp, "%.6f,", avg_time);

    free(arr);
    free(temp);
}

int main()
{
    srand(time(NULL));

    int sizes[] = {100, 500, 1000, 5000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_runs = 5;

    FILE *fp = fopen("sorting_results.csv", "w");
    fprintf(fp, "n,BubbleSort,BubbleSortOpt,GnomeSort,RadixSort,QuickSort,HeapSort\n");

    for (int i = 0; i < num_sizes; i++)
    {
        int n = sizes[i];
        fprintf(fp, "%d,", n);
        printf("\nTesting for array size n = %d\n", n);

        test_algorithm("BubbleSort", BubbleSort, n, num_runs, fp);
        test_algorithm("BubbleSortOpt", BubbleSortOpt, n, num_runs, fp);
        test_algorithm("GnomeSort", GnomeSort, n, num_runs, fp);
        test_algorithm("RadixSort", RadixSortWrapper, n, num_runs, fp);
        test_algorithm("QuickSort", QuickSortWrapper, n, num_runs, fp);
        test_algorithm("HeapSort", HeapSort, n, num_runs, fp);

        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("\nResults have been written to sorting_results.csv\n");

    printf("Running analysis.py...\n");
    int result = system("python analysis.py");
    if (result != 0)
    {
        printf("Error: Failed to run analysis.py.\n");
        return 1;
    }

    printf("Analysis completed successfully!\n");

    return 0;
}