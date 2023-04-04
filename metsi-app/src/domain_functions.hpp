#ifndef METSI_DOMAIN_FUNCTIONS_HPP
#define METSI_DOMAIN_FUNCTIONS_HPP
#include <vector>

float circle_area(float, float = 0.0f);
float median(std::vector<float>);
std::vector<float> basal_areas(const std::vector<float>& stems, const std::vector<float>& ds);

#endif //METSI_DOMAIN_FUNCTIONS_HPP