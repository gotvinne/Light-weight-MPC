/**
 * @file FSRCoefficients.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "FSRCoefficients.h"
#include <vector>
#include <iostream>

#include <Eigen/Dense>

FSRCoefficients::FSRCoefficients(const Eigen::MatrixXf& SR, int n_CV, int n_MV, int N, int P, int M, int W) : SR_{SR}, 
                                 n_CV_{n_CV}, n_MV_{n_MV}, N_{N}, P_{P}, M_{M}, W_{W} {
    // Initializing theta, phi and SR_matrix
    theta_ = Eigen::MatrixXf::Zero(n_CV*(P-W), n_MV*M);
    phi_ = Eigen::MatrixXf::Zero(P, N-P);
    for (int row = 0; row < n_CV; row++) {
        std::vector<Eigen::MatrixXf> col_vec; 
        for (int col = 0; col < n_MV; col++) {
            col_vec.push_back(Eigen::MatrixXf::Zero(P, M));
        }
        SR_matrix_.push_back(col_vec);
    }
    setSRMatrix();
}

void FSRCoefficients::GenerateLowerTriangularMatrix(const Eigen::VectorXf& vec, Eigen::MatrixXf& S) {
    int n = M_;
    int t = 0;
    for (int i = 0; i < P_; i++) {
        if (i >= (M_-1)) {
            n--; 
        }
        if (n > 0) {
            for (int j = 0; j < n; j++) {
                S(i+j, j) = vec(i);
            }
        } else {
            for (int k = 0; k < M_; k++) {
                S(i, (M_-1)-k) = vec((M_-1)+t+k);
            }
            t++;
        }
    }
}

void FSRCoefficients::SelectPredictionVec(Eigen::MatrixXf& S) {
    int interval = 0; 
    for (int i = 0; i < n_CV_; i++) {
        Eigen::VectorXf vec = Eigen::VectorXf::Zero(P_);
        for (int j = 0; j < n_MV_*N_; j = j + N_) {
            for (int k = 0; k < P_; k++) {
                vec(k) = SR_(i, j+k);
            } 
            GenerateLowerTriangularMatrix(vec, S);
        }
    }
}

void FSRCoefficients::setSRMatrix() {
    for (int i = 0; i < n_MV_; i++) {
        Eigen::MatrixXf S = Eigen::MatrixXf::Zero(P_, M_);
        for (int j = 0; j < n_CV_; j++) {
            SelectPredictionVec(S);
            SR_matrix_[i][j] = S;
        }
    }
}

void FSRCoefficients::PrintSRMatrix(int i, int j) {
    std::cout << SR_matrix_[i][j] << std::endl;
}