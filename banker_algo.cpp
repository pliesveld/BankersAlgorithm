#include <algorithm>
using std::transform;
using std::all_of;
#include <functional>
using std::minus;
#include <set>
using std::set;

#include "banker.hpp"


bool canGrantRequest(BankerSystem &sys, uint32_t p_i,const ResourceList & request_i);


bool operator<=(const ResourceList &lhs, const ResourceList &rhs)
{
	size_t s,j;
	assert((s = lhs.size()) == rhs.size());
	for(j = 0;j < s;++j)
	{
		if( rhs[j] < lhs[j] )
			return false;
	}
	return true;
}


bool canAllocate(const ResourceList &p_req, const ResourceList &r_avail)
{
	return p_req <= r_avail;
}


bool isSafeState(BankerSystem &sys)
{
	struct c_unique {
		c_unique() : unique(0) { }
		uint16_t operator()() { return unique++; }
		uint16_t unique;
	} ProcessId;

	size_t p_len = sys.Processes();
	size_t r_len = sys.Types();

	std::vector<bool> Finish(p_len,false);

	std::vector<uint16_t> p_list(p_len);
	std::generate(p_list.begin(),p_list.end(),ProcessId);

	ResourceList Work(sys.vAvailable);

	std::set<uint16_t> p_set{p_list.begin(),p_list.end()};
	std::set<uint16_t>::iterator p_it = p_set.begin();

	for(;p_it != p_set.end();)
	{
		size_t P_i = *p_it;

#ifdef DEBUG
		std::cout << "Checking Process #" << P_i << std::endl;
#endif

		if( canAllocate(sys.mProcess[P_i].vNeed, Work) )
		{
			for(size_t j = 0;j < r_len;++j)
			{
				Work[j] += sys.mProcess[P_i].vAllocated[j];
			}
			Finish[P_i] = true;
#ifdef DEBUG
			std::cout << "Finished Process #" << P_i << std::endl;
#endif
			p_set.erase(p_it);
			p_it = p_set.begin();
			continue;
		}

		++p_it;
	}
	
#ifdef DEBUG
	std::cout << "Fin Process scan\n";
	std::cout << sys;
#endif

	return all_of(Finish.begin(),Finish.end(),[](bool fin) { return fin == true;}  );
}


bool canGrantRequest(BankerSystem &sys, uint32_t p_i)
{
	ResourceList claim(sys.mProcess[p_i].vMax);
	std::transform(claim.begin(),claim.end(),sys.mProcess[p_i].vAllocated.begin(),claim.begin(),std::minus<uint16_t>());
	return canGrantRequest(sys,p_i,claim);
}



// vector Request

// #1 if Request_i > Need_i
//			error; process p_i has exceeded maximum claim
//		else
//			goto #2


// #2 if Request_i <= Available_i 
//			goto #3
//		else
//			Resource not available, block p_i


// #3  copy system state to temp state;
//			Available -= Request_i
//			Allocatied_i += Request_i
//			Need_i -= Request_i
//
//			if temp sys.isSafeState()
//				allocate resources for p_i
//				commit temp state to sys state
///		else
//				block p_i on Request_i
//				discard temp state

bool canGrantRequest(BankerSystem &sys, uint32_t p_i,const ResourceList & request_i)
{

	if( !canAllocate(request_i, sys.mProcess[p_i].vNeed) )
	{
#ifdef DEBUG
		std::fprintf(stderr,"Process %u has exceeded maximum claim.\n",p_i);
		abort();
#endif
		return false;
	}

	if( canAllocate(request_i, sys.vAvailable) )
	{
		ResourceList t_available( sys.vAvailable );
		ResourceList t_allocated( sys.mProcess[p_i].vAllocated );
		ResourceList t_need( sys.mProcess[p_i].vNeed );

		size_t r_size = sys.Types();
		for(size_t j = 0;j < r_size;++j)
		{
			sys.vAvailable[j] -= request_i[j];
			sys.mProcess[p_i].vAllocated[j] += request_i[j];
			sys.mProcess[p_i].vNeed[j] -= request_i[j];
		}

		if( !isSafeState(sys) )
		{
			sys.vAvailable.swap(t_available);
			sys.mProcess[p_i].vAllocated.swap(t_allocated);
			sys.mProcess[p_i].vNeed.swap(t_need);
#ifdef DEBUG
			std::cout << "Unsafe-state;  cannot grant  " << p_i << std::endl;
			//debug_rl("block on request_i",request_i);
#endif
			return false;
		}

#ifdef DEBUG
		std::cout << "Allocated " << p_i << std::endl;
		//debug_rl("Resources",request_i);
#endif
		return true;
	} 

#ifdef DEBUG
	std::cout << "Not enough available resources for " << p_i << std::endl;
	//debug_rl("block because of request_i",request_i);
#endif
	return false;
}



