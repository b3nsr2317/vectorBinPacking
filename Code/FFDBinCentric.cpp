#include <vector>
#include "FFDItemCentric.h"
#include "mergeSort.h"

#include <iostream>

std::vector<std::vector<int>> FFDBinCentric(std::vector<std::vector<int>> dataset, int binSize) {
	std::cout << "FFD Bin Centric:" << std::endl;
	dataset = sortByProduct(dataset);
	int vectorSize = dataset[0].size();
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins;
	int currentBin = -1;
	while (dataset.size() > 0) {
		bins.push_back({});
		currentBin++;
		for (int i = 1; i < vectorSize; i++) {
			bins[currentBin].push_back(binSize);
		}
		int currentItem = 0;
		while (currentItem < dataset.size()) {
			int fit = true;
			// check if any values don't fit in the current bin
			for (int i = 1; i < vectorSize; i++) {
				if (dataset[currentItem][i] > bins[currentBin][i - 1]) {
					fit = false;
					currentItem++;
					break;
				}
			}
			if (fit) {
				bool full = false;
				bins[currentBin].push_back(dataset[currentItem][0]);
				for (int i = 1; i < vectorSize; i++) {
					bins[currentBin][i - 1] -= dataset[currentItem][i];
					if (bins[currentBin][i - 1] == 0) {
						full = true;
					}
				}
				dataset.erase(dataset.begin() + currentItem);
				if (full) {
					break;
				}
			}
		}
	}
	// remove the remaining capacity values from the bins, and return result
	for (auto& bin : bins) {
		std::vector<int> resultVector;
		for (int i = vectorSize - 1; i < bin.size(); i++) {
			resultVector.push_back(bin[i]);
		}
		result.push_back(resultVector);
	}
	return result;
}

