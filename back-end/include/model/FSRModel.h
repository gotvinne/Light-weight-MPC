/**
 * @file FSRModel.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#ifndef FSR_MODEL_H
#define FSR_MODEL_H

#include <vector>
#include <map>
#include <string>

#include <Eigen/Eigen>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;
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
    MatrixXd du_tilde_mat_; /** Post change in actuation matrix (n_MV, (N-W-1)) */

    VectorXd** pp_SR_vec_; /** Matrix of Eigen::VectorXd holding every n_CV * n_MV step response */
    MatrixXd** pp_SR_mat_; /** Tensor of Eigen::MatrixXd representing the SISO prediction (P-W,M) times (n_CV, n_MV) */
    MatrixXd theta_; /** Matrix of all SISO predictions (n_CV*(P-W), n_MV*M) */
    MatrixXd phi_; /** Past step coefficients (n_CV*(P-W+1), n_MV*(N-W-1)) */
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
     * @brief Pad the vector, by appending Sn (last element of vec) pad times
     * 
     * @param vec Eigen::VectorXd to be padded
     * @param pad Number of pads
     * @param sn Last S-coefficient
     * @return Eigen::VectorXd Padded vector 
     */
    VectorXd PadVec(VectorXd& vec, int pad, double sn);

    /**
     * @brief Set the Psi object
     * 
     */
    void setPsi();

    
    VectorXd getDuTilde();

public: 

    SparseXd getOmegaY();
    
    FSRModel() : n_CV_{0}, n_MV_{0} {}
    /**
     * @brief The constructor. Constructing the object allocating memory for the SISO prediction matric
     */
    FSRModel(VectorXd** SR, std::map<std::string, int> m_param, int P, int M, int W,
            const std::vector<double>& init_u, const std::vector<double>& init_y);

    /**
     * @brief Construct a new FSRModel object, Open loop constructor
     * 
     * @param SR 
     * @param m_param 
     * @param init_u 
     * @param init_y 
     */
    FSRModel(VectorXd** SR, std::map<std::string, int> m_param,
            const std::vector<double>& init_u, const std::vector<double>& init_y);
    /**
     * @brief The destructor. Freeing the memory allocated in the constructor
     */
    ~FSRModel();

    void setDuTildeMat(const MatrixXd& mat) { du_tilde_mat_ = mat; }

    /** Get functions */
    int getP() const { return P_; }
    int getM() const { return M_; }
    int getW() const { return W_; }
    int getN_CV() const { return n_CV_; }
    int getN_MV() const { return n_MV_; }

    MatrixXd getTheta() const { return theta_; }
    MatrixXd getDuTildeMat() const { return du_tilde_mat_; }
    VectorXd getUK() const { return u_K_; } // Returns most recent U
    VectorXd getU() const { return u_; }

    /** MPC functionality*/
    /**
     * @brief Projecting the FSRModel, by updating the former step responses and actuation
     * 
     * @param du Optimized actuation for next projection, du = omega_u * du
     */
    void UpdateU(const VectorXd& du);

    /**
     * @brief Return model output, Y(k+1) = Omega * (Theta * Delta U + Phi * Delta U_tilde + Psi * U)
     *                                    = Omega * (Theta * Delta U + Lambda)
     * Y is a vector containing every P * n_CV predictions further in time
     * 
     * @param du [Eigen::VectorXd]
     * @return VectorXd predicted output, one step, k+1 ahead. 
     */
    VectorXd getY(const VectorXd& du) { return getOmegaY() * (theta_ * du + getLambda()); } // Must be changed for soft constraint!

    /**
     * @brief Get the Lambda object, Lambda = Phi * Delta U_tilde + Psi * U
     * 
     * @return VectorXd 
     */
    VectorXd getLambda() { return phi_ * getDuTilde() + psi_ * u_; }

    /** Print functions */
    void PrintTheta() const;
    void PrintPhi() const;
    void PrintPsi() const;
    void PrintActuation() const;
};

#endif // FSR_MODEL_H
