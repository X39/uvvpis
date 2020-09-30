#include "binary.h"

#include <fstream>
#include <filesystem>

uvvpis::binary::result uvvpis::binary::load(std::string path)
{
	std::filesystem::path fpath(path);
	fpath = fpath.lexically_normal();

	if (!std::filesystem::exists(fpath))
	{
		return file_not_found;
	}
	std::ifstream file(fpath.c_str(), std::ios_base::binary | std::ios_base::in);
	if (!file.good())
	{
		return file_not_good;
	}
	if (!file.is_open())
	{
		return file_not_open;
	}
	auto filesize = std::filesystem::file_size(fpath);
	clear();
	resize(filesize);

	file.read(reinterpret_cast<char*>(data()), filesize);
	return ok;
}

uvvpis::binary::result uvvpis::binary::save(std::string path)
{
	std::filesystem::path fpath(path);
	fpath = fpath.lexically_normal();

	std::ofstream file(fpath.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
	if (!file.good())
	{
		return file_not_good;
	}
	if (!file.is_open())
	{
		return file_not_open;
	}

	file.write(reinterpret_cast<char*>(data()), size() * sizeof(instruction));
	return ok;
}
