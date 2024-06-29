#include "NumericalFileInfo.h"

#include <algorithm>
#include <iostream>
#include <fstream>

NumericalFileInfo::NumericalFileInfo(std::string filepath) : m_filepath(filepath) {
	std::ifstream inputFile(m_filepath);
	if (!inputFile.is_open()) {
		throw std::runtime_error("Error: Unable to open file!");
	}

	std::vector<data_t> fileData;

	data_t inputNumber;
	while (inputFile >> inputNumber) {
		fileData.push_back(inputNumber);
	}

	if (fileData.empty()) {
		throw std::runtime_error("Error: File is empty!");
	}

	initialize(fileData);
}

std::ostream& operator<<(std::ostream& out, const NumericalFileInfo& right) {
	out.precision(13);
	out << "File info: " << std::endl;
	out << "Filepath: " << right.m_filepath << std::endl;
	out << "Data size: " << right.m_dataSize << std::endl;
	out << "Sum: " << right.m_sum << std::endl;
	out << "Min number: " << right.m_min << std::endl;
	out << "Max number: " << right.m_max << std::endl;
	out << "Median: " << right.m_median << std::endl;
	out << "Arithmetic mean: " << right.m_arithmeticMean << std::endl;
	out << "Longest Continuous Increasing Subsequence size: " << right.m_LCIS.size() << std::endl;
	out << "LCIS: ";
	for (const auto& number : right.m_LCIS) {
		out << number << " ";
	}
	out << std::endl << "Longest Continuous Decreasing Subsequence size: " << right.m_LCDS.size() << std::endl;
	out << "LCDS: ";
	for (const auto& number : right.m_LCDS) {
		out << number << " ";
	}

	return out;
}

void NumericalFileInfo::initialize(std::vector<data_t>& fileData) {
	m_dataSize = fileData.size();
	m_min = m_max = m_sum = fileData[0];

	// (C)LCIS - (Current) Longest Continuous Increasing Subsequence
	Range LCIS, CLCIS;
	// (C)LCDS - (Current) Longest Continuous Decreasing Subsequence
	Range LCDS, CLCDS;

	for (size_t index = 1; index < fileData.size(); ++index) {
		m_min = std::min(m_min, fileData[index]);
		m_max = std::max(m_max, fileData[index]);
		m_sum += fileData[index];

		trackLCS(fileData, index, LCIS, CLCIS, true);
		trackLCS(fileData, index, LCDS, CLCDS, false);
	}

	m_arithmeticMean = m_sum / static_cast<double>(m_dataSize);

	m_LCIS.assign(fileData.begin() + LCIS.m_begin, fileData.begin() + LCIS.m_end);
	m_LCDS.assign(fileData.begin() + LCDS.m_begin, fileData.begin() + LCDS.m_end);

	initializeMedian(fileData);
}

void NumericalFileInfo::trackLCS(const std::vector<data_t>& fileData, const size_t index, Range& LCS,
	Range& CLCS, const bool isSubsequenceIncreasing) {
	if ((isSubsequenceIncreasing && fileData[index - 1] < fileData[index]) 
		|| (!isSubsequenceIncreasing && fileData[index - 1] > fileData[index])) {
		++CLCS.m_end;
		if (CLCS > LCS) {
			LCS = CLCS;
		}
	}
	else {
		CLCS = Range(index, index + 1);
	}
}

void NumericalFileInfo::initializeMedian(std::vector<data_t>& fileData) {
	std::nth_element(fileData.begin(), fileData.begin() + m_dataSize / 2, fileData.end());
	if (m_dataSize % 2 == 0) {
		std::nth_element(fileData.begin(), fileData.begin() + (m_dataSize - 1) / 2, fileData.end());
		m_median = (fileData[(m_dataSize - 1) / 2] + fileData[m_dataSize / 2]) / 2.0;
	}
	else {
		m_median = fileData[m_dataSize / 2];
	}
}