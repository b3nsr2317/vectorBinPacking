#include <iostream>
#include <vector>
#include "FFDItemCentric.h"
#include "mergeSort.h"


std::vector<std::vector<int>> GroupedByMostSignificant(std::vector<std::vector<int>> dataset, int binSize) {
	std::cout << "Grouped By Most Significant:" << std::endl;
	int vectorSize = dataset[0].size();
	std::vector<std::vector<int>> fullDataset = dataset;
	std::vector<std::vector<std::vector<int>>> splitDataset;
	std::vector<std::vector<int>> result;
	std::vector <std::vector<int>> bins;
	// split the dataset into lists where the index of the list is also the index of the most significant value of each item in the list
	for (int i = 1; i < vectorSize; i++) {
		splitDataset.push_back(std::vector<std::vector<int>>());
		// go through each remaining vector
		for (int j = 0; j < dataset.size(); j++) {
			std::vector<int> vector = dataset[j];
			// check if vector[i] is the most significant value
			bool largest = true;
			for (int k = 1; k < vector.size(); k++) {
				if (vector[k] > vector[i]) {
					largest = false;
				}
			}
			// add to split dataset, remove from dataset, decrement j to compensate for removal
			if (largest) {
				splitDataset[i - 1].push_back(vector);
				dataset.erase(dataset.begin() + j);
				j--;
			}
		}
	}
	// sort each list by its most significant value
	for (int i = 0; i < splitDataset.size(); i++) {
		std::vector<std::vector<int>> listToSort;
		for (auto& vector : splitDataset[i]) {
			listToSort.push_back({ vector[0], vector[i + 1] });
		}
		listToSort = mergeSort(listToSort);
		std::vector<std::vector<int>> sortedList;
		for (auto& vector : listToSort) {
			sortedList.push_back(fullDataset[vector[0]]);
		}
		splitDataset[i] = sortedList;
	}
	bool datasetEmpty = false;
	int currentBin = -1;
	while (!datasetEmpty) {
		bins.push_back({});
		currentBin++;
		for (int i = 1; i < vectorSize; i++) {
			bins[currentBin].push_back(binSize);
		}
		bool binFull = false;
		// vector of lists in splitDataset that may have an item that fits in bin
		std::vector<int> untestedLists;
		for (int i = 0; i < splitDataset.size(); i++) {
			untestedLists.push_back(i);
		}
		while (!binFull) {
			// get vector of most empty dimension(s)
			std::vector<int> mostRemainingSpace = { untestedLists[0] };
			for (int i : untestedLists) {
				if (bins[currentBin][i] > bins[currentBin][mostRemainingSpace[0]]) {
					mostRemainingSpace = { i };
				}
				else if (bins[currentBin][i] == bins[currentBin][mostRemainingSpace[0]]) {
					mostRemainingSpace.push_back(i);
				}
			}
			int listToCheck = mostRemainingSpace[0];
			// check which list has the most items left
			if (mostRemainingSpace.size() > 1) {
				for (int i : mostRemainingSpace) {
					if (splitDataset[listToCheck].size() < splitDataset[i].size()) {
						listToCheck = i;
					}
				}
			}
			bool addedItem = false;
			for (int i = 0; i < splitDataset[listToCheck].size(); i++) {
				std::vector<int> item = splitDataset[listToCheck][i];
				bool fit = true;
				for (int j = 1; j < vectorSize; j++) {
					if (item[j] > bins[currentBin][j - 1]) {
						fit = false;
						break;
					}
				}
				if (fit) {
					addedItem = true;
					bins[currentBin].push_back(item[0]);
					for (int j = 1; j < vectorSize; j++) {
						bins[currentBin][j - 1] -= item[j];
					}
					splitDataset[listToCheck].erase(splitDataset[listToCheck].begin() + i);
					break;
				}
			}
			if (!addedItem) {
				for (int i = 0; i < untestedLists.size(); i++) {
					if (untestedLists[i] == listToCheck) {
						untestedLists.erase(untestedLists.begin() + i);
					}
				}
			}
			if (untestedLists.size() == 0) {
				binFull = true;
			}
		}
		// check if all lists are empty before looping
		datasetEmpty = true;
		for (auto& list : splitDataset) {
			if (list.size() > 0) {
				datasetEmpty = false;
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

