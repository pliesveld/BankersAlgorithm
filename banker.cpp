#include <algorithm>

#include "banker.hpp"
using namespace std;

size_t BankerSystem::Types() const
{
#ifdef DEBUG
	size_t cnt;
	assert( (cnt = cResourceType.size()) == vTotal.size() && cnt);
#endif
	return cnt;
}

size_t BankerSystem::Processes() const
{
	return mProcess.size();
}


void BankerSystem::UpdateResourcesAvailable()
{
	size_t nTypes = Types();
	vAvailable = vTotal;
	for(auto & p_i : mProcess)
	{
		p_i.vNeed.resize(nTypes,0);
		for(size_t j = 0;j < nTypes;++j)
		{
			vAvailable[j] -= p_i.vAllocated[j];
			p_i.vNeed[j] = p_i.vMax[j] - p_i.vAllocated[j];
		}
	}
}

