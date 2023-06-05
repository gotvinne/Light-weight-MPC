
# Module implementing and solving QP problem

Implemented using [osqp-eigen](https://github.com/robotology/osqp-eigen) C++ wrapper for the [OSQP](https://osqp.org/) software.

The OSQP, operator splitting QP solver solves the problems of the following form:

$$ min \quad \frac{1}{2} z^T \boldsymbol{H} z+q^T z \\ 
\text{subject to} \quad l \leq \boldsymbol{A} z \leq u $$ 

The solver software can be customized: 
- [OSQP solver settings](https://osqp.org/docs/interfaces/solver_settings.html#solver-settings)
- [Status values and errors](https://osqp.org/docs/interfaces/status_values.html)

## Step Response MPC solver

This solver solves the QP defining the MPC problem for a finite step response model (FSRM) in terms of only one optimization variable (condensed form). This form is obtained by using the null space method on the optimilization problem formulated by quadratic constraining the output $Y$ and input $\Delta U$. The number of parameters considered in the cost function is dependant on the horizons $P \succ 0$, $M \succ 0$ and $W \geq 0$.

$$ \min \sum_{j=W+1}^{P}\left\|(y(k+j \mid k)-r_y(k+j))\right\|_{\bar{Q}}^2+ \sum_{j=0}^{(M-1)} \left\|\Delta u(k+j)\right\|_{\bar{R}}^2+\bar{\rho} \bar{\epsilon}+\underline{\rho} \underline{\epsilon} $$ 

$$ min \quad Y(k+(P-W))^TQY(k+(P-W)) + \Delta U(k+(M-1))^TR\Delta U(k+(M-1)) $$

*Condensed form:*

$$ \min_{z_{cd}} \quad \frac{1}{2} z_{cd}^T \boldsymbol{G_{cd}} z_{cd}+q^T_{cd} z_{cd} $$

where 

$$ \boldsymbol{G_{cd}} = 2 \cdot blkdiag( \boldsymbol{\Theta}^{T} \boldsymbol{\bar{Q}} \boldsymbol{\Theta} + \boldsymbol{\bar{R}}, \boldsymbol{0}, \boldsymbol{0}), $$

$$ q_{cd}(k) = \begin{bmatrix} 2 \cdot \boldsymbol{\Theta}^T\boldsymbol{\bar{Q}}(\Lambda(k) - \mathcal{T}(k)) \\ \rho_{h} \\ \rho_{l} \end{bmatrix}, $$ 

s.t.

$$ \underline{z}_{cd}(k) = \begin{bmatrix}
        \Delta \underline{U} \\ \underline{U} \\ -\infty \\ \underline{Y} \\ 0 \\ 0
    \end{bmatrix} - \begin{bmatrix} 0 \\ \mathbf{K}^{-1} \mathbf{\Gamma} U(k-1) \\ \mathbf{\Lambda}(k) \\ \mathbf{\Lambda}(k)\\ 0 \\ 0 \end{bmatrix} \leq 
    \begin{bmatrix}
    \mathbf{I} & \mathbf{0} & \mathbf{0} \\
    \mathbf{K^{-1}} & \mathbf{0} & \mathbf{0} \\
    \mathbf{\Theta} & \mathbf{-I} & \mathbf{0} \\
    \mathbf{\Theta} & \mathbf{0} & \mathbf{I} \\
    \mathbf{0} & \mathbf{I} & \mathbf{0} \\
    \mathbf{0} & \mathbf{0} & \mathbf{I} \\
    \end{bmatrix} z_{cd} \leq \begin{bmatrix}
        \Delta \bar{U} \\ \bar{U} \\ \bar{Y} \\ \infty \\ \infty \\ \infty
    \end{bmatrix} - \begin{bmatrix} 0 \\ \mathbf{K}^{-1} \mathbf{\Gamma} U(k-1) \\ \mathbf{\Lambda}(k) \\ \mathbf{\Lambda}(k) \\ 0 \\ 0 \end{bmatrix} = \bar{z}_{cd}(k) $$

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
| $z_{cd}$  | $M \cdot n_{MV} + 2 \cdot n_{CV} $  |
| $\boldsymbol{\bar{Q}}$ | $n_{CV} \cdot (P - W) \times n_{CV} \cdot (P - W)$  |
| $\boldsymbol{\bar{R}}$ | $M \cdot n_{MV}  \times M \cdot n_{MV}$ |
| $\boldsymbol{\Theta}$  | $n_{CV} \cdot (P-W) \times M \cdot n_{MV}$ |
| $\boldsymbol{\Phi}$ | $n_{C V}\left(P-W\right) \times \sum_{j=1}^{n_{M V}}\left(N-W-1\right)$ |
| $\boldsymbol{\Psi}$ | $n_{C V}\left(P-W\right) \times n_{M V}$ |
| $\boldsymbol{K}$ | $M \cdot n_{MV} \times M \cdot n_{MV}$ |
| $\boldsymbol{\Gamma}$ | $M \cdot n_{MV} \times n_{MV}$  |
| $\boldsymbol{A}$ | $n_{MV} + (P - W) \cdot n_{CV} + 2 n_{CV} \times M \cdot n_{MV} + 2 n_{CV}$ |
| $\boldsymbol{G_{cd}}$ | $M \cdot n_{MV} + 2 n_{CV} \times M \cdot n_{MV} + 2 n_{CV}$ |
| $q_{cd}$ | $M \cdot n_{MV} + 2 \cdot n_{CV}$ |

</div>
