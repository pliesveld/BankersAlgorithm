#ifndef __banker_hpp__
#define __banker_hpp__
#define DEBUG


#include <cstdint>
#include <cassert>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

#include <iterator>
using std::ostream_iterator;
using std::istream;

#include <vector>
using std::vector;

#include <utility>
using std::swap;

using ResourceList=std::vector<uint16_t>;

class Process
{
public:
	ResourceList vMax;
	ResourceList vAllocated;
	ResourceList vNeed; // Max_j - Allocated_j
};


class BankerSystem
{
public:
	std::vector<char>			cResourceType;
	ResourceList				vTotal;
	ResourceList				vAvailable;
	std::vector<Process>		mProcess;
public:
	template<typename _Arg>
	void AddProcess(_Arg&& p);

	void UpdateResourcesAvailable();


public:
	size_t Types() const;				//number of resource types
	size_t Processes() const;			//number of processes
};


bool canAllocate(const ResourceList &p_req, const ResourceList &r_avail);
bool isSafeState(BankerSystem &sys);
bool canGrantRequest(BankerSystem &sys, uint32_t p_i);


BankerSystem ReadSystemState(istream &is);
ostream& operator<<(ostream &os, BankerSystem &sys);
ostream& operator<<(ostream &os, Process &proc);
void debug_rl(const char *str, const ResourceList &rl);


#endif
