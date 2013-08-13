#ifndef __banker_tcc__
#define __banker_tcc__
#include <algorithm>
using std::transform;
#include <vector>
using std::vector;

#include "banker.hpp"

template<typename _Arg>
void BankerSystem::AddProcess(_Arg &&p)
{
	size_t n_res;
	assert((n_res = p.vMax.size()) == Types());

	if(!canAllocate(p.vMax,vTotal))
	{
#ifdef DEBUG
		std::cerr << "Process max claim greater than total resources available\n";
#endif
		return;
	}

	p.vNeed.resize(n_res,0);

	if(p.vAllocated.size() == 0)
	{
		p.vAllocated.resize(n_res,0);
		p.vNeed = p.vMax;
	} else {
		assert(p.vAllocated.size() == n_res);
	
		if(!canAllocate(p.vAllocated, vAvailable))
		{
#ifdef DEBUG
			std::cerr << "Process requesting allocated resources more than available.\n";
			return;
#endif
		}

		std::transform(p.vNeed.begin(),p.vNeed.end(),p.vMax.begin(),p.vAllocated.begin(),std::minus<uint16_t>());
	}

	mProcess.push_back(std::forward<_Arg>(p));
}
#endif


