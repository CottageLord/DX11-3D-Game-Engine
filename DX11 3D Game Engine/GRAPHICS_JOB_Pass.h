#pragma once
#include "SYS_CLASS_Graphics.h"
#include "GRAPHICS_JOB_Job.h"
#include <vector>

class Pass
{
public:
	void Accept(Job job) noexcept
	{
		jobs.push_back(job);
	}
	void Execute(Graphics& gfx) const noxnd
	{
		for (const auto& j : jobs)
		{
			j.Execute(gfx);
		}
	}
	void Reset() noexcept
	{
		jobs.clear();
	}
private:
	std::vector<Job> jobs;
};