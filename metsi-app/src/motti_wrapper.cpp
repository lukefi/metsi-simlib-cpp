#include "motti_wrapper.hpp"

#ifdef WITH_MOTTI4
/**
 * Construct with the given motti4 library filename reference and populate the exposed
 * and known Motti4 functions.
 *
 * @param dl_filename_base the name of the library file without the dll/dylib/so suffix
 */
MottiWrapper::MottiWrapper(const std::string& filename) :
dylib(".", filename, false) {
    Init = get_function<decltype(Motti4::Motti4Init)>("Motti4SiteInit");
    SiteInit = get_function<decltype(Motti4::Motti4SiteInit)>("Motti4SiteInit");
	Growth = get_function<decltype(Motti4::Motti4Growth)>("Motti4SiteInit");
}

MottiWrapper& MottiWrapper::get_instance(const std::string& filename) {
    static MottiWrapper motti_lib(filename);
    return motti_lib;
}

#else
MottiWrapper::MottiWrapper(const std::string&) {
	throw std::exception("Application not built with Motti 4 library support.");
}
#endif
