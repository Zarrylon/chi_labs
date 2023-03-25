#pragma once

template<typename T, size_t N>
class List
{
private:
	T* arr;
	int arrSize;

public:
	List() : arrSize(0), arr(new T[arrSize]) {}
	~List() { if (arr != nullptr) delete[] arr; }

	size_t size() const { return arrSize; }
	const T& operator[] (int index) const { return arr[index]; }

	void operator +=(const T& value)
	{
		T* tmp = new T[arrSize + 1];

		for (int i = 0; i < arrSize; i++)
		{
			tmp[i] = arr[i];
		}

		arrSize++;

		delete[] arr;
		arr = tmp;

		arr[arrSize - 1] = value;
	}
};