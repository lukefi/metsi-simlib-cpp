#include "sim_events.hpp"
#include "motti_wrapper.hpp"

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

StateReference<SimulationState> grow_motti(StateReference<SimulationState> state, EventParameters params) {
    int step = 5;
    if(params.contains("step")) {
        step = boost::lexical_cast<int>(params["step"]);
    }
    auto& motti = MottiWrapper::get_instance();

    //TODO: state to Motti4Site, Motti4Tree etc here
    //...

    //TODO: call Motti 4 functions
    //...

    //TODO: results back to state
    //...

    return state;
}
