#include "NumericalFileInfo.h"

#include <algorithm>
#include <fstream>
#include <iostream>

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

	initializeData(fileData);
}

std::ostream& operator<<(std::ostream& out, const NumericalFileInfo& right) {
	out.precision(14);
	out << "File info: " << std::endl;
	out << "Filepath: " << right.m_filepath << std::endl;
	out << "Data size: " << right.m_dataSize << std::endl;
	out << "Sum: " << right.m_sum << std::endl;
	out << "Max number: " << right.m_max << std::endl;
	out << "Min number: " << right.m_min << std::endl;
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

void NumericalFileInfo::initializeData(std::vector<data_t>& fileData) {
	m_dataSize = fileData.size();
	m_min = m_max = m_sum = fileData[0];

	// (C)LCIS - (Current) Longest Continuous Increasing Subsequence
	Sequence LCIS, CLCIS;
	// (C)LCDS - (Current) Longest Continuous Decreasing Subsequence
	Sequence LCDS, CLCDS;

	for (size_t i = 1; i < fileData.size(); ++i) {
		m_min = std::min(m_min, fileData[i]);
		m_max = std::max(m_max, fileData[i]);
		m_sum += fileData[i];

		updateSequence(fileData, i, LCIS, CLCIS, true);
		updateSequence(fileData, i, LCDS, CLCDS, false);
	}

	m_arithmeticMean = m_sum / static_cast<double>(m_dataSize);

	m_LCIS.insert(m_LCIS.end(), fileData.begin() + LCIS.m_begin, fileData.begin() + LCIS.m_end);
	m_LCDS.insert(m_LCDS.end(), fileData.begin() + LCDS.m_begin, fileData.begin() + LCDS.m_end);

	initializeMedian(fileData);
}

void NumericalFileInfo::updateSequence(const std::vector<data_t>& fileData, const size_t index, Sequence& longestSequence,
	Sequence& currentSequence, const bool isIncreasing) {
	if ((isIncreasing && fileData[index - 1] < fileData[index]) || (!isIncreasing && fileData[index - 1] > fileData[index])) {
		++currentSequence.m_end;
		if (currentSequence > longestSequence) {
			longestSequence = currentSequence;
		}
	}
	else {
		currentSequence = Sequence(index, index + 1);
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