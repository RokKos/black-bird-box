#include "bbbpch.h"
#include "Util.h"

namespace Core {
	
	std::string Util::ReadFile(std::string file_path)
	{
		std::stringstream buffer;
		buffer << std::ifstream(file_path).rdbuf();
		return buffer.str();
	}

}