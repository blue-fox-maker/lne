#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>

class PHC_index
{
private:
	std::vector<std::vector<std::vector<std::pair<int, int>>>> data;

public:
	static PHC_index load_file(std::string file_path);
	size_t num_vert() const;
	bool search(size_t vert, size_t k, size_t ts, size_t te);
	std::set<size_t> search(size_t k, size_t ts, size_t te);
	void display(bool detail) const;
};

size_t PHC_index::num_vert() const
{
	return data.size();
}

PHC_index PHC_index::load_file(std::string file_path)
{
	std::cout << "-------------------- Loading PHC from file" << std::endl;
	PHC_index phc;
	auto ifs = std::fstream(file_path, std::ios::in);
	size_t num_vert;
	ifs >> num_vert;
	std::cout << "number of vertex: " << num_vert << std::endl;
	phc.data.resize(num_vert);
	for (size_t i = 0; i < num_vert; i++)
	{
		size_t vert, num_line;
		ifs >> vert >> num_line;
		phc.data[vert] = std::vector<std::vector<std::pair<int, int>>>(num_line);
		for (size_t j = 0; j < num_line; j++)
		{
			size_t num_interval;
			ifs >> num_interval;
			phc.data[vert][j] = std::vector<std::pair<int, int>>(num_interval);
			for (size_t k = 0; k < num_interval; k++)
			{
				size_t ts, te;
				ifs >> ts >> te;
				phc.data[vert][j][k] = std::make_pair(ts, te);
			}
		}
	}
	return phc;
}

bool PHC_index::search(size_t vert, size_t k, size_t ts, size_t te)
{
	if (vert < data.size() - 1 && k >= 2 && k < data[vert].size() + 2)
	{
		auto intervals = data[vert][k - 2];
		auto interval = *std::prev(
			std::upper_bound(intervals.begin(), intervals.end(), ts,
							 [](size_t value, auto x)
							 {
								 return value < x.first;
							 }));
		return te >= interval.second;
	}
	return false;
}

std::set<size_t> PHC_index::search(size_t k, size_t ts, size_t te)
{
	std::cout << "-------------------- Searching " << k << "-core in [" << ts << ", " << te << "]" << std::endl;
	std::set<size_t> result;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (search(i, k, ts, te))
			result.insert(i);
	}
	std::cout << "The result is: ";
	for (auto v : result)
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;
	return result;
}

void PHC_index::display(bool detail = true) const
{
	std::cout << "-------------------- PHC structure display" << std::endl;
	size_t size = 0;
	for (auto &&vert : data)
		for (auto &&line : vert)
			size += sizeof(line);
	std::cout << "total size: " << size << " Byte" << std::endl;
	if (detail)
		for (size_t v = 0; v < data.size(); v++)
		{
			std::cout << "vert " << v << ":\n";
			for (size_t k = 0; k < data[v].size(); k++)
			{
				std::cout << "(k=" << (k + 2) << "): ";
				for (auto &&interval : data[v][k])
					std::cout << "[" << interval.first << ", " << interval.second << "] ";
				std::cout << std::endl;
			}
		}
	std::cout << std::endl;
}

// class LNE_index
// {
// private:
// 	std::vector<std::vector<std::pair<std::set<size_t>, std::vector<std::tuple<size_t, size_t, size_t, size_t>>>>> data;
// 	static LNE_index load_file(std::string file_path);
// 	void display();
// 	std::set<size_t> search(size_t k, size_t ts, size_t te);
// };

// LNE_index LNE_index::load_file(std::string file_path)
// {
// 	std::cout << "-------------------- Loading LNE from file" << std::endl;
// 	LNE_index lne;
// 	auto ifs = std::fstream(file_path, std::ios::in);
// 	size_t num_k;
// 	ifs >> num_k;
// 	lne.data.resize(num_k);
// 	for (size_t k = 0; k < num_k; k++)
// 	{
// 		size_t num_core;
// 		ifs >> num_core;
// 		lne.data[k - 2].resize(num_core);
// 		for (size_t i = 0; i < num_core; i++)
// 		{
// 			size_t num_vert, num_region;
// 			ifs >> num_vert >> num_region;
// 			auto core = std::set<size_t>{};
// 			for (size_t j = 0; j < num_vert; j++)
// 			{
// 				size_t cur_vert;
// 				ifs >> cur_vert;
// 				core.insert(cur_vert);
// 			}
// 			auto regions = std::vector<std::tuple<size_t, size_t, size_t, size_t>>(num_region);
// 			for (size_t j = 0; j < num_region; j++)
// 			{
// 				size_t x1, y1, x2, y2;
// 				ifs >> x1 >> y1 >> x2 >> y2;
// 				regions.push_back({x1, y1, x2, y2});
// 			}
// 			lne.data[k - 2][i].second.push_back(std::make_pair(core, regions));
// 		}
// 	}
// 	return lne;
// }

// void LNE_index::display()
// {
// 	std::cout << "-------------------- LNE index display" << std::endl;
// 	for (size_t k = 0; k < data.size(); k++)
// 	{
// 		std::cout << "(k=" << k + 2 << ")" << std::endl;
// 		for (auto &&pair : data[k])
// 		{
// 			std::cout << "num of cores: " << data[k].size() << std::endl;
// 			auto core = pair.first;
// 			auto regions = pair.second;
// 			std::cout << "core:";
// 			for (auto &&v : core)
// 				std::cout << " " << v;
// 			std::cout << "\n";
// 			for (auto &&region : regions)
// 			{
// 				auto [x1, y1, x2, y2] = region;
// 				std::cout << "[" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << "] ";
// 			}
// 			std::cout << "\n";
// 		}
// 	}
// }