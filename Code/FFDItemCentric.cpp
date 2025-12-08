#include <vector>
#include "FFDItemCentric.h"
#include "mergeSort.h"

#include <iostream>

std::vector<std::vector<int>> sort(std::vector<std::vector<int>> dataset);

std::vector<std::vector<int>> binPack(std::vector<std::vector<int>> dataset, int binSize) {
	dataset = sort(dataset);
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins = { {} };
	for (int i = 1; i < dataset[0].size(); i++) {
		bins[0].push_back(binSize);
	}
	int lastBin = 0;
	for (std::vector<int> item : dataset) {
		bool fit = false;
		int bin = 0;
		while (!fit) {
			// if no existing bins fit the item, create a new one
			if (bin < bins.size()) {
				// check each dimension against the remaining capacity of the bin (index 0 through d-1 of the bin)
				fit = true;
				// check if any values don't fit in the current bin
				for (int i = 1; i < item.size(); i++) {
					if (item[i] > bins[bin][i - 1]) {
						fit = false;
						break;
					}
				}
				// add item to bin if it fits
				if (fit) {
					bins[bin].push_back(item[0]);
					for (int i = 1; i < item.size(); i++) {
						bins[bin][i - 1] -= item[i];
					}
				}
			}
			else {
				// create new bin, add item
				std::vector<int> newBin;
				for (int i = 1; i < item.size(); i++) {
					newBin.push_back(binSize - item[i]);
				}
				newBin.push_back(item[0]);
				bins.push_back(newBin);
				fit = true;
			}
			// increment current bin index
			bin++;
		}
	}
	// remove the remaining capacity values from the bins, and return result
	for (std::vector<int> bin : bins) {
		std::vector<int> resultVector;
		for (int i = dataset[0].size() - 1; i < bin.size(); i++) {
			resultVector.push_back(bin[i]);
		}
		result.push_back(resultVector);
	}
	return result;
}

std::vector<std::vector<int>> sort(std::vector<std::vector<int>> dataset) {
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
		sortInput[vect[0]] = {vect[0], product};
	}
	// order dataset according to sort result
	std::vector<std::vector<int>> sortResult = mergeSort(sortInput);
	for (int i = 0; i < sortResult.size(); i++) {
		sortedDataset[i] = dataset[sortResult[i][0]];
	}
	return sortedDataset;
}