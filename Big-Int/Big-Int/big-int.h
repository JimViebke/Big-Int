#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <bitset>
#include <iomanip>

class Big_Int
{
public:
	using block = uint32_t; // store data in the second-largest primitive datatype, so we can manipulate data in the largest

private:
	using block2 = uint64_t;
	static const unsigned block_max = (block)-1; // the largest value one block can hold
	static const unsigned block_bits = sizeof(block) * 8; // the number of bits in a block
	static const unsigned block_count = 1039; // 2^(32*1039) is long enough to handle all 10K-digit numbers

	std::vector<block> data;

public:
	Big_Int()
	{
		data = std::move(std::vector<block>(block_count, 0));
	}
	Big_Int(const block & block_value) : Big_Int() // for the moment, no type larger than "block" can be used to initialize a Big_Int
	{
		data[block_count - 1] = block_value; // set our initial value (range 0 to block_max)
	}

	Big_Int operator+(const Big_Int & rhs) const
	{
		Big_Int sum;

		for (unsigned i = block_count - 1; i > 0; --i)
		{
			// do the add using a datatype wide enough to never overflow
			const block2 result = block2(data[i]) + rhs[i] + sum[i]; // lhs + rhs + carry

			// if overflow needs to be handled
			if (result > block_max)
			{
				// the right half of the result is our new current value
				sum[i] = (result & sizeof block);

				// the left half of the result goes in block to the left
				sum[i - 1] = (result >> block_bits);
			}
			// no overflow, just store the result
			else
			{
				sum[i] = (block)result;
			}
		}

		return sum;
	}

	friend std::ostream& operator<<(std::ostream & out, const Big_Int & number);

private:
	inline block& operator[](const unsigned & i) { return data[i]; } // forward operator[] to the underlying container
	inline const block& operator[](const unsigned & i) const { return data[i]; } // forward operator[] to the underlying container

};

// this operates in base 4.2 billion
std::ostream& operator <<(std::ostream & out, const Big_Int & number)
{
	// seek until we find the first instance of data that is greater than 0
	unsigned start_block = 0;
	for (unsigned i = 0; i < Big_Int::block_count; ++i)
	{
		if (number[i] > 0)
		{
			start_block = i;
			break;
		}
	}

	// if no block is greater than 0, output a 0 and return
	if (start_block == Big_Int::block_count) return (out << 0);

	// seeking from the start block onwards, accumulate data into the stream
	// this could be optimized by pre-calculating the size of a string required to store the output before piping it to the output stream.
	std::stringstream ss;
	for (unsigned i = start_block; i < Big_Int::block_count; ++i)
		ss << " " << number[i];

	return (out << ss.str());
}
