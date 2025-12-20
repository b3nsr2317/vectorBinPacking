#include <vector>
#include <cmath>
#include <iostream>
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

std::vector<std::vector<int>> sortByProduct(std::vector<std::vector<int>> dataset) {
	std::vector<std::vector<int>> sortInput(dataset.size(), std::vector<int>(2));
	std::vector<std::vector<int>> sortedDataset = dataset;
	// get the product of each dimension to use as measure of vector size in sort
	for (std::vector<int> vect : dataset) {
		bool first = true;
		int product = 1;
		for (int val : vect) {
			if (first) {
				first = false;
			}
			else {
				product *= val;
			}
		}
		sortInput[vect[0]] = { vect[0], product };
	}
	// order dataset according to sort result
	std::vector<std::vector<int>> sortResult = mergeSort(sortInput);
	for (int i = 0; i < sortResult.size(); i++) {
		sortedDataset[i] = dataset[sortResult[i][0]];
	}
	return sortedDataset;
}

std::vector<std::vector<int>> multiFactorSort(std::vector<std::vector<int>> dataset) {
	std::vector<std::vector<int>> sortInput(dataset.size(), std::vector<int>(2));
	std::vector<std::vector<int>> sortedDataset = dataset;
	std::vector<std::vector<int>> vectorRangeAndProduct(dataset.size(), std::vector<int>(3));
	int avgRange = 0;
	int avgAvg = 0;
	// get the difference between largest and smallest dimension to use as measure of vector size in sort
	// get average size of each vector as measure of size in sort
	for (auto& vect : dataset) {
		int largest = 1;
		int smallest = 1;
		int avg = 1;
		for (int i = 1; i < vect.size(); i++) {
			avg += vect[i];
			if (vect[i] < vect[smallest]) {
				smallest = i;
			}
			else if (vect[i] > vect[largest]) {
				largest = i;
			}
		}
		avg /= (vect.size() - 1);
		int range = vect[largest] - vect[smallest];
		avgRange += range;
		avgAvg += avg;
		vectorRangeAndProduct[vect[0]] = { vect[0], range, avg };
	}
	avgRange /= dataset.size();
	avgAvg /= dataset.size();
	int rangeWeight = round(1000.0 * avgRange / (avgRange + avgAvg));
	int avgWeight = 1000 - rangeWeight;
	for (int i = 0; i < dataset.size(); i++) {
		sortInput[i][0] = vectorRangeAndProduct[i][0];
		int size = vectorRangeAndProduct[i][1] * rangeWeight + vectorRangeAndProduct[i][2] * avgWeight;
		sortInput[i][1] = size;
	}
	// order dataset according to sort result
	std::vector<std::vector<int>> sortResult = mergeSort(sortInput);
	for (int i = 0; i < sortResult.size(); i++) {
		sortedDataset[i] = dataset[sortResult[i][0]];
	}
	return sortedDataset;
}