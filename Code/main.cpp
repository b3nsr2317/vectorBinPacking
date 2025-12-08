#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "getDatasetArray.h"

// change header file to choose algorithm
#include "FFDItemCentric.h"

// function declarations
int getBinSize(std::string inputStr);

int main() {
	const int dimensions = 2;
	// open file, get dataset length and bin size
	std::string inputFilePath = "../BenchmarkDatasets/CL_03_200_07.TXT";
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
	std::vector<std::vector<int>> result = binPack(dataset, binSize);
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
	std::cout << "Total bins: " << result.size();
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