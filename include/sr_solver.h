/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef SR_SOLVER_H
#define SR_SOLVER_H

/**
 * @brief 
 * 
 */
void sr_solver(const int& T, const std::map<std::string,int> model_param);

void setInequalityConstraints();
void setWeightMatrices();

#endif // SR_SOLVER_H
