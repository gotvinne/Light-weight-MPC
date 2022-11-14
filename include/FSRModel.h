/**
 * @file FSRModel.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FSR_MODEL_H
#define FSR_MODEL_H

#include <vector>

#include <Eigen/Dense>
using MatrixXf = Eigen::MatrixXf;
using VectorXf = Eigen::VectorXf; 
/**
 * @brief A Finite Step Response model object. C++ class object holding the FSR model given a spesific format of the step response coefficients.
 * A MPC configuration is also passed as input in order shape the system matrices for the MPC algorithm. 
 */ 
class FSRModel {
private:
    int n_CV_; /** Number of controlled variables */
    int n_MV_; /** Number of manipulated variables */
    int N_; /** Number of step response coefficients */
    int P_; /** Prediction horizon */
    int M_; /** Control horizon */
    int W_; /** Time delay coefficient */

    VectorXf u_N_; /** Manipulated variables n_MV */
    VectorXf y_; /** Controlled variables n_CV */
    VectorXf u_k_; /** Denotes U_tilde(k-1) n_MV */
    MatrixXf du_tilde_; /** Change in actuation (n_MV*(N-W-1), N-1) */

    VectorXf** pp_SR_vec_; /** Matrix of Eigen::VectorXf holding every n_CV * n_MV step response */
    MatrixXf** pp_SR_mat_; /** Tensor of Eigen::MatrixXf representing the SISO prediction (P-W,M) times (n_CV, n_MV) */
    MatrixXf theta_; /** Matrix of all SISO predictions (n_CV*(P-W), n_MV*M) */
    MatrixXf phi_; /** Past step coefficients (n_CV*P-W, n_MV*(N-W-1)) */
    MatrixXf azymuth_; /** Last step coefficient matrix, (n_CV (P-W), n_MV)*/
public: 
    /**
     * @brief The constructor. Constructing the object allocating memory for the SISO prediction matric
     */
    FSRModel(VectorXf** SR, int n_CV, int n_MV, int N, int P, int M, int W, 
            const std::vector<float>& init_u, const std::vector<float>& init_y);
    /**
     * @brief The destructor. Freeing the memory allocated in the constructor
     */
    ~FSRModel();

    void AllocateAndDeepCopy(VectorXf** SR);

    /**
     * @brief Set pp_SR_mat, by generating SISO predictions from pp_SR_vec
     */
    void setSRMatrix();
    /**
     * @brief Set lower triangular matrix based on the prediction vector
     * 
     * @param pred_vec prediction vector 
     * @param S Eigen::MatrixXf to be filled with lower triangular SISO predictions
     */
    void setLowerTriangularMatrix(const VectorXf& pred_vec, MatrixXf& S);
    
    /**
     * @brief Set the Theta Matrix object. By filling the matrix with SISO preductions from pp_SR_mat
     */
    void setThetaMatrix();

    /**
     * @brief Set the Phi Matrix object holding the previous step coefficients
     */
    void setPhiMatrix();

    /**
     * @brief Filling one row of phi
     * 
     * @param vec Vector to be filled n_MV times into phi
     * @param row row indicator
     */
    void FillRowPhi(const VectorXf& vec, const int& row);

    /**
     * @brief Pad the vector, by appending Sn (last element of vec) pad times
     * 
     * @param vec Eigen::VectorXf to be padded
     * @param pad Number of pads
     * @return Eigen::VectorXf Padded vector 
     */
    VectorXf PadVec(VectorXf& vec, int pad);

    void setAzymuth();

    void UpdateUN(const VectorXf& du);
    void UpdateUK();

    /** Get functions */
    int getP() const { return P_; }
    int getM() const { return M_; }
    int getW() const { return W_; }
    int getN_CV() const { return n_CV_; }
    int getN_MV() const { return n_MV_; }
    int getN() const { return N_; }
    MatrixXf getTheta() const { return theta_; }
    MatrixXf getPhi() const { return phi_; }
    VectorXf getUN() const { return u_N_; }

    /** Print functions */
    void PrintPPSR(int i, int j);
    void PrintTheta();
    void PrintPhi();
    void PrintAzymuth();
};
#endif // FSR_MODEL_H
