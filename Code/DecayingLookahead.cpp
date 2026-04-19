#include <vector>
#include <iostream>
#include <cmath>
#include "DecayingLookahead.h"
#include "mergeSort.h"

std::vector<int> fillBin(std::vector<std::vector<int>> &dataset, int binSize, int initialLookahead, float decayRate);
std::vector<int> placeItems(std::vector<std::vector<int>>& dataset, std::vector<int> bin, float lookahead, float &decayRate, int startItem);
bool checkItem(std::vector<int> &bin, std::vector<int> &item);

std::vector<std::vector<int>> DecayingLookahead(std::vector<std::vector<int>> dataset, int binSize, int lowerBound) {
	//std::cout << "Decaying Lookahead:" << std::endl;
	int datasetSize = dataset.size();
	int vectorSize = dataset[0].size();
	float estimatePerBin = dataset.size() / lowerBound;
	int initialLookforward = std::ceil(25/estimatePerBin);
	if (initialLookforward < 3) {
		initialLookforward = 3;
	}
	float decayRate = 4/estimatePerBin;
	//std::cout << "Starting Lookforward: " << initialLookforward << ". Decay Rate: " << decayRate << std::endl;
	dataset = sortL1(dataset);
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins;
	while (dataset.size() > 0) {
		bins.push_back(fillBin(dataset, binSize, initialLookforward, decayRate));
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

std::vector<int> fillBin(std::vector<std::vector<int>> &dataset, int binSize, int initialLookahead, float decayRate) {
	bool binFull = false;
	std::vector<int> bin = {};
	for (int i = 1; i < dataset[0].size(); i++) {
		bin.push_back(binSize - dataset[0][i]);
	}
	bin.push_back(dataset[0][0]);
	std::vector<int> fullBin = placeItems(dataset, bin, float(initialLookahead), decayRate, 1);
	// remove used items from dataset
	int datasetIndex = 0;
	std::vector<int> toRemove = {};
	for (int i = dataset[0].size() - 1; i < fullBin.size(); i++) {
		for (int j = datasetIndex; j < dataset.size(); j++) {
			if (dataset[j][0] == fullBin[i]) {
				datasetIndex = j + 1;
				toRemove.push_back(j);
				break;
			}
		}
	}
	for (int i = toRemove.size() - 1; i >= 0; i--) {
		dataset.erase(dataset.begin() + toRemove[i]);
	}
	return fullBin;
}

std::vector<int> placeItems(std::vector<std::vector<int>> &dataset, std::vector<int> bin, float lookahead, float &decayRate, int startItem) {
	// check dataset is not "empty"
	if (startItem >= dataset.size()) {
		return bin;
	}
	// calculate new lookahead, ensure it is at least 1
	float newLookahead = lookahead - decayRate;
	if (newLookahead < 1) {
		newLookahead = 1;
	}
	std::vector<int> newBin = bin;
	// check n items, where n is the current lookahead
	for (int i = 0; i < std::ceil(lookahead); i++) {
		std::vector<int> testBin = bin;
		bool itemFit = false;
		// if an item fits, add it to the test bin and recurse
		for (int j = startItem; j < dataset.size(); j++) {
			if (checkItem(testBin, dataset[j])) {
				for (int k = 1; k < dataset[j].size(); k++) {
					testBin[k - 1] -= dataset[j][k];
				}
				testBin.push_back(dataset[j][0]);
				testBin = placeItems(dataset, testBin, newLookahead, decayRate, (j + 1));
				itemFit = true;
				startItem = j + 1;
				break;
			}
		}
		// if no item fits, stop looping
		if (!itemFit) {
			break;
		}
		// if test bin is better than current best, replace it
		int bestTotal = 0;
		int newTotal = 0;
		for (int j = 0; j < dataset[0].size() - 1; j++) {
			bestTotal += newBin[j];
			newTotal += testBin[j];
		}
		if (newTotal < bestTotal) {
			newBin = testBin;
		}
	}
	// return bin
	return newBin;
}

bool checkItem(std::vector<int> &bin, std::vector<int> &item) {
	int fit = true;
	// check if any values don't fit in the current bin
	for (int i = 1; i < item.size(); i++) {
		if (item[i] > bin[i - 1]) {
			fit = false;
			break;
		}
	}
	return fit;
}