#ifndef _NUMERICAL_FILE_INFO_
#define _NUMERICAL_FILE_INFO_

#include <string>
#include <vector>

class NumericalFileInfo {
public:
	using data_t = long long;

	NumericalFileInfo(std::string);
	friend std::ostream& operator <<(std::ostream&, const NumericalFileInfo&);
	// other methods
	// ...
private:
	struct Sequence {
		Sequence(size_t begin = 0, size_t end = 1) : m_begin(begin), m_end(end) {
		}
		friend bool operator >(const Sequence& left, const Sequence& right) {
			return (left.m_end - left.m_begin) > (right.m_end - right.m_begin);
		}
		friend bool operator <(const Sequence& left, const Sequence& right) {
			return !(left > right);
		}
		size_t m_begin;
		size_t m_end;
	};

	std::string m_filepath;
	size_t m_dataSize;
	data_t m_min;
	data_t m_max;
	double m_median;
	data_t m_sum;
	double m_arithmeticMean;
	std::vector<data_t> m_LCIS;
	std::vector<data_t> m_LCDS;

	void initializeData(std::vector<data_t>&);
	void updateSequence(const std::vector<data_t>&, const size_t, Sequence&, Sequence&, const bool);
	void initializeMedian(std::vector<data_t>&);
};

#endif 