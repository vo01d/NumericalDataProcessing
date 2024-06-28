#ifndef _NUMERICAL_FILE_INFO_
#define _NUMERICAL_FILE_INFO_

#include <string>
#include <vector>

class NumericalFileInfo {
public:
	using data_t = long long;

	NumericalFileInfo(std::string filepath);
	friend std::ostream& operator <<(std::ostream& out, const NumericalFileInfo& right);
	// other methods
	// ...
private:
	std::string m_filepath;
	size_t m_dataSize;
	data_t m_min;
	data_t m_max;
	double m_median;
	data_t m_sum;
	double m_arithmeticMean;
	std::vector<data_t> m_LCIS;
	std::vector<data_t> m_LCDS;

	void initializeData(std::vector<data_t>& fileData);
	size_t getSequenceSize(const std::pair<size_t, size_t>& sequence);
};

#endif 