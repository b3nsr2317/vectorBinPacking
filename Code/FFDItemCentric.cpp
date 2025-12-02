#include <vector>
#include "FFDItemCentric.h"
#include "mergeSort.h"

#include <iostream>

std::vector<std::vector<int>> sort(std::vector<std::vector<int>> dataset);

std::vector<std::vector<int>> binPack(std::vector<std::vector<int>> dataset, int binSize) {
	dataset = sort(dataset);
	return dataset;
}

std::vector<std::vector<int>> sort(std::vector<std::vector<int>> dataset) {
	std::vector<std::vector<int>> sortInput(dataset.size(), std::vector<int>(2));
	std::vector<std::vector<int>> newDataset = dataset;
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
		sortInput[vect[0]] = {vect[0], product};
	}
	std::vector<std::vector<int>> result = mergeSort(sortInput);
	for (std::vector<int> row : result) {
		std::cout << row[1] << std::endl;
	}
	for (int i = 0; i < result.size(); i++) {
		newDataset[i] = dataset[result[i][0]];
	}
	return newDataset;
}