#include "motti_wrapper.hpp"

#ifdef WITH_MOTTI4
/**
 * Construct with the given motti4 library filename reference and populate the exposed
 * and known Motti4 functions.
 *
 * @param dl_filename_base the name of the library file without the dll/dylib/so suffix
 */
MottiWrapper::MottiWrapper(const std::string& filename) :
motti_lib(dylib(".", filename, false)) {
    this->Init = this->motti_lib.get_function<decltype(Motti4::Motti4Init)>("Motti4SiteInit");
    this->SiteInit = this->motti_lib.get_function<decltype(Motti4::Motti4SiteInit)>("Motti4SiteInit");
	this->Growth = this->motti_lib.get_function<decltype(Motti4::Motti4Growth)>("Motti4SiteInit");
}
#else
MottiWrapper::MottiWrapper(const std::string&) {
	throw std::exception("Application not built with Motti 4 library support.");
}
#endif
