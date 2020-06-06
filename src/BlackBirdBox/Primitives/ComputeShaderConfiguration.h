#pragma once

namespace Core {
	class ComputeShaderConfiguration
	{
	public:
		ComputeShaderConfiguration();
		ComputeShaderConfiguration(const std::array<unsigned int, 3>& work_group_size, const std::array<unsigned int, 3>& local_group_size);

		const std::array<unsigned int, 3>& GetWorkGroupSize() const { return work_group_size_; }
		const std::array<unsigned int, 3>& GetLocalGroupSize() const {return local_group_size_;}

	private:
		std::array<unsigned int, 3> work_group_size_;
		std::array<unsigned int, 3> local_group_size_;

	};
}