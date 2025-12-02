#include <vector>
#include <cmath>
#include "mergeSort.h"

std::vector<std::vector<int>> merge(std::vector<std::vector<int>> left, std::vector<std::vector<int>> right);

std::vector<std::vector<int>> mergeSort(std::vector<std::vector<int>> dataset) {
	if (dataset.size() <= 1) {
		return dataset;
	}
	// get midpoint, no need to round as integer division so truncated by default
	size_t middle = dataset.size() / 2;
	// divide into 2 halves recursively
	std::vector<std::vector<int>> left = mergeSort(std::vector<std::vector<int>>(dataset.begin(), dataset.begin() + middle));
	std::vector<std::vector<int>> right = mergeSort(std::vector<std::vector<int>>(dataset.begin() + middle, dataset.end()));

	// merge 2 sorted sublists
	return merge(left, right);
}

std::vector<std::vector<int>> merge(std::vector<std::vector<int>> left, std::vector<std::vector<int>> right) {
	// create index for both lists, and for result
	size_t ileft = 0;
	size_t iright = 0;
	size_t i = 0;
	// get size of both lists
	size_t lsize = left.size();
	size_t rsize = right.size();
	// define result vector
	std::vector<std::vector<int>> result(lsize + rsize, std::vector<int>(2));

	// compare items one by one until one list is empty
	while (ileft < lsize && iright < rsize) {
		if (left[ileft][1] <= right[iright][1]) {
			result[i] = right[iright];
			iright++;
		}
		else {
			result[i] = left[ileft];
			ileft++;
		}
		i++;
	}
	// append remaining values to end of result, then return
	while (ileft < lsize) {
		result[i] = left[ileft];
		ileft++;
		i++;
	}
	while (iright < rsize) {
		result[i] = right[iright];
		iright++;
		i++;
	}
	return result;
}