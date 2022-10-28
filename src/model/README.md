## MODELS

### The Step response model

This is a module for generating customized step-response coefficients from a first order time delayed model. 

$$ \frac{Y(s)}{U(s)}=\frac{K e^{-\theta s}}{\tau s+1}, $$
where $K$, $\theta$ and $\tau$ are respectively the gain, the time delay and the time constant of the system. $U(s)$ is modeled as a step input, $$ U(s) = \frac{\Delta u}{s}, $$ where $\Delta u$ is the model input.  

### Reference

In order to simulate the MPC scenario realistically, a reference system generating a measured output is desired. This model represents the output of an avaliable sensor for the SISO case. The measured value can be used to realize output feedback by obtaining a bias correction term. 

By reformulating the first order time delayed model as an ODE:

$$ \dot{y} = -\frac{1}{\tau}y + \frac{K}{\tau}u(t-\theta) $$

This system is solved using a numerical ODE solver: [odeint](http://headmyshoulder.github.io/odeint-v2/)