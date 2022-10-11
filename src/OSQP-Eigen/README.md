
# Module implementing and solving QP problem

This is done by using OSQP, implemented using OSQP-Eigen C++ wrapper.

[Example](https://osqp.org/docs/examples/setup-and-solve.html)
[MPC_example](https://github.com/robotology/osqp-eigen/blob/master/example/src/MPCExample.cpp)


The OSQP, operator splitting QP solver solves the problems of the following form:

$$ min \quad \frac{1}{2} z^T H z+q^T z $$
subject to $$ l \leq A z \leq u $$ 
