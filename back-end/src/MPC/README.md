
# Module implementing and solving QP problem

Implemented using [osqp-eigen](https://github.com/robotology/osqp-eigen) C++ wrapper for the [OSQP](https://osqp.org/) software.

The OSQP, operator splitting QP solver solves the problems of the following form:

$$ min \quad \frac{1}{2} z^T \boldsymbol{H} z+q^T z \\ 
\text{subject to} \quad l \leq \boldsymbol{A} z \leq u $$ 

## Step Response MPC solver

This solver solves the QP defining the MPC problem for a finite step response model (FSRM) in terms of only one optimization variable (condensed form). This form is obtained by using the null space method on the optimilization problem formulated by quadratic constraining the output $Y$ and input $\Delta U$: 

$$ \min \sum_{j=W}^{P}\left\|(y(k+j \mid k)-r_y(k+j))\right\|_{\bar{Q}}^2+ \sum_{j=0}^{(M-1)} \left\|\Delta u(k+j)\right\|_{\bar{R}}^2+\bar{\rho} \bar{\epsilon}+\underline{\rho} \underline{\epsilon} $$ 

$$ min \quad Y(k+w)^TQY(k+w) + \Delta U(k+i)^TR\Delta U(k+i) $$

*Condensed form:*

$$ \min \quad \frac{1}{2}z_{cd}^T(2\Theta^T\bar{Q}\Theta + 2\bar{R})z_{cd} + 2\Theta^TQ(\Lambda(k)-\tau(k))z_{cd} \\
   \quad \frac{1}{2} z_{cd}^T \boldsymbol{H} z_{cd}+q^T z_{cd} $$

where 

$$ \Lambda(k) = \boldsymbol{\Phi} \Delta \tilde{U}(k)+\Psi \tilde{U}(k-N)+B(k)$$ 

$$ \underline{z}_{st} - \begin{bmatrix} 0 \\ \boldsymbol{K}^{-1} \boldsymbol{\Gamma} \bar{U}(k-N) \\ \Lambda(k) \end{bmatrix} \leq 
 \begin{bmatrix} \boldsymbol{I} \\ \boldsymbol{K}^{-1}\\ \boldsymbol{\Theta} \end{bmatrix} z_{cd} \leq \bar{z}_{st} - \begin{bmatrix} 0 \\ \boldsymbol{K}^{-1} \boldsymbol{\Gamma} \bar{U}(k-N) \\ \Lambda(k) \end{bmatrix} \\
 l \leq \boldsymbol{A} z \leq u$$
 


**Some matrix definitions:**

$$ \boldsymbol{K} =\operatorname{blkdiag}\left(\boldsymbol{K_1}, \boldsymbol{K_2}, \ldots, \boldsymbol{K_{n_{M V}}}\right), \\
        \boldsymbol{\Gamma} =\operatorname{blkdiag}\left(\Gamma_1, \Gamma_2, \ldots, \Gamma_{n_{M V}}\right) $$

$$ K_j=\left[\begin{array}{ccccc}
    1 & 0 & \ldots & 0 & 0 \\
    -1 & 1 & \ddots & 0 & 0 \\
    0 & -1 & \ddots & 0 & 0 \\
    \vdots & \vdots & \ddots & \ddots & \vdots \\
    0 & 0 & \ldots & -1 & 1
    \end{array}\right], \quad \Gamma_j=\left[\begin{array}{c}
    1 \\
    0 \\
    0 \\
    \vdots \\
    0
    \end{array}\right] $$
  
**Overview of the dimensions used in the controller:**

<div style="margin-left: 20%;
            margin-right: auto;
            width: 100%">

| Matrix expression | Condensed dimensions |
| :-: | :-: |
| $Y$ | $P \times n_{CV}$  |
| $\Delta U$ | $M \times n_{MV}$  |
| $z_{cd}$  | $M \cdot n_{MV} + dim(\epsilon)$  |
| $\boldsymbol{\bar{Q}}$ | $n_{CV} \cdot (P - W + 1) \times n_{CV} \cdot (P - W + 1)$  |
| $\boldsymbol{\bar{R}}$ | $M \cdot n_{MV}  \times M \cdot n_{MV}$ |
| $\boldsymbol{\Theta}$  | $n_{CV} \cdot (P-W) \times M \cdot n_{MV}$ |
| $\boldsymbol{\Phi}$ | $n_{C V}\left(P-W+1\right) \times \sum_{j=1}^{n_{M V}}\left(N-W-1\right)$ |
| $\boldsymbol{\Psi}$ | $n_{C V}\left(P-W+1\right) \times n_{M V}$ |
| $\boldsymbol{K}$ | $M \cdot n_{MV} \times M \cdot n_{MV}$ |
| $\boldsymbol{A}$ | $2M \cdot n_{MV} + P \cdot n_{CV } \times M \cdot n_{MV}$|
| $\boldsymbol{\Gamma}$ | $M \cdot n_{MV} \times n_{MV}$  |


</div>
