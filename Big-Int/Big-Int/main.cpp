
/* Jim Viebke

Big Integer classes

Jul 31 2016 */

#include "big-int.h"

int main()
{
	// test 1 + 1
	Big_Int a(1);
	Big_Int b(1);
	std::cout << a + b << std::endl;

	// test (block maximum) + 1
	Big_Int c((uint32_t) - 1);
	std::cout << (c + 1) << std::endl;
	std::cout << (c + 2) << std::endl;
	std::cout << (c + 3) << std::endl;
	std::cout << (c + 4) << std::endl;
	std::cout << (c + 5) << std::endl;
}
