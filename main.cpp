#include <Wt/WApplication.h>
#include <Wt/WGlobal.h>
#include <memory>

#include "fb.h"

int main(int argc, char **argv)
{
	auto server = std::make_shared<fb::server>();

	auto fb_maker = [server](const Wt::WEnvironment& env)
	{
		return std::make_unique<fb::fb>(env, server.get());
	};

	return Wt::WRun(argc, argv, fb_maker);
}
