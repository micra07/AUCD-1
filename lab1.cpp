#include <iostream>
#include <string>
#include <random>
#include <utility>
template<typename T>
class Set {
public:


	Set() :_data(nullptr), _size(0) {} /

		Set(T * data, int size)
	{
		int* new_size = new int(0);

		_data = unique_data(data, size, new_size);

		_size = *new_size;

		this->sort();

		delete new_size;
	}

	Set(int size, const T begin, const T end) : _size(size), _data(new T[size])
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

	bool operator!=(const Set& set)
	{
		return !(*this == set);
	}

	Set<T> intersection(const Set<T>& set)
	{
		auto dif = *this - set;

		return (*this - dif);
	}

	void swap(Set<T>& set) noexcept
	{
		std::swap(_data, set.get_data());
		std::swap(_size, set.get_size());
	}

	T& operator[](int index)
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

	Set& operator+=(const Set& set)
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

	Set& operator+=(const T& value)
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

	Set& operator-=(const T& value)
	{
		int index = element_is_there(_data, _size, value);
		if (index == -1) {
			return *this;
		}
		this->remove(index);
		return *this;
	}

	int find(const T& value) const
	{
		for (int i = 0; i < _size; i++)
		{
			if (_data[i] == value) return i;
		}
		return -1;
	}

	int get_size() const
	{
		return _size;
	}

	int& get_size()
	{
		return _size;
	}

	const T* get_data() const
	{
		return _data;
	}

	T*& get_data()
	{
		return _data;
	}

	void remove(int index)
	{
		if (index < 0 || index >= _size) {
			throw std::out_of_range("MySet::remove, index is out of range");
		}
		for (int i = index; i < _size - 1; i++)
		{
			T temp = _data[i];
			_data[i] = _data[i + 1];
			_data[i + 1] = temp;
		}
		_size--;
	}

	void find_duplicates(T* set, int size)
	{
		bool found = false;
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (set[i] == set[j]) {
					found = true;
					std::cout << "Duplicate number: " << set[i] << std::endl;
				}
			}
		}
		if (!found) {
			std::cout << "There are no duplicate numbers in this set" << std::endl;
		}
	}


	template<typename R>
	Set operator+(const R& set) {
		Set copy(*this);
		return copy += set;
	}
	template<typename R>
	Set operator-(const R& set) {
		Set copy(*this);
		return copy -= set;
	}

	~Set() {
		delete[] _data;
		_size = 0;
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

template <>
Set<std::string>::Set(int size, std::string begin, std::string end) : _size(size), _data(new std::string[size])
{
	int asc_start = begin[0];
	int asc_end = end[0];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(asc_start, asc_end);
	for (int i = 0; i < _size; i++)
	{
		char* word = new char[6]();
		for (int j = 0; j < 5; j++)
		{
			word[j] = dis(gen);
		}
		word[6] = '\n';
		_data[i] = std::string(word);
	}
}

template <>
Set<std::pair<int, double>>::Set(int size, std::pair<int, double> begin, std::pair<int, double> end) : _size(size), _data(new std::pair<int, double>[size])
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis_first(begin.first, end.first);
	std::uniform_real_distribution<> dis_second(begin.second, end.second);
	for (int i = 0; i < size; i++)
	{
		_data[i] = std::make_pair(dis_first(gen), dis_second(gen));
	}
}

template<typename T, typename R>
std::ostream& operator<<(std::ostream& os, std::pair<T, R>& pair) {
	std::cout << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, Set<T>& set) {
	for (size_t i = 0; i < set.get_size(); i++)
	{
		std::cout << set[i] << " ";
		if (i == 9) std::cout << "\n";
	}
	return os;
}

using namespace std;

int main() {
	int iarray1[] = { 3, 3, 2, 5, 9, 6 ,6,2,1 };//6
	int iarray2[] = { 4, 5, 7, 8, 9 };//5
	int iarray3[] = { 1, 2, 3, 2, 5, 3, 4 };//5
	int iarray4[] = { 1, 7, 3, 4, 5, 6, 6, 1 };//6
	float farray1[] = { 1.12, 2.33, 1.45, 5.76, 2.33, 2.33, 9.875 };//7
	float farray2[] = { 1.34, 3.23, 1.45, 5.76, 8.34 };//5
	string sarray[] = { "peach", "pear", "apple", "pear" };//4
	pair<int, double> parray1[] = { make_pair(1, 1.12), make_pair(5, 3.12), make_pair(3, 7.12), make_pair(9, 4.12), make_pair(5, 3.12) };//5
	pair<int, double> parray2[] = { make_pair(3, 1.12), make_pair(5, 3.12), make_pair(1, 1.12), make_pair(5, 7.12), make_pair(6, 8.88) };//5

	auto iset1 = Set<int>(iarray1, 9);
	auto iset2 = Set<int>(iarray2, 5);

	auto iset3 = Set<int>(iarray3, 5);
	auto iset4 = Set<int>(iarray4, 6);

	auto fset1 = Set<float>(farray1, 7);
	auto fset2 = Set<float>(farray2, 5);

	auto sset1 = Set<string>(sarray, 3);

	auto pset1 = Set<pair<int, double>>(parray1, 5);
	auto pset2 = Set<pair<int, double>>(parray2, 5);

	std::cout << "Set of int" << "\n";
	std::cout << iset1 << "\n\n";
	std::cout << "Set of float" << "\n";
	std::cout << fset1 << "\n\n";
	std::cout << "Set of string" << "\n";
	std::cout << sset1 << "\n\n";
	std::cout << "Set of pair" << "\n";
	std::cout << pset1 << "\n\n";

	std::cout << "Sum of sets(int):" << "\n";
	std::cout << iset1 << " + " << iset2 << " = ";
	auto sum = iset1 + iset2;
	std::cout << sum;
	std::cout << "\n\n";

	std::cout << "Difference of sets(float):" << "\n";
	std::cout << fset1 << " - " << fset2 << " = ";
	auto dif = fset1 - fset2;
	std::cout << dif;
	std::cout << "\n\n";

	std::cout << "+= and -= with string set:\n";
	std::cout << sset1 << "\n";
	sset1 += "orange";
	sset1 -= "apple";
	std::cout << sset1;
	std::cout << "\n\n";

	std::cout << "Intersection of sets(int):" << "\n";
	std::cout << iset1 << ",  " << iset2 << " = ";
	auto inter_int = iset1.intersection(iset2);
	std::cout << inter_int;
	std::cout << "\n\n";

	std::cout << "Relation of sets(int):" << "\n";
	std::cout << iset4 << ",  " << iset3 << endl;
	std::cout << "\n\n";

	std::cout << "Find duplicates in set(int):" << "\n";
	std::cout << "{ 1, 2, 3, 4, 5, 3, 2 } " << "\n";
	iset3.find_duplicates(iarray3, 7);
	std::cout << "\n\n";

	std::cout << "No find duplicates in set(int):" << "\n";
	std::cout << "{ 4, 5, 7, 8, 9 } " << "\n";
	iset2.find_duplicates(iarray2, 5);
	std::cout << "\n\n";

	std::cout << "random c-or: \n";
	auto rset1 = Set<int>(10, 1, 100);
	std::cout << rset1;

	return 0;
}
