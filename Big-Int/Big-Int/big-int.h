#pragma once

#include <iostream>
#include <vector>
#include <sstream>

class Big_Int
{
	// public:
	using block = __int32; // store data in the second-largest primitive datatype, so we can manipulate data in the largest

private:
	using datum = __int64;
	static const unsigned block_max = (block)-1; // the largest value one block can hold
	static const unsigned block_size = sizeof(block) * 8; // the number of bits in a block
	static const unsigned block_count = 1000; // the number of blocks

	std::vector<block> data(0, block_count);

public:
	Big_Int() {}
	Big_Int(const block & block) // for the moment, no type larger than "block" can be used to initialize a Big_Int
	{
		data[block_count - 1] = block;
	}

	Big_Int(Big_Int&) = delete;

	Big_Int operator + (const Big_Int & a, const Big_Int & b)
	{
		Big_Int sum;

		for (unsigned i = block_count - 1; i > 0; --i)
		{
			const datum result = datum(a[i]) + b[i] + sum[i]; // a + b + carry

															  // if overflow needs to be handled
			if (result > block_max)
			{
				// store block_max, the remainer is carried
				sum[i] = block_max;

				// remove block_max  (the amount already stored) from result to calculate the carry over, then shift the result back in range
				// before shift:
				// [ 1101...1110xxxx...xxxx ] <-- (2*block_size)
				// after shift:
				//  0000...0000 [ 1101...1110 ] <-- (1*block_size)
				sum[i - 1] = (result - block_max) >> block_width;
			}
			// no overflow, just store the result
			else
			{
				sum[i] = result;
			}
		}

		return sum;
	}

	// this operates in base 4.2 billion
	std::ostream operator <<(std::ostream & out, const Big_Int & number) const
	{
		// seek until we find the first instance of data that is greater than 0
		unsigned start_block = 0;
		for (unsigned i = 0; i < block_count; ++i)
			if (number[i] > 1) { start_block = i; break; }

		// if no block is greater than 0, output a 0 and return
		if (start_index == bucket_count) return (out << 0);

		// seeking from the start block onwards, accumulate data into the stream
		// this could be optimized by pre-calculating the size of a string required to store the output before piping it to the output stream.
		std::stringstream ss;
		for (unsigned i = start_block; i < block_count; ++i)
			ss << " " << number[i];

		return (out << ss);
	}

private:
	inline block& operator[](const unsigned & i) { return data[i]; } // forward operator[] to the underlying container
	inline const block& operator[](const unsigned & i) const { return data[i]; } // forward operator[] to the underlying container

};

// test 1 + 1
Big_Int a(1);
Big_Int b(1);
std::cout << a + b << std::endl;

// test (block maximum) + 1
Big_Int c((Bug_Number::block) - 1);
std::cout << c + a << std::endl;
