#include "bbbpch.h"
#include "ComputeShaderConfiguration.h"

namespace Core {
	ComputeShaderConfiguration::ComputeShaderConfiguration() :
		work_group_size_({1024,1024,1024}),
		local_group_size_({64, 64, 64})
	{
	}

	ComputeShaderConfiguration::ComputeShaderConfiguration(const std::array<unsigned int, 3>& work_group_size, const std::array<unsigned int, 3>& local_group_size) :
		work_group_size_(work_group_size),
		local_group_size_(local_group_size)
	{

	}

}