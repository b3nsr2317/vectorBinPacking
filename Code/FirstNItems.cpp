#include <vector>
#include <cmath>
#include "FirstNItems.h"
#include "mergeSort.h"

#include <iostream>

std::vector<std::vector<int>> FirstNItems(std::vector<std::vector<int>> dataset, int binSize) {
	std::cout << "First N Items:" << std::endl;
	dataset = sortByProduct(dataset);
	int minBins = 0;
	for (int i = 1; i < dataset[0].size(); i++) {
		float total = 0;
		for (auto& item : dataset) {
			total += item[i];
		}
		if (total / binSize > minBins) {
			minBins = std::ceil(total / binSize);
		}
	}
	std::cout << "Minimum bins: " << minBins << std::endl;
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins = {};
	for (int i = 0; i < minBins; i++) {
		bins.push_back({});
		for (int j = 1; j < dataset[0].size(); j++) {
			bins[i].push_back(binSize - dataset[i][j]);
		}
		bins[i].push_back(dataset[i][0]);
	}
	for (int i = 0; i < minBins; i++) {
		dataset.erase(dataset.begin() + i);
	}
	for (auto& item : dataset) {
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
	for (auto& bin : bins) {
		std::vector<int> resultVector;
		for (int i = dataset[0].size() - 1; i < bin.size(); i++) {
			resultVector.push_back(bin[i]);
		}
		result.push_back(resultVector);
	}
	return result;
}