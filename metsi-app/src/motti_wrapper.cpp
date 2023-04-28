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

Motti4State::Motti4State(StateReference<SimulationState> state) {
    control.calibrate = DISABLE_GROWTH_CALIBRATION;
    populate_site(state);
    populate_trees(state);
}

void Motti4State::populate_site(StateReference<SimulationState> state) {
    auto& stand = state->get_stand();
    yy.year = stand.get<float>("year");
    // TODO: assuming here that CRS is EPSG:3067. Do better.
    yy.Y = stand.get<float>("lat")/1000;
    yy.X = stand.get<float>("lon")/1000;
    yy.Z = stand.get<float>("masl");
    yy.dd = stand.get<float>("dd");
    yy.mal = stand.get<float>("land_use_category");
    yy.mty = stand.get<float>("site_type_category");
    yy.alr = stand.get<float>("soil_peatland_category");
    yy.verl = stand.get<float>("tax_class");
    yy.verlt = stand.get<float>("tax_class_reduction");
    yy.spedom = 2.0f; // TODO: dominant species not available
    yy.spedom2 = 2.0f; // TODO: dominant species not available
    yy.prt = 0.0f; // TODO: regeneration method not available
    yy.nstorey = 1.0f; // TODO: storeys not really available
}

void Motti4State::populate_trees(StateReference<SimulationState> state) {
    auto& dbhs = state->get_vars("tree#dbh");
    auto& hs = state->get_vars("tree#height");
    auto& stems = state->get_vars("tree#stems_ha");
    auto& spes = state->get_vars("tree#species");
    auto& ages = state->get_vars("tree#age");
    auto& age13s = state->get_vars("tree#age_bh");
    auto& origins = state->get_vars("tree#origin");
    auto& stand = state->get_stand();
    numtrees = stand.trees.size();
    float year = stand.get<float>("year");
    for(int i=0;i<numtrees;i++) {
        auto& tree = yp[i];
        tree.d13 = dbhs[i];
        tree.h = hs[i];
        tree.f = stems[i];
        tree.spe = spes[i];
        tree.age = year - ages[i];
        tree.age13 = year - age13s[i];
        tree.snt = origins[i];
        tree.storie = 0.0f; //TODO: no source for this
        tree.id = i+1;
    }
}

StateReference<SimulationState> grow_motti(StateReference<SimulationState> state, EventParameters params) {
    int step = 5;
    if(params.contains("step")) {
        step = boost::lexical_cast<int>(params["step"]);
    }
    auto& motti = MottiWrapper::get_instance();
    auto ms = Motti4State(state);

    motti.SiteInit(&ms.yy, &ms.yy.Y, &ms.yy.X, &ms.yy.Z, ms.rv);






    //TODO: state to Motti4Site, Motti4Tree etc here
    //...

    //TODO: call Motti 4 functions
    //...

    //TODO: results back to state
    //...

    return state;
}
