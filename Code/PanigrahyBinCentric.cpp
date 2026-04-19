#include <iostream>
#include <vector>
#include "PanigrahyBinCentric.h"
#include "mergeSort.h"

int getBestFit(std::vector<int> capacity, std::vector<std::vector<int>>& dataset, int vectSize);
int getDotProduct(std::vector<int>& item, std::vector<int>& capacity, int vectSize);

std::vector<std::vector<int>> PanigrahyBinCentric(std::vector<std::vector<int>> dataset, int binSize) {
	//std::cout << "Panigrahy Dot-Product Bin Centric:" << std::endl;
	int vectorSize = dataset[0].size();
	std::vector <std::vector<int>> bins;
	int currentBin = -1;
	while (dataset.size() > 0) {
		// create new bin
		bins.push_back({});
		currentBin++;
		std::vector<int> remainingCapacity;
		for (int i = 1; i < vectorSize; i++) {
			remainingCapacity.push_back(binSize);
		}
		bool itemFits = true;
		while (itemFits) {
			if (dataset.size() == 0) {
				break;
			}
			int nextItem = getBestFit(remainingCapacity, dataset, vectorSize);
			if (nextItem == -1) {
				itemFits = false;
			}
			else {
				bins[currentBin].push_back(dataset[nextItem][0]);
				for (int i = 0; i < vectorSize - 1; i++) {
					remainingCapacity[i] -= dataset[nextItem][i + 1];
				}
				dataset.erase(dataset.begin() + nextItem);
			}
		}
	}
	return bins;
}

int getBestFit(std::vector<int> capacity, std::vector<std::vector<int>>& dataset, int vectSize) {
	int best = 0;
	int bestItem = -1;
	for (int i = 0; i < dataset.size(); i++) {
		bool fits = true;
		for (int j = 0; j < vectSize - 1; j++) {
			if (dataset[i][j + 1] > capacity[j]) {
				fits = false;
				break;
			}
		}
		if (fits) {
			int dotProduct = getDotProduct(dataset[i], capacity, vectSize);
			if (dotProduct > best) {
				best = dotProduct;
				bestItem = i;
			}
		}
	}
	return bestItem;
}

int getDotProduct(std::vector<int>& item, std::vector<int>& capacity, int vectSize) {
	int total = 0;
	for (int i = 0; i < vectSize - 1; i++) {
		total += item[i + 1] * capacity[i];
	}
	return total;
}