#include <vector>
#include <cstdint>

#include <cstdlib>
#include <cassert>

using namespace std;

#include "banker.hpp"
#include "banker.tcc"

int main()
{
	BankerSystem sys{{'A','B','C','D','E','F','G'},
						  {  1,  2,  3,  4,  5,  6,  7}};
	sys.UpdateResourcesAvailable();

	sys.AddProcess(Process{{0,2,0,0,0,2,7}});
	sys.AddProcess(Process{{0,0,0,4,5,2,0}});
	sys.AddProcess(Process{{1,0,3,0,0,2,0}});
	sys.AddProcess(Process{{1,0,0,0,0,1,0}});

	sys.UpdateResourcesAvailable();

	std::cout << sys << endl;

	if( isSafeState(sys) )
		std::cout << "SafeState\n";
	else
		std::cout << "UnSafeState\n";

	canGrantRequest(sys,0);
	canGrantRequest(sys,1);
	canGrantRequest(sys,2);
	std::cout << sys << endl;
	canGrantRequest(sys,3);
	std::cout << sys << endl;
}
