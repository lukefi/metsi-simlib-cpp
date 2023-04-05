//
// Created by Juho-Petteri Yliuntinen on 4.4.2023.
//

#include <numbers>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "domain_functions.hpp"

/**
 * Circle area with diameter d and scaling exponent. Default scale 0 assumes that the unit of d becomes unit squared.
 * Scale -1 scales d with factor 10^-1 (e.g. cm -> m, making the result m^2)
 * Scale -2 scales d with factor 10^-2 (e.g. dm -> m, making the result m^2)
 * @param d diameter
 * @param scale scaling exponent
 * @return
 */
float circle_area(float d, float scale) {
    float scaling_factor = std::pow(10.0f, scale);
    return std::numbers::pi * 0.5 * std::pow(scaling_factor*d*0.5, 2);
}

/**
 * Compute median value from given vector.
 * @param v
 * @return
 * @throws std::domain_error upon attempting to compute an empty vector
 */
float median(std::vector<float> v) {
    if(v.empty()) {
        throw std::domain_error("Can not get median of empty vector");
    }
    std::sort(v.begin(), v.end());
    if(v.size() % 2 == 0) {
        return (v[(v.size() -1) / 2] + v[v.size() / 2])/2;
    }
    else {
        return v[v.size()/2];
    }
}

/**
 * Compute the basal area per hectare for given pairs of stem count per hectare and diameter (dm)
 * @param stems stem count per hectare
 * @param ds diameters (dm)
 * @return a vector of basal areas (m^2) per hectare
 */
std::vector<float> basal_areas(const std::vector<float>& stems, const std::vector<float>& ds) {
    if(stems.size() != ds.size()) {
        throw std::domain_error("Attempting to compute basal area for nonequal length source data.");
    }
    std::vector<float> result(stems.size());
    for(int i = 0; i < stems.size(); i++) {
        result[i] = circle_area(ds[i], -2) * stems[i];
    }
    return result;
}
