#ifndef MOTTI_WRAPPER_HPP
#define MOTTI_WRAPPER_HPP
#include <metsi-simlib/framework_util.hpp>
#include <string>
#include <vector>
#include <dylib.hpp>
#include <functional>
#include <motti4.hpp>
#include "state_model.hpp"
#include "metsi-simlib/core_types.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define MOTTI4_FILENAME "libmotti4dll.dll"
#elif defined(__APPLE__)
#define MOTTI4_FILENAME "libmotti4dll.dylib"
#else
#define MOTTI4_FILENAME "libmotti4dll.so"
#endif

/**
 * Singleton wrapper for a given Motti4 dynamic library file (dll/dylib/so). Exposes
 * key functions based on the motti4.hpp header declarations. Additional functions may
 * be exposed here when they become necessary.
 */
class MottiWrapper : protected dylib {
protected:
    explicit MottiWrapper(const std::string &);
public:
    static MottiWrapper& get_instance(const std::string& = MOTTI4_FILENAME);
    MottiWrapper(const MottiWrapper&) = delete;
    void operator=(const MottiWrapper&) = delete;

    std::function<decltype(Motti4::Motti4Init)> Init;
    std::function<decltype(Motti4::Motti4SiteInit)> SiteInit;
    std::function<decltype(Motti4::Motti4Growth)> Growth;
};

/**
 * This struct represents the data set necessary to use the Motti4SiteInit and Motti4Growth functions
 * with the purpose of growing reference trees with Motti4 growth model algorithms. Initiating the
 * structure is based on the work for using the library via Python FFI in
 * https://github.com/menu-hanke/pymotti
 */
struct Motti4State {
    Motti4::Motti4Site yy;
    Motti4::Motti4Trees yp;
    Motti4::Motti4Saplings ut;
    Motti4::Motti4KorArray kor;
    Motti4::Motti4KorArray apv;
    Motti4::Motti4VcrArray vcr;
    Motti4::Motti4Ctrl control;
    Motti4::Motti4FerArray fer;
    int numtrees = 0;
    int numfer = 0;
    int DISABLE_GROWTH_CALIBRATION = 53712;
    int rv[4];

    Motti4State(StateReference<SimulationState> state);
    void populate_site(StateReference<SimulationState> state);
    void populate_trees(StateReference<SimulationState> state);
};

StateReference<SimulationState> grow_motti(StateReference<SimulationState>, EventParameters);

static std::map<std::string, ParameterizedEventFn<SimulationState>> motti_events{
        {"grow", grow_motti},
        {"grow_motti", grow_motti}
};
#endif