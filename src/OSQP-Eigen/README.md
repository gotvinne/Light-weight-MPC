
# Module implementing and solving QP problem

This is done by using OSQP, implemented using OSQP-Eigen C++ wrapper.

[OSQP Example](https://osqp.org/docs/examples/setup-and-solve.html)
[OSQP-Eigen MPC_example](https://github.com/robotology/osqp-eigen/blob/master/example/src/MPCExample.cpp)


The OSQP, operator splitting QP solver solves the problems of the following form:

$$ min \quad \frac{1}{2} z^T H z+q^T z $$
subject to $$ l \leq A z \leq u $$ 

## Step Response solver, sr_solver

This solver solves the QP defining the MPC problem for a finite step response model (FSRM) in terms of only one optimization variable (condensed form). This form is obtained by using the null space method on the optimilization problem formulated by quadratic constraining the output $Y$ and input $\Delta U$: 

$$ \min \sum_{j=W}^{P}\left\|y(k+j \mid k)-r_y(k+j)\right\|_{\bar{Q}}^2+\sum_{j=0}^{M-1}\|\Delta u(k+j)\|_{\bar{R}}^2+\bar{\rho} \bar{\epsilon}+\underline{\rho} \underline{\epsilon} $$
$$ min \quad Y(k+w)^TQY(k+w) + \Delta U(k+i)^TR\Delta U(k+i) $$

*Condensed form:*

$$ min \quad \frac{1}{2}z_*^T(2\Theta^T\bar{Q}\Theta + 2\bar{R})z_* + 2\Theta^TQ(\Lambda(k)-\tau(k))z_*$$
where $$ \Lambda(k) =  $$ 


## State-space solver, ss_solver

This solver solves the QP defining the MPC problem for a state-space model in reduced from. 

