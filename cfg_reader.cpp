#include "cfg_reader.h"

void config::readCfg(const std::string& path_to_file)
{
	std::ifstream file;
	file.open(path_to_file);
	file >> filterName >> up >> left >> bottom >> right;
	file.close();
}
