#include <cilantro/point_cloud.hpp>
#include <cilantro/ransac_hyperplane_estimator.hpp>
#include <cilantro/visualizer.hpp>
#include <cilantro/common_renderables.hpp>

void callback(bool &re_estimate) {
    re_estimate = true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Please provide path to PLY file" << std::endl;
        return 0;
    }

    cilantro::PointCloud3f cloud(argv[1]);

    cilantro::Visualizer viz("HyperplaneRANSACEstimator example", "disp");
    bool re_estimate = false;
    viz.registerKeyboardCallback('a', std::bind(callback, std::ref(re_estimate)));

    std::cout << "Press 'a' for a new estimate" << std::endl;

    viz.addObject<cilantro::PointCloudRenderable>("cloud", cloud);
    while (!viz.wasStopped()) {
        if (re_estimate) {
            re_estimate = false;

            cilantro::PlaneRANSACEstimator3f pe(cloud.points);
            pe.setMaxInlierResidual(0.01f).setTargetInlierCount((size_t)(0.15*cloud.size())).setMaxNumberOfIterations(250).setReEstimationStep(true);
            cilantro::HomogeneousVector<float,3> plane = pe.getModelParameters();
            std::vector<size_t> inliers = pe.estimateModelParameters().getModelInliers();
            std::cout << "RANSAC iterations: " << pe.getPerformedIterationsCount() << ", inlier count: " << pe.getNumberOfInliers() << std::endl;

            cilantro::PointCloud3f planar_cloud(cloud, inliers);
            viz.addObject<cilantro::PointCloudRenderable>("plane", planar_cloud.points, cilantro::RenderingProperties().setPointColor(1,0,0).setPointSize(3.0));

            std::cout << "Press 'a' for a new estimate" << std::endl;
        }
        viz.spinOnce();
    }

    return 0;
}
