#pragma once

#include <vector>
#include <Eigen/Dense>

enum ColormapType {
    COLORMAP_NONE,
    COLORMAP_JET,
    COLORMAP_GREY,
    COLORMAP_BLUE2RED
};

std::vector<Eigen::Vector3f> colormap(const std::vector<float> &scalars, float scalar_min, float scalar_max, ColormapType colormapType);
