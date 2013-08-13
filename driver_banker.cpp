#include <vector>
#include <cstdint>

#include <cstdlib>
#include <cassert>

using namespace std;

#include "banker.hpp"

int main()
{
	BankerSystem sys = ReadSystemState(cin);
	std::cout << sys << endl;

	if( isSafeState(sys) )
		std::cout << "SafeState\n";
	else
		std::cout << "UnSafeState\n";

	
}
