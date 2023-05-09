## MODELS

### The Step Response Mode: FSRModel
The FSRModel is a C++ object creating the dynamics of a general linear step response model. The following equations and matrices are avaliable by calling the FSRModel functionality.

**Output prediction equation**

$$ Y(k+P) = \boldsymbol{\Theta} \Delta U(k+M)+ \boldsymbol{\Phi} \Delta \tilde{U}(k)+\Psi \tilde{U}(k-N)+B(k) $$

**Theta-matrix definition:**

$$ \boldsymbol{S} \triangleq\left[\begin{array}{cccc}
s_1 & 0 & \cdots & 0 \\
s_2 & s_1 & 0 & \vdots \\
\vdots & \vdots & \ddots & 0 \\
s_M & s_{M-1} & \cdots & s_1 \\
s_{M+1} & s_M & \cdots & s_2 \\
\vdots & \vdots & \ddots & \vdots \\
s_P & s_{P-1} & \cdots & s_{P-M+1}
\end{array}\right] $$

$$ \boldsymbol{\Theta} \triangleq\left[\begin{array}{cccc}
\boldsymbol{S}_{11} & \boldsymbol{S}_{12} & \cdots & \boldsymbol{S}_{1 n_{MV}} \\
\boldsymbol{S}_{21} & \cdots & \cdots & \boldsymbol{S}_{2 n_{MV}} \\
\vdots & \vdots & \vdots & \vdots \\
\boldsymbol{S}_{n_{CV} 1} & \cdots & \cdots & \boldsymbol{S}_{n_{CV} n_{MV}} \end{array}\right]_{n_{CV} \cdot (P-W) \times M \cdot n_{MV}} $$

**Phi-matrix definition:**
$$ 
\boldsymbol{\Phi}=\left[\begin{array}{cccc}
\boldsymbol{\Phi_{1,1}} & \boldsymbol{\Phi_{1,2}} & \cdots & \boldsymbol{\Phi_{1, n_{M V}}} \\
\boldsymbol{\Phi_{2,1}} & \boldsymbol{\Phi_{2,2}} & \cdots & \boldsymbol{\Phi_{2, n_{M V}}} \\
\vdots & \vdots & \ddots & \vdots \\
\boldsymbol{\Phi_{n_{C V}, 1}} & \boldsymbol{\Phi_{n_{C V}, 2}} & \cdots & \boldsymbol{\Phi_{n_{C V}, n_{M V}}}
\end{array}\right]_{n_{C V}\left(P-W+1\right) \times \sum_{j=1}^{n_{M V}}\left(N-W-1\right)} $$

$$
\boldsymbol{\Phi_{i, j}} =\left[\begin{array}{ccccc}
s_{W+1} & s_{W+2} & \ldots & s_{N-2} & s_{N-1} \\
s_{W+2} & s_{W+3} & \ldots & s_{N-1} & s_{N}\\
\vdots & \vdots & \vdots & \vdots & \vdots \\
s_{P+1} & s_{P+2} & \ldots & s_{N} & s_{N}
\end{array}\right]_{\left(P-W+1\right) \times N-W-1}
 $$

**Psi-matrix definition:**

$$ \boldsymbol{\Psi} =\left[\begin{array}{cccc}
\Psi_{1,1} & \Psi_{1,2} & \cdots & \Psi_{1, n_{M V}} \\
\Psi_{2,1} & \Psi_{2,2} & \cdots & \Psi_{2, n_{M V}} \\
\vdots & \vdots & \ddots & \vdots \\
\Psi_{n_{C V}, 1} & \Psi_{n_{C V}, 2} & \cdots & \Psi_{n_{C V}, n_{M V}}
\end{array}\right]_{n_{C V}\left(P-W+1\right) \times n_{M V}} $$

$$
\Psi_{i, j} =\left[\begin{array}{c}
s_{N} \\
s_{N} \\
\vdots \\
s_{N}
\end{array}\right]_{\left(P-W+1\right) \times 1}
$$

#### Retrieve Step response coefficients

This is a module for generating customized step-response coefficients from a first order time delayed model. 

$$ \frac{Y(s)}{U(s)}=\frac{K e^{-\theta s}}{\tau s+1}, $$

where $K$, $\theta$ and $\tau$ are respectively the gain, the time delay and the time constant of the system. $U(s)$ is modeled as a step input, 

$$ U(s) = \frac{\Delta u}{s}, $$ 

where $\Delta u$ is the model input.  

### Plant model:

In order to simulate the MPC scenario realistically, a reference system generating a measured output is desired. This model represents the output of an avaliable sensor for the SISO case. The measured value can be used to realize output feedback by obtaining a bias correction term. 

By reformulating the first order time delayed model as an ODE:

$$ \dot{y} = -\frac{1}{\tau}y + \frac{K}{\tau}u(t-\theta) $$

This system is solved using a numerical ODE solver: [odeint](http://headmyshoulder.github.io/odeint-v2/)