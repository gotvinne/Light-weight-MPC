/**
 * @file FSRModel.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FSR_MODEL_H
#define FSR_MODEL_H

#include <Eigen/Dense>

//! A Finite Step Response model object
/*!
 C++ class object holding the FSR model given a spesific format of the step response coefficients.
 A MPC configuration is also passed as input in order shape the system matrices for the MPC algorithm. 
 */
class FSRModel {
private:
    int n_CV_;
    int n_MV_;
    int N_; 
    int P_;
    int M_;
    int W_;

    Eigen::VectorXf** pp_SR_vec_;
    Eigen::MatrixXf** pp_SR_mat_; // (P-W,M) times (n_CV, n_MV)
    Eigen::MatrixXf theta_; // (n_CV*(P-W), n_MV*M)
    Eigen::MatrixXf phi_; // (n_CV*P-W, n_MV*N-(P-W))
public: 
    //! The constructor
    /*! Constructing the object allocating memory for the SISO prediction matric */
    FSRModel(Eigen::VectorXf** SR, int n_CV, int n_MV, int N, int P, int M, int W);
    //! The destructor 
    /*! Freeing the memory allocated in the constructor */
    ~FSRModel();

    // SISO Step response matrix
    void setSRMatrix();
    void GenerateLowerTriangularMatrix(const Eigen::VectorXf& vec, Eigen::MatrixXf& S);
    
    // Prediction Theta matrix
    void setThetaMatrix();

    // Previous Phi matrix
    // void setPhiMatrix();
    // void SelectPastVec();
    // void FillPhi(const Eigen::VectorXf& vec, const int& row);

    // Get functions
    int getN() const { return N_; }
    int getP() const { return P_; }
    int getM() const { return M_; }
    int getW() const { return W_; }
    int getN_CV() const { return n_CV_; }
    int getN_MV() const { return n_MV_; }

    // Print functionality
    void PrintPPSR(int i, int j);
    void PrintTheta();
    //void PrintSR(); Need new implementation
    void PrintPhi();
    
};

#endif // FSR_MODEL_H
