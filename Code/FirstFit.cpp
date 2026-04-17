#include <iostream>
#include <vector>
#include "FirstFit.h"


std::vector<std::vector<int>> FirstFit(std::vector<std::vector<int>> dataset, int binSize) {
	//std::cout << "First Fit:" << std::endl;
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins = { {} };
	for (int i = 1; i < dataset[0].size(); i++) {
		bins[0].push_back(binSize);
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