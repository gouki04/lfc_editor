#pragma once
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

template<class T>
class TopNTable
{
public:
	std::vector<std::shared_ptr<T>> Keys;
	std::vector<int> Values;

public:
	TopNTable(int capacity)
	{
		Keys.resize(capacity, nullptr);
		Values.resize(capacity, std::numeric_limits<int>::lowest());
	}

	bool Push(std::shared_ptr<T> key, int value)
	{
		for (int i = 0; i < Values.size(); ++i) {
			if (value > Values[i]) {
				for (int j = Values.size() - 2; j >= i; --j) {
					Keys[j + 1] = Keys[j];
					Values[j + 1] = Values[j];
				}
				Keys[i] = key;
				Values[i] = value;
				return true;
			}
		}

		return false;
	}
};