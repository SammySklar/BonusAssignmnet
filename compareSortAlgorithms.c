//Samuel Sklar - Bonus Assignment - 3/22/24
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz;
    size_t* ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

void DeAlloc(void* ptr)
{
    size_t* pSz = (size_t*)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
    return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int pData[], int n)
{
    extraMemoryAllocated = 0;

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        int index = i;
        int value = pData[index];
        while (2 * index + 1 < n) {
            int left = 2 * index + 1;
            int right = 2 * index + 2 < n ? 2 * index + 2 : left;
            int childIndex = pData[left] > pData[right] ? left : right;
            if (value >= pData[childIndex]) break;
            pData[index] = pData[childIndex];
            index = childIndex;
        }
        pData[index] = value;
    }

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        int temp = pData[0];
        pData[0] = pData[i];
        pData[i] = temp;

        // call max heapify on the reduced heap
        int index = 0;
        int value = pData[index];
        while (2 * index + 1 < i) {
            int left = 2 * index + 1;
            int right = 2 * index + 2 < i ? 2 * index + 2 : left;
            int childIndex = pData[left] > pData[right] ? left : right;
            if (value >= pData[childIndex]) break;
            pData[index] = pData[childIndex];
            index = childIndex;
        }
        pData[index] = value;
    }
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
int mergeSort(int pData[], int l, int r)
{
    int extraMemoryAllocated = 0;

    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        extraMemoryAllocated += mergeSort(pData, l, m);
        extraMemoryAllocated += mergeSort(pData, m + 1, r);

        // Merge the sorted halves in place
        int len1 = m - l + 1;
        int len2 = r - m;
        int* L = (int*)Alloc(sizeof(int) * len1);
        int* R = (int*)Alloc(sizeof(int) * len2);

        // Copy data to temporary arrays L[] and R[]
        for (int i = 0; i < len1; i++)
            L[i] = pData[l + i];
        for (int j = 0; j < len2; j++)
            R[j] = pData[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < len1 && j < len2) {
            if (L[i] <= R[j]) {
                pData[k++] = L[i++];
            } else {
                pData[k++] = R[j++];
            }
        }

        // Copy the remaining elements of L[], if any
        while (i < len1) {
            pData[k++] = L[i++];
        }

        // Copy the remaining elements of R[], if any
        while (j < len2) {
            pData[k++] = R[j++];
        }

        // Deallocate memory
        DeAlloc(L);
        DeAlloc(R);

        // Add the memory allocated for L and R to the total extra memory
        extraMemoryAllocated += sizeof(int) * (len1 + len2);
    }

    return extraMemoryAllocated;
}


// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
    int extraMemoryAllocated = 0;

    for (int i = 1; i < n; ++i) {
        int key = pData[i];
        int j = i - 1;
        
        // Move elements of pData[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && pData[j] > key) {
            pData[j + 1] = pData[j];
            j--;
        }
        pData[j + 1] = key;
    }

    // No additional memory was allocated by insertion sort
    extraMemoryAllocated = 0;

    printf("Extra memory allocated: %d\n", extraMemoryAllocated);
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int *pData, int n)
{
    int extraMemoryAllocated = 0;

    // Bubble sort algorithm
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (pData[j] > pData[j + 1])
            {
                // Swap pData[j] and pData[j + 1]
                int temp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = temp;
            }
        }
    }

    // No additional memory was allocated by bubble sort
    extraMemoryAllocated = 0;

    printf("Extra memory allocated: %d\n", extraMemoryAllocated);
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int *pData, int n)
{
    int extraMemoryAllocated = 0;

    // Selection sort algorithm
    for (int i = 0; i < n - 1; ++i)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (pData[j] < pData[minIndex])
            {
                minIndex = j;
            }
        }
        // Swap pData[i] and pData[minIndex]
        int temp = pData[i];
        pData[i] = pData[minIndex];
        pData[minIndex] = temp;
    }

    // No additional memory was allocated by selection sort
    extraMemoryAllocated = 0;

    printf("Extra memory allocated: %d\n", extraMemoryAllocated);
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    *ppData = NULL;
    
    if (inFile)
    {
        // Read the size of the data
        fscanf(inFile, "%d\n", &dataSz);
        
        // Allocate memory for the data
        *ppData = (int *)Alloc(sizeof(int) * dataSz);
        
        // Read the data elements
        for (int i = 0; i < dataSz; ++i) {
            fscanf(inFile, "%d", &((*ppData)[i]));
        }
        
        fclose(inFile);
    }
    
    return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
