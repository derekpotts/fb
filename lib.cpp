#include <fstream>

namespace fb
{

	long long vmsize_get()
	{
		std::ifstream stat("/proc/self/stat");
		std::string s;
		for(auto i=0; i<23; i++) stat >> s;
		auto byte_count = stoll(s);
		return byte_count / 1024.0;
	}

}
