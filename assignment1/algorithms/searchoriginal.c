#include "searchoriginal.h"

long long int searchorig(unsigned long long int *arr, long long int size, long long int value){
	return linearSearch(arr,size,value);//Return the index of the element in arr[] which has data that is equal to value
}	

long long int linearSearch(unsigned long long int *arr, long long int size, long long int value){
	unsigned long long int counter1, counter2;
	for (counter1 = 0; counter1 < size-1; counter1++){
		if (arr[counter1] == value){
			return counter1;
		}
	}
	return -1;
}

