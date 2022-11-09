
# Module implementing and solving QP problem

Implemented using [osqp-eigen](https://github.com/robotology/osqp-eigen) C++ wrapper for the [OSQP](https://osqp.org/) software.

The OSQP, operator splitting QP solver solves the problems of the following form:

$$ min \quad \frac{1}{2} z^T H z+q^T z $$
subject to $$ l \leq A z \leq u $$ 

## Step Response solver, sr_solver

This solver solves the QP defining the MPC problem for a finite step response model (FSRM) in terms of only one optimization variable (condensed form). This form is obtained by using the null space method on the optimilization problem formulated by quadratic constraining the output $Y$ and input $\Delta U$: 

$$ \min \sum_{j=W}^{P}\left\|y(k+j \mid k)-r_y(k+j)\right\|_{\bar{Q}}^2+\sum_{j=0}^{M-1}\|\Delta u(k+j)\|_{\bar{R}}^2+\bar{\rho} \bar{\epsilon}+\underline{\rho} \underline{\epsilon} $$ 

$$ min \quad Y(k+w)^TQY(k+w) + \Delta U(k+i)^TR\Delta U(k+i) $$

*Condensed form:*

$$ min \quad \frac{1}{2}z_{cd}^T(2\Theta^T\bar{Q}\Theta + 2\bar{R})z_{cd} + 2\Theta^TQ(\Lambda(k)-\tau(k))z_{cd} $$
where 

$$ \Lambda(k) =  $$ 


## State-space solver, ss_solver

This solver solves the QP defining the MPC problem for a state-space model in reduced from. 

