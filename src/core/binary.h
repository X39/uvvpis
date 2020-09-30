#pragma once
#include "instruction.h"
#include <vector>
#include <string>

namespace uvvpis
{
	class binary : public std::vector<instruction>
	{
	public:
		enum result
		{
			ok = 0,
			file_not_found,
			file_not_good,
			file_not_open
		};

		// Loads the contents of the given path into this binary.
		// Any prior contents are cleared.
		result load(std::string path);
		// Saves the contents of this binary at the given path.
		// If file already exists, contents will be overriden.
		result save(std::string path);
	};
}