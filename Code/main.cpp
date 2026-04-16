#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <regex>
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

const std::vector<int> HEURISTICS = { 1, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

const bool CAPRARATOTHBENCHMARK = false;


// function declarations
int getBinSize(std::string inputStr);
int getLowerBound(std::vector<std::vector<int>> dataset, int binsize);

int main() {
	int dimensions;
	// open file, get dataset length and bin size
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class1/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class2/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class3/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class4/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class5/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class6/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class7/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class8/";
	//std::string inputFolderPath = "../BenchmarkDatasets/2cbp/Class10/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class1/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class2/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class3/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class4/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class5/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class6/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class7/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class8/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Panigrahy_instances_multidim/class9/";
	//std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class1/";
	//std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class2/";
	//std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class3/";
	std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class4/";
	//std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class5/";
	//std::string inputFolderPath = "../BenchmarkDatasets/New_instances_multidim/class6/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Triplet_instances_multidim/Caprara/";
	//std::string inputFolderPath = "../BenchmarkDatasets/Triplet_instances_multidim/Falkenauer/";

	int datasetsUsed = 0;
	std::vector<int> totalTime;
	std::vector<int> totalBins;
	std::vector<int> totalConfirmedOptimal;
	int totalLowerBound = 0;

	bool firstRun = true;

	for (auto& inputFile : std::filesystem::directory_iterator(inputFolderPath)) {
		std::string inputFileStr = inputFile.path().filename().string();
		// Check file matches desired dataset size and dimensions
		// for caprara toth, use "_d_"
		// for vectorpack use "_s_d_"
		const std::regex pattern("_500_3_");
		if (!std::regex_search(inputFileStr, pattern)) {
			continue;
		}
		datasetsUsed++;
		std::string inputFilePath = inputFolderPath + inputFileStr;
		//std::cout << inputFilePath;
		std::string line;
		std::ifstream DataFile(inputFilePath);
		int binSize;
		int datasetLength;
		if (CAPRARATOTHBENCHMARK) {
			dimensions = 2;
			std::getline(DataFile, line);
			datasetLength = std::stoi(line);
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
		// scope limited to allow reuse of name "i"
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
		for (int i = 0; i < HEURISTICS.size(); i++) {
			int heuristic = HEURISTICS[i];
			std::vector<std::vector<int>> result;
			auto start = std::chrono::high_resolution_clock::now();
			switch (heuristic) {
			case 0:
				result = FirstFit(dataset, binSize);
				break;
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
			if (firstRun) {
				totalBins.push_back(result.size());
				totalTime.push_back(duration.count());
				if (result.size() == lowerBound) {
					totalConfirmedOptimal.push_back(1);
				}
				else {
					totalConfirmedOptimal.push_back(0);
				}
			}
			else {
				totalBins[i] += result.size();
				totalTime [i] += duration.count();
				if (result.size() == lowerBound) {
					totalConfirmedOptimal[i]++;
				}
			}
			//std::cout << "Total bins: " << result.size();
			//if (result.size() == lowerBound) {
			//	std::cout << " (definitely optimal)" << std::endl;
			//}
			//else {
			//	std::cout << std::endl;
			//}
			//std::cout << "Time taken (ms): " << duration.count() << std::endl;
			//std::cout << std::endl;
		}
		//std::cout << "Dimensions: " << dimensions << std::endl;
		totalLowerBound += lowerBound;
		//std::cout << "Lower bound minimum bins: " << lowerBound << std::endl;
		if (firstRun) {
			firstRun = false;
		}
	}
	std::cout << "Instances Tested: " << datasetsUsed << " - Average Lower Bound: " << 1.0 * totalLowerBound / datasetsUsed << std::endl << std::endl;
	for (int i = 0; i < HEURISTICS.size(); i++) {
		std::cout << "Heuristic " << HEURISTICS[i] << ":" << std::endl;
		std::cout << "Mean Time: " << 1.0 * totalTime[i] / datasetsUsed << "ms - ";
		std::cout << "Mean Bins: " << 1.0 * totalBins[i] / datasetsUsed << std::endl;
		if (totalConfirmedOptimal[i] > 0) {
			std::cout << totalConfirmedOptimal[i] << " confirmed optimal results" << std::endl << std::endl;
		}
	}
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