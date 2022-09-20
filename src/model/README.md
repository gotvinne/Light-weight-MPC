### MODEL GENERATOR

This is a module for generating customized step-response coefficients from a first order time delayed model. 

$$ \frac{Y(s)}{U(s)}=\frac{K e^{-\theta s}}{\tau s+1}, $$
where $K$, $\theta$ and $\tau$ are respectively the gain, the time delay and the time constant of the system. $U(s)$ is modeled as a step input, $$ U(s) = \frac{\Delta u}{s}, $$ where $\Delta u$ is the model input.  