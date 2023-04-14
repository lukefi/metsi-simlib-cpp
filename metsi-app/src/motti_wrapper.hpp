#ifndef MOTTI_WRAPPER_HPP
#define MOTTI_WRAPPER_HPP

#include <dylib.hpp>
#include <functional>
#include <motti4.hpp>

/**
 * This wrapper loads a given Motti4 dynamic library file (dll/dylib/so) and populates
 * key functions based on the motti4.hpp header declarations. Additional functions may
 * be exposed here when they become necessary.
 */
struct MottiWrapper {
    dylib motti_lib;
    std::function<decltype(Motti4::Motti4Init)> Init;
    std::function<decltype(Motti4::Motti4SiteInit)> SiteInit;
    std::function<decltype(Motti4::Motti4Growth)> Growth;

    explicit MottiWrapper(const std::string &);
};

#endif