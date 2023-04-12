#ifndef MOTTI_WRAPPER_HPP
#define MOTTI_WRAPPER_HPP

#include <dylib.hpp>
#include <functional>

using f_Motti4SiteInit = std::function<void(float*, float*, float*, float*, int*)>;

struct MottiWrapper {

	dylib motti_lib;
	f_Motti4SiteInit Motti4SiteInit;

	explicit MottiWrapper(const std::string&);
};

#endif