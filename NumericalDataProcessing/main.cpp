#include "Timer.h"
#include "NumericalFileInfo.h"

#include <iostream>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: data <file_path>" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		Timer timer;
		std::string filepath(argv[1]);
		NumericalFileInfo numericalFileInfo(filepath);

		std::cout << numericalFileInfo << std::endl;

		std::cout << std::endl << "Duration " << timer.getDuration() << "s" << std::endl;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return EXIT_SUCCESS;
}