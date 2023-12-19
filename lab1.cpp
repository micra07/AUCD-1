#include <iostream>
#include <string>
#include <random>
#include <utility>
template<typename T>
class Set {
public:


	Set() :_data(nullptr), _size(0) {}//

	Set(T* data, int size) //
	{
		int* new_size = new int(0);

		_data = unique_data(data, size, new_size);

		_size = *new_size;

		this->sort();

		delete new_size;
	}

	Set(int size, const T begin, const T end) : _size(size), _data(new T[size])//
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(begin, end);
		for (int i = 0; i < size; ++i) {
			auto value = 0.0;
			int index = 0;
			while (index != -1) {
				value = dis(gen);
				index = element_is_there(_data, _size, value);
			}
			_data[i] = value;
		}
		this->sort();
	}

	Set(const Set& set) :_size(set.get_size()), _data(new T[set.get_size()]) 
	{
		for (int i = 0; i < set.get_size(); i++)
		{
			_data[i] = set[i];
		}
	}
	/////////
	Set& operator=(const Set& set) 
	{
		Set copy(set);
		swap(copy);
		return *this;
	}

	bool operator==(const Set& set) 
	{
		if (_size != set.get_size()) return false;
		for (int i = 0; i < _size; i++)
		{
			if (std::abs(_data[i] - set[i]) >= epsilon) return false;
		}
		return true;
	}

	bool operator!=(const Set& set)//
	{
		return !(*this == set);
	}

	Set<T> intersection(const Set<T>& set) //
	{
		auto dif = *this - set;

		return (*this - dif);
	}

	void swap(Set<T>& set) noexcept //
	{
		std::swap(_data, set.get_data());
		std::swap(_size, set.get_size());
	}

	T& operator[](int index) //
	{
		if (index < 0 || index >= _size) {
			throw std::out_of_range("MySet::operator[], index is out of range");
		}
		return _data[index];
	}

	const T& operator[](int index) const 
	{
		if (index < 0 || index >= _size) {
			throw std::out_of_range("MySet::operator[] const, index is out of range");
		}
		return _data[index];
	}

	Set& operator+=(const Set& set) //
	{
		int new_size = _size + set.get_size();

		T* new_data = new T[new_size]();

		for (int i = 0; i < _size; ++i)
		{
			new_data[i] = _data[i];
		}

		int j = 0;

		for (int i = _size; i < new_size; ++i)
		{
			new_data[i] = set[j];
			++j;
		}

		delete[] _data;

		int* actual_size = new int(0);

		_data = unique_data(new_data, new_size, actual_size);

		delete[] new_data;

		_size = *actual_size;

		this->sort();

		return *this;
	}

	Set& operator-=(const Set& set) 
	{
		for (int i = 0; i < set.get_size(); i++)
		{
			int index = element_is_there(_data, _size, set[i]);
			if (index != -1) {

				this->remove(index);
			}
		}
		return *this;
	}

	Set& operator+=(const T& value) //
	{
		if (element_is_there(_data, _size, value) != -1) {
			return *this;
		}
		int new_size = _size + 1;
		T* new_data = new T[new_size];
		for (int i = 0; i < _size; i++)
		{
			new_data[i] = _data[i];
		}
		new_data[new_size - 1] = value;
		delete[] _data;
		_data = new_data;
		_size = new_size;
		this->sort();
		return *this;
	}

	Set& operator-=(const T& value) //
	{
		int index = element_is_there(_data, _size, value);
		if (index == -1) {
			return *this;
		}
		this->remove(index);
		return *this;
	}

	int find(const T& value) const //
	{
		for (int i = 0; i < _size; i++)
		{
			if (_data[i] == value) return i;
		}
		return -1;
	}


private:
	int element_is_there(T* data, int size, const T& value) {
		for (int i = 0; i < size; i++)
		{
			if (data[i] == value) return i;
		}
		return -1;
	}

	T* unique_data(T* data, int size, int* new_size) {
		T* unique_data = new T[size]();
		int i = 0;
		int j = 0;
		while (i < size && j < size) {
			if (element_is_there(unique_data, *new_size, data[j]) == -1) {
				unique_data[i] = data[j];
				*new_size += 1;
				++i;
				++j;
			}
			else ++j;
		}
		return unique_data;
	}

	void sort() {
		for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _size - 1; j++)
			{
				if (_data[j] > _data[j + 1]) {
					T temp = _data[j];
					_data[j] = _data[j + 1];
					_data[j + 1] = temp;
				}
			}
		}
	}

	const double epsilon = 0.001;

	T* _data;

	int _size;
};
