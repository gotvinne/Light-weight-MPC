/**
 * @file FSRCoefficients.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FSR_COEFFICIENTS_H
#define FSR_COEFFICIENTS_H

#include <Eigen/Dense>
#include <vector>

class FSRCoefficients {
private:
    int n_CV_;
    int n_MV_;
    int N_; 
    int P_;
    int M_;
    int W_;

    Eigen::MatrixXf SR_; // Can consider making this array of Eigen::VectorXf, simplifying access
    std::vector<std::vector<Eigen::MatrixXf>> SR_matrix_; // (P,M) times (n_CV, n_MV) , can consider allocating memory for array instead of vector
    Eigen::MatrixXf theta_; // (n_CV *(P-W), n_MV*M)
    Eigen::MatrixXf phi_; // (P, N-P)
public: 
    FSRCoefficients(const Eigen::MatrixXf& SR, int n_CV, int n_MV, int N, int P, int M, int W);
    void GenerateLowerTriangularMatrix(const Eigen::VectorXf& vec, Eigen::MatrixXf& S);
    void SelectPredictionVec(Eigen::MatrixXf& S);
    void setSRMatrix();

    void PrintSRMatrix(int i, int j);
};

#endif // STEP_RESPONSE_COEFFICIENTS_H
