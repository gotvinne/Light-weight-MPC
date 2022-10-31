/**
 * @file FSRCoefficients.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "FSRCoefficients.h"
#include <vector>

#include <Eigen\Dense>

FSRCoefficients::FSRCoefficients(const Eigen::MatrixXf& SR, const MPCConfig& conf) : SR_{SR}, conf_{conf} {

    for (int row = 0; row < conf.n_CV; row++) {
        std::vector<
        for (int col = 0; col < conf.n_MV) {

        }
    }

}

void StepResponseCoefficients::setLowerTriangularSMatrix(const Eigen::VectorXf& vec) {
    int P = mpc_config.P;
    int M = mpc_config.M;
    S = Eigen::MatrixXf::Zero(P, M);
    int n = M;
    for (int i = 0; i < P; i++) {
        if (i >= (M-1)) {
            n--; 
        }
        for (int j = 0; j < n; j++) {
            S(i+j, j) = vec(i);
        }
    }
}