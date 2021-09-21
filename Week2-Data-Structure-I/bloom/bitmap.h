#pragma once


class Bitmap
{
public:
	Bitmap()
		: _size(0)
	{}

	Bitmap(size_t size)
		:_size(0)
	{
		_array.resize((size >> 5) + 1);
	}
	void Resize(size_t size)
	{
		_array.resize((size >> 5) + 1);
	}
	void Set(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (_array[index] & (1 << i))
		{
			return;
		}
		else
		{
			_array[index] |= (1 << i);
			_size++;
		}
	}

	void Reset(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (_array[index] & (1 << i))
		{
			_array[index] &= ~(1 << i);
			_size--;
		}
		else
		{
			return;
		}
	}

	bool Test(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;

		return _array[index] & (1 << i);
	}

private:
	vector<size_t> _array;
	size_t _size;
};