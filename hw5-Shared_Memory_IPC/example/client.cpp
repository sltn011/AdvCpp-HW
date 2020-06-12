#include "Shmem.h"
#include <iostream>
#include <string>
#include <sys/wait.h>

int main()
{
	HW::shmem::Map<std::string, std::string> map(1024);
	map.insert({"ABC", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"});
	int pid = fork();
	if (pid < 0) {
		return 1;
	}
	else if (pid == 0) {
		std::cout << "ABC" << " " <<  map["ABC"] << std::endl;
		map.insert({"123", "1234567891011121314151617181920"});
		map.set_value({"ABC", "ABBACDDCEFFEGHHGIJJIABBACDDCEFFEGHHGIJJI"});
	}
	else {
		wait(nullptr);
		std::cout << "123" << " " << map["123"] << std::endl;
		std::cout << "ABC" << " " <<  map["ABC"] << std::endl;
		map.erase("123");
		std::cout << "ABC" << " " <<  map["ABC"] << std::endl;
	}
	return 0;
}
