#include <iomanip>
#include <sstream>
using std::ostringstream;
using std::setw;

#include "banker.hpp"

using namespace std;


BankerSystem ReadSystemState(istream &is)
{
	BankerSystem sys;
	std::vector<char> r_types;

	while( !is.eof() && !is.fail() && is.peek() == '#')
		is.ignore(500,'\n');

	char c = '\0';
	while( !is.eof() && !is.fail() && is.peek() != '\n' && (is >> c) )
	{
		if( c >= 'A' && c <= 'Z' )
		{
			r_types.push_back(c);
		} else {
			cerr << "Expected resource type.\n";
			abort();
		}
	}

	is.ignore(500,'\n');

	size_t nTypes = r_types.size();

	sys.cResourceType.swap( r_types );

	while( !is.eof() && !is.fail() && is.peek() == '#')
		is.ignore(500,'\n');

	ResourceList r_count;
	r_count.reserve( nTypes );

	int value = -1;
	while ( !is.eof() && !is.fail() && is.peek() != '\n' && (is >> value) )
	{
		size_t s;
		if( (s = r_count.size()) > nTypes )
		{
			cerr << "Too many values; expected " << nTypes << " got " << s << endl;
			abort();
		} else if ( value < 0 ) {
			cerr << "Invalid resource count\n";
			abort();
		}

		r_count.push_back( value );
		value = -1;
	}


	sys.vTotal.swap( r_count );
	is.ignore(500,'\n');

	while( !is.eof() && !is.fail() && is.peek() == '#')
		is.ignore(500,'\n');

	while( !is.eof() && !is.fail())
	{
		ResourceList r_limits;
		r_limits.reserve( nTypes * 2 );
		value = -1;
		while ( !is.eof() && !is.fail() && is.peek() != '\n' && (is >> value) )
		{
			size_t s;
			if( ( s = r_limits.size()) > nTypes*2 )
			{
				cerr << "Too many values; expected " << nTypes*2 << " got " << s << endl;
				abort();
			}

			r_limits.push_back( value );
		}


		if( r_limits.size() == 0 )
			break;

		assert( r_limits.size() == nTypes*2 );

		ResourceList r_claim(r_limits.begin() + nTypes, r_limits.end());

		r_limits.resize( nTypes );


		Process p_i;
		p_i.vAllocated.swap( r_limits );
		p_i.vMax.swap( r_claim );
		sys.mProcess.emplace_back(p_i);

		is.ignore(500,'\n');
	}

	
	sys.UpdateResourcesAvailable();
	return sys;
}


ostream& operator<<(ostream &os, BankerSystem &sys)
{

	size_t nTypes;
	assert( (nTypes = sys.cResourceType.size()) == sys.vTotal.size() );


	ostringstream buf_h;
	ostringstream buf_d;
	ostringstream buf_a;

	size_t i = 0;;
	for(i = 0; i < nTypes - 1;++i)
	{
		buf_h << setw(4) << std::right << sys.cResourceType[i] << ", ";
		buf_d << setw(4) << std::right << sys.vTotal[i] << ", ";
		buf_a << setw(4) << std::right << sys.vAvailable[i] << ", ";
	}

	buf_h << setw(4) << std::right << sys.cResourceType[i];
	buf_d << setw(4) << std::right << sys.vTotal[i];
	buf_a << setw(4) << std::right << sys.vAvailable[i];

	os << std::setw(18) << std::left << "Resource Types" << buf_h.str() << endl << std::setw(18) << std::left << "Total" << buf_d.str() << endl << std::setw(18) << std::left << "Available" << buf_a.str() << endl;


	size_t nP = 0;
	os << std::left << setw(12) << " " << std::right << buf_h.str() << std::endl;

	for( auto& p_i : sys.mProcess )
	{
		os << setw(8) << std::left << "Process" << setw(6) << std::right << nP++ << std::endl;
		os << p_i;
	}

	return os;
	
}



ostream& operator<<(ostream &os, Process &proc)
{
	ostringstream buf_p;
	ostringstream buf_al;
	ostringstream buf_ma;
	ostringstream buf_ne;

	//buf_p << setw(24) << std::left << "Process: ";
	buf_al << setw(12) << std::left << "Allocated: ";
	buf_ma << setw(12) << std::left << "Max: ";
	buf_ne << setw(12) << std::left << "Need: ";

	size_t j = 0, nTypes = proc.vMax.size();

	for(j = 0;j < nTypes - 1;++j)
	{
		buf_ma << setw(4) << std::right << proc.vMax[j] << setw(2) << ", ";
		buf_al << setw(4) << std::right << proc.vAllocated[j] << setw(2) << ", " ;
		buf_ne << setw(4) << std::right << proc.vNeed[j] << setw(2) << ", ";
	}

	buf_ma << setw(4) << std::right << proc.vMax[j];
	buf_al << setw(4) << std::right << proc.vAllocated[j];
	buf_ne << setw(4) << std::right << proc.vNeed[j];


	//os << buf_p.str() << endl
	os		<<  buf_ma.str() << endl
			<< buf_al.str()  << endl
			<< buf_ne.str() << endl;
	return os;
}



void debug_rl(const char *str, const ResourceList &rl)
{
	std::cerr << str;
	for(auto &i : rl)
		std::cerr << ' ' << i;
	std::cerr << std::endl;
}
