#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include "getDatasetArray.h"


// heuristic = 0
#include "FirstFit.h"
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
// heuristic = 7
#include "PanigrahyBinCentric.h"
// heuristic = 8
#include "FFDProductSort.h"
// heuristic = 9
#include "FFDL2.h"
// heuristic = 10
#include "FFDLinf.h"
// heuristic = 11
#include "FFDL1Weighted.h"

const std::vector<int> HEURISTICS = { 7, 1 };

const bool CAPRARATOTHBENCHMARK = false;

// function declarations
int getBinSize(std::string inputStr);
int getLowerBound(std::vector<std::vector<int>> dataset, int binsize);

int main() {
	int dimensions;
	// open file, get dataset length and bin size
	//std::string inputFilePath = "../BenchmarkDatasets/2cbp/Class4/CL_04_200_01.TXT";
	std::string inputFilePath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class1/class1_500_5_1.vbp";
	//std::string inputFilePath = "../BenchmarkDatasets/New_instances_multidim/class1/class1_500_3_0.vbp";
	//std::string inputFilePath = "../BenchmarkDatasets/Triplet_instances_multidim/Caprara/classC_60_10_0.vbp";
	//std::string inputFilePath = "../BenchmarkDatasets/Triplet_instances_multidim/Falkenauer/classF_60_3_0.vbp";
	std::string line;
	std::ifstream DataFile(inputFilePath);
	int binSize;
	int datasetLength;
	if (CAPRARATOTHBENCHMARK) {
		dimensions = 2;
		std::getline(DataFile, line);
		datasetLength= std::stoi(line);
		std::getline(DataFile, line);
		binSize = getBinSize(line);
	}
	else {
		std::getline(DataFile, line);
		dimensions = std::stoi(line);
		std::getline(DataFile, line);
		binSize = getBinSize(line);
		std::getline(DataFile, line);
		datasetLength = std::stoi(line);
	}
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
	std::vector<std::vector<int>> dataset = getDatasetArray(datasetString, (dimensions + 1), CAPRARATOTHBENCHMARK);
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
	for (int heuristic : HEURISTICS) {
		std::vector<std::vector<int>> result;
		auto start = std::chrono::high_resolution_clock::now();
		switch (heuristic) {
		case 0:
			result = FirstFit(dataset, binSize);
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
		case 7:
			result = PanigrahyBinCentric(dataset, binSize);
			break;
		case 8:
			result = FFDProductSort(dataset, binSize);
			break;
		case 9:
			result = FFDL2(dataset, binSize);
			break;
		case 10:
			result = FFDLinf(dataset, binSize);
			break;
		case 11:
			result = FFDL1Weighted(dataset, binSize);
			break;
		}
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		// counter to confirm all items have been packed
		//int itemCount = 0;
		// output result
		/*for (std::vector<int> row : result) {
			bool first = true;
			for (int val : row) {
				if (first) {
					std::cout << val;
					first = false;
				}
				else {
					std::cout << ", " << val;
				}
				itemCount++;
			}
			std::cout << std::endl;
		}
		std::cout << "items used: " << itemCount << std::endl;*/
		std::cout << "Total bins: " << result.size();
		if (result.size() == lowerBound) {
			std::cout << " (definitely optimal)" << std::endl;
		}
		else {
			std::cout << std::endl;
		}
		std::cout << "Time taken (ms): " << duration.count() << std::endl;
		std::cout << std::endl;
	}
	std::cout << "Dimensions: " << dimensions << std::endl;
	std::cout << "Lower bound minimum bins: " << lowerBound << std::endl;
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
			if (!started) { started = true; }
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