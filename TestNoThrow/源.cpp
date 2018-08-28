#include <iostream>
#include <new>

#include <thread>

int main()
{
	int n = 0;
	try {
		while (true) {
			new int[100000000ul];   // 抛出重载
			std::cout << n++ << std::endl;
		}
	}
	catch (const std::bad_alloc& e) {
		std::cout << e.what() << '\n';
	}

	while (true) {
		int* p = new(std::nothrow) int[100000000ul]; // 不抛出重载
		std::cout << n++ << std::endl;
		if (p == nullptr) {
			std::cout << "Allocation returned nullptr\n";
			break;
		}
	}
	getchar();
	return 0;
}