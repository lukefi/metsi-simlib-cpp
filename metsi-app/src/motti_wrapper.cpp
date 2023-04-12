#include "motti_wrapper.hpp"

#ifdef WITH_MOTTI4
MottiWrapper::MottiWrapper(const std::string& dl_filename_base) : motti_lib(dylib(".", dl_filename_base)) {
	this->Motti4SiteInit = this->motti_lib.get_function<void(float*, float*, float*, float*, int*)>("Motti4SiteInit");
}
#else
MottiWrapper::MottiWrapper(const std::string&) {
	throw std::exception("Application not built with Motti 4 library support.");
}
#endif
