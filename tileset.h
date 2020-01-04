#pragma once
struct CPairIntHash {
	inline std::size_t operator()(const std::pair<int, int> & v) const {
		return v.first * 31 + v.second;
	}
};
using CTileSet = std::unordered_set<std::pair<int, int>, CPairIntHash>;