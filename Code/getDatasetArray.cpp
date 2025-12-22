#include "getDatasetArray.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>

std::vector<int> parseLine(std::string line, int dimensions, int lineNo);
std::vector<int> parseLineCaparaToth(std::string line, int dimensions);

std::vector<std::vector<int>> getDatasetArray(std::vector<std::string> inputData, int dimensions, bool ct) {
	// create 2D vector to hold dataset
	std::vector<std::vector<int>> dataset(inputData.size(), std::vector<int>(dimensions));
	// loop through line by line, adding relevant values to dataset
	int counter = 0;
	for (std::string line : inputData) {
		if (ct) {
			dataset[counter] = parseLineCaparaToth(line, dimensions);
		} else {
			dataset[counter] = parseLine(line, dimensions, counter);
		}
		counter++;
	}
	return dataset;
}

std::vector<int> parseLine(std::string line, int dimensions, int lineNo) {
	// create vector to hold parsed data, counter for current index, and other temp variables
	std::vector<int> lineData(dimensions);
	lineData[0] = lineNo;
	int counter = 1;
	bool prevIsData = false;
	std::string currentNumber;
	// loop through characters in line
	for (char c : line) {
		// check if current character is a space
		if (c == ' ') {
			// if at end of a number: add to dataset, clear current number, and increment counter
			if (prevIsData) {
				// add current number to vector
				lineData[counter] = std::stoi(currentNumber);
				currentNumber = "";
				prevIsData = false;
				counter++;
				// end if all dimensions are filled
				if (counter > dimensions) {
					break;
				}
			}
		}
		else {
			// add character to current number
			currentNumber += c;
			prevIsData = true;
		}
	}
	return lineData;
}

std::vector<int> parseLineCaparaToth(std::string line, int dimensions) {
	// create vector to hold parsed data, counter for current index, and other temp variables
	std::vector<int> lineData(dimensions);
	int counter = 0;
	bool prevIsData = false;
	std::string currentNumber;
	// loop through characters in line
	for (char c : line) {
		// check if current character is a space
		if (c == ' ') {
			// if at end of a number: add to dataset, clear current number, and increment counter
			if (prevIsData) {
				// if first number (line number), subract 1 to match vectors' index
				if (counter > 0) {
					lineData[counter] = std::stoi(currentNumber);
				}
				else {
					lineData[counter] = std::stoi(currentNumber) - 1;
				}
				currentNumber = "";
				prevIsData = false;
				counter++;
			}
		}
		else {
			// add character to current number
			currentNumber += c;
			prevIsData = true;
		}
	}
	// add final number to dataset, as there is no space at the end of the line
	lineData[counter] = std::stoi(currentNumber);
	return lineData;
}