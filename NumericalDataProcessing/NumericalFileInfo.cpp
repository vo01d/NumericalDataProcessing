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
	std::pair<size_t, size_t> LCIS(0, 1), CLCIS(0, 1);
	// (C)LCDS - (Current) Longest Continuous Decreasing Subsequence
	std::pair<size_t, size_t> LCDS(0, 1), CLCDS(0, 1);

	for (size_t i = 1; i < fileData.size(); ++i) {
		m_min = std::min(m_min, fileData[i]);
		m_max = std::max(m_max, fileData[i]);
		m_sum += fileData[i];

		if (fileData[i - 1] < fileData[i]) {
			++CLCIS.second;
			if (getSequenceSize(CLCIS) > getSequenceSize(LCIS)) {
				LCIS = CLCIS;
			}
		}
		else {
			CLCIS = std::pair<size_t, size_t>(i, i + 1);
		}

		if (fileData[i - 1] > fileData[i]) {
			++CLCDS.second;
			if (getSequenceSize(CLCDS) > getSequenceSize(LCDS)) {
				LCDS = CLCDS;
			}
		}
		else {
			CLCDS = std::pair<size_t, size_t>(i, i + 1);
		}
	}

	m_arithmeticMean = m_sum / static_cast<double>(m_dataSize);

	m_LCIS.insert(m_LCIS.end(), fileData.begin() + LCIS.first, fileData.begin() + LCIS.second);
	m_LCDS.insert(m_LCDS.end(), fileData.begin() + LCDS.first, fileData.begin() + LCDS.second);

	std::nth_element(fileData.begin(), fileData.begin() + m_dataSize / 2, fileData.end());
	if (m_dataSize % 2 == 0) {
		std::nth_element(fileData.begin(), fileData.begin() + (m_dataSize - 1) / 2, fileData.end());
		m_median = (fileData[(m_dataSize - 1) / 2] + fileData[m_dataSize / 2]) / 2.0;
	}
	else {
		m_median = fileData[m_dataSize / 2];
	}
}

size_t NumericalFileInfo::getSequenceSize(const std::pair<size_t, size_t>& sequence) {
	return sequence.second - sequence.first;
}