#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include "getDatasetArray.h"


// heuristic = 1
#include "FFDItemCentric.h"
// heuristic = 2
#include "FFDBinCentric.h"
// heursitic = 3
#include "GroupedByMostSignificant.h"
// heuristic = 4
#include "FirstNItems.h"
// heuristic = 5
#include "FFDBinCentricDynamicSort.h"
// heuristic = 6
#include "DecayingLookahead.h"

const int HEURISTIC = 6;

// function declarations
int getBinSize(std::string inputStr);
int getLowerBound(std::vector<std::vector<int>> dataset, int binsize);

int main() {
	const int dimensions = 2;
	// open file, get dataset length and bin size
	std::string inputFilePath = "../BenchmarkDatasets/CL_01_25_01.TXT";
	std::string line;
	std::ifstream DataFile(inputFilePath);
	std::getline(DataFile, line);
	int datasetLength = std::stoi(line);
	std::getline(DataFile, line);
	int binSize = getBinSize(line);
	std::vector<std::string> datasetString(datasetLength);
	// scope limited to allow reuse of name "i" if required
	{
		int i = 0;
		while (std::getline(DataFile, line)) {
			datasetString[i] = line;
			i++;
		}
	}
	// convert the dataset to a usable format (2d vector)
	// note - vector used because bin size is a runtime-constant, and not a compiler-constant
	//      - vector used for each value instead of array for consistency
	//      - also makes function definition of getDatasetArray less complex and size is not required on definition 
	std::vector<std::vector<int>> dataset = getDatasetArray(datasetString, (dimensions + 1));
	// output dataset (for testing)
	/*for (std::vector<int> row : dataset) {
		bool first = true;
		for (int val : row) {
			if (first) {
				std::cout << val;
				first = false;
			}
			else {
				std::cout << ", " << val;
			}
		}
		std::cout << std::endl;
	}*/
	int lowerBound = getLowerBound(dataset, binSize);
	std::vector<std::vector<int>> result;
	auto start = std::chrono::high_resolution_clock::now();
	switch (HEURISTIC) {
	case 1:
		result = FFDItemCentric(dataset, binSize);
		break;
	case 2:
		result = FFDBinCentric(dataset, binSize);
		break;
	case 3:
		result = GroupedByMostSignificant(dataset, binSize);
		break;
	case 4:
		result = FirstNItems(dataset, binSize);
		break;
	case 5:
		result = FFDBinCentricDynamicSort(dataset, binSize);
		break;
	case 6:
		result = DecayingLookahead(dataset, binSize, lowerBound);
		break;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	// output result
	for (std::vector<int> row : result) {
		bool first = true;
		for (int val : row) {
			if (first) {
				std::cout << val;
				first = false;
			}
			else {
				std::cout << ", " << val;
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Lower bound minimum bins: " << lowerBound << std::endl;
	std::cout << "Total bins: " << result.size();
	if (result.size() == lowerBound) {
		std::cout << " (definitely optimal)" << std::endl;
	} else {
		std::cout << std::endl;
	}
	std::cout << "Time taken (ms): " << duration.count();
}

int getBinSize(std::string inputStr) {
	std::string binsizeStr;
	bool started = false;
	// loop through characters in string, adding non-space characters to binsize
	// break if at least one character has been added to string and current character is a space (i.e., end of first number)
	for (char c : inputStr)
	{
		if (started && c == ' ') {
			break;
		}
		else if (c != ' ') {
			if (not started) { started = true; }
			binsizeStr += c;
		}
	}
	// return result as int
	return std::stoi(binsizeStr);
}

int getLowerBound(std::vector<std::vector<int>> dataset, int binsize) {
	int binsReqd = 0;
	for (int i = 1; i < dataset[0].size(); i++) {
		int total = 0;
		for (auto& vect : dataset) {
			total += vect[i];
		}
		int newReq = std::ceil((1.0 * total) / binsize);
		if (i == 1) {
			binsReqd = newReq;
		} else if (binsReqd < newReq) {
			binsReqd = newReq;
		}
	}
	return binsReqd;
}