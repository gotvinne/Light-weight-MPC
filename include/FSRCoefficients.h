/**
 * @file FSRCoefficients.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FSR_COEFFICIENTS_H
#define FSR_COEFFICIENTS_H

#include <Eigen\Dense>
#include "parse.h"

#include <vector>

class FSRCoefficients {
private:
    Eigen::MatrixXf SR_;
    MPCConfig conf_;
    std::vector<std::vector<Eigen::MatrixXf>> SR_matrix_;
    Eigen::MatrixXf theta; 
    Eigen::MatrixXf phi; 
public: 
    FSRCoefficients(const Eigen::MatrixXf& SR, const MPCConfig& conf);
    void setLowerTriangularSMatrix(const Eigen::VectorXf& vec);
};



#endif // STEP_RESPONSE_COEFFICIENTS_H