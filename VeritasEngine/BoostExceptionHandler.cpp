#include <exception>

namespace boost
{
#ifdef BOOST_NO_EXCEPTIONS
	void throw_exception(std::exception const & e)
	{
		
	}
#endif
}
