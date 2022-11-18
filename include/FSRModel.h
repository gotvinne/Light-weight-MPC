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
using MatrixXd = Eigen::MatrixXd;
using VectorXd = Eigen::VectorXd; 
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

    VectorXd u_; /** Manipulated variables, U(k-N), n_MV */
    VectorXd y_; /** Controlled variables n_CV */

    VectorXd u_K_; /** Denotes U(k-1), n_MV */
    VectorXd du_tilde_; /** Post change in actuation (n_MV*(N-W-1)) */

    VectorXd** pp_SR_vec_; /** Matrix of Eigen::VectorXd holding every n_CV * n_MV step response */
    MatrixXd** pp_SR_mat_; /** Tensor of Eigen::MatrixXd representing the SISO prediction (P-W,M) times (n_CV, n_MV) */
    MatrixXd theta_; /** Matrix of all SISO predictions (n_CV*(P-W), n_MV*M) */
    MatrixXd phi_; /** Past step coefficients (n_CV*P-W, n_MV*(N-W-1)) */
    MatrixXd psi_; /** Last step coefficient matrix, (n_CV (P-W), n_MV)*/

    /**
     * @brief Helper function allocating and performing deep copy of Eigen::VectorXd** 
     * 
     * @param SR Dobble pointer Eigen::VectorXd**
     */
    void AllocateAndDeepCopy(VectorXd** SR);

    /**
     * @brief Set pp_SR_mat, by generating SISO predictions from pp_SR_vec
     */
    void setSRMatrix();
    
    /**
     * @brief Set lower triangular matrix based on the prediction vector
     * 
     * @param pred_vec prediction vector 
     * @param S Eigen::MatrixXd to be filled with lower triangular SISO predictions
     */
    void setLowerTriangularMatrix(const VectorXd& pred_vec, MatrixXd& S);
    
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
    void FillRowPhi(const VectorXd& vec, const int& row);

    /**
     * @brief Pad the vector, by appending Sn (last element of vec) pad times
     * 
     * @param vec Eigen::VectorXd to be padded
     * @param pad Number of pads
     * @return Eigen::VectorXd Padded vector 
     */
    VectorXd PadVec(VectorXd& vec, int pad);

    /**
     * @brief Set the Psi object
     * 
     */
    void setPsi();

public: 
    /**
     * @brief The constructor. Constructing the object allocating memory for the SISO prediction matric
     */
    FSRModel(VectorXd** SR, int n_CV, int n_MV, int N, int P, int M, int W, 
            const std::vector<double>& init_u, const std::vector<double>& init_y);
    /**
     * @brief The destructor. Freeing the memory allocated in the constructor
     */
    ~FSRModel();

    /**
     * @brief Projecting the FSRModel, by updating the former step responses and actuation
     * 
     * @param du Optimized actuation for next projection
     * @param du_gamma Optimized actuation scaled to fit phi
     */
    void UpdateU(const VectorXd& du, const MatrixXd& du_gamma);

    /** Get functions */
    int getP() const { return P_; }
    int getM() const { return M_; }
    int getW() const { return W_; }
    int getN_CV() const { return n_CV_; }
    int getN_MV() const { return n_MV_; }
    int getN() const { return N_; }

    MatrixXd getTheta() const { return theta_; }
    MatrixXd getPhi() const { return phi_; }
    VectorXd getUK() const { return u_K_; }
    VectorXd getLambda() const { return phi_ * du_tilde_ + psi_ * u_; }

    /** Print functions */
    void PrintPPSR(int i, int j);
    void PrintTheta();
    void PrintPhi();
    void PrintPsi();
};

#endif // FSR_MODEL_H
