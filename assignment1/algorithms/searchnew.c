#include "searchnew.h"

long long int searchnew(unsigned long long int *arr, long long int size, long long int value){
	return opt(arr, size, value);//Return the index of the element in arr[] which has data that is equal to value
}

long long int opt(unsigned long long int *arr, long long int size, long long int value) {
// +++++++++++++++++++ Add your code below, DO NOT EDIT ANYTHING ABOVE THIS LINE ++++++++++++++++++//
	// Binary search
	long long int middle;
	long long int left = 0;
    long long int right = size - 1;
    
    while (left <= right) {
        middle = left + (right - left) / 2;
		
        if (arr[middle] == value) {
            return middle;
        } else if (arr[middle] > value) {
			right = middle - 1;
            
        } else {
            left = middle + 1;
        }
    }

	return -1;//Return the index of the element in arr[] which has data that is equal to value


}

