import React from "react";
import { Box, Typography, Link, Table, TableContainer, TableHead, TableRow, TableCell, TableBody, Paper  } from "@mui/material";

import "../../css/Modules.css"

import 'katex/dist/katex.min.css';
import { BlockMath, InlineMath } from 'react-katex';

function createData(
    name: string,
    num: string,
  ) {
    return { name, num };
  }
  
  const rows = [
    createData('z_{cd}', `M \\cdot n_{MV} + 2 \\cdot n_{CV}`),
    createData('\\boldsymbol{\\bar{Q}}', `n_{CV} \\cdot (P - W) \\times n_{CV} \\cdot (P - W)`),
    createData('\\boldsymbol{\\bar{R}}', `M \\cdot n_{MV}  \\times M \\cdot n_{MV}`),
    createData('\\boldsymbol{\\Theta}', `n_{CV} \\cdot (P-W) \\times M \\cdot n_{MV}`),
    createData('\\boldsymbol{\\Phi}', `n_{C V}\\left(P-W\\right) \\times \\sum_{j=1}^{n_{M V}}\\left(N-W-1\\right)`),
    createData('\\boldsymbol{\\Psi}', `n_{C V}\\left(P-W\\right) \\times n_{M V}`),
    createData('\\boldsymbol{K}', `M \\cdot n_{MV} \\times M \\cdot n_{MV}`),
    createData('\\boldsymbol{\\Gamma}', `M \\cdot n_{MV} \\times n_{MV}`),
    createData('\\boldsymbol{A}', `2M \\cdot n_{MV} + P \\cdot n_{CV} \\times M \\cdot n_{MV}`)
  ];

/**
 * Module describing the MPC algorithm
 */
export default function Algorithm() {

    return (
        <div className="Algorithm">
            <Box sx={{ width: '75%', pl: "5%", pt: "3%"}}>
                <Typography variant="h4" sx={{fontWeight: 'bold'}}> MPC algorithm: </Typography>
                <Typography variant="body1" gutterBottom>
                Implemented using <Link href="https://github.com/gotvinne/Light-weight-MPC/tree/main/src/OSQP-Eigen" underline="hover"> {"osqp-eigen"} </Link>
                C++ wrapper for the <Link href="https://osqp.org/" underline="hover"> {"OSQP"} </Link> software:
                </Typography>
                <Typography variant="body1" gutterBottom>
                The OSQP, operator splitting QP solver solves the problems of the following form:
                </Typography>
                <BlockMath math={`min \\frac{1}{2} z^T \\boldsymbol{H} z+q^T z \\quad s.t. \\quad \\ l \\leq \\boldsymbol{A} z \\leq u `} />
                <Typography> 
                    Given, <InlineMath math={"H"} /> is a positive definite matrix, yielding a convex quadratic program. The OSQP solver uses an custom ADMM-based first order method and is one of the fastest QP solvers avaliable. 
                </Typography>
                <Typography variant="subtitle1" sx={{fontWeight: "bold"}}> Step Response MPC solver: </Typography>
                <Typography variant="body1" gutterBottom>
                This MPC controller is defined as a standard quadratic program (QP) where the cost aims to minimize the error between the output <InlineMath math={"Y"}/> and the reference <InlineMath math={"r_y"} />:
                </Typography>

                <BlockMath math={`\\min \\sum_{j=W}^{P}\\left|(y(k+j \\mid k)-r_y(k+j))\\right|_{\\bar{Q}}^2+ \\sum_{j=0}^{(M-1)} \\left|\\Delta u(k+j)\\right|_{\\bar{R}}^2+\\bar{\\rho} \\bar{\\epsilon}+\\underline{\\rho} \\underline{\\epsilon}`} />
                <Typography variant="body1" gutterBottom>
                The cost function is constrained by the model definition and relating variables. The general FSRM-MPC algorithm can be summaried as:
                </Typography>
                <BlockMath math={`min \\quad Y(k+(P-W))^T \\boldsymbol{\\bar{Q}} Y(k+(P-W))+\\Delta U(k+(M-1))^T \\boldsymbol{\\bar{R}} \\Delta U(k+(M-1)) \\\\\ 
                                        -2 \\mathcal{T}(k)^T \\boldsymbol{\\bar{Q}} Y(k+(P-W))+\\rho_h^T \\epsilon_h+\\rho_l^T \\epsilon_l,`} />
                <Typography variant="body1" gutterBottom>
                such that:
                </Typography>
                <BlockMath math={`Y(k+(P-W))  = \\boldsymbol{\\Theta} \\Delta U(k+(M-1))+ \\boldsymbol{\\Phi} \\Delta \\tilde{U}(k)+\\Psi \\tilde{U}(k-N)+B(k), \\
                                     = \\boldsymbol{\\Theta} \\Delta U(k)+ \\hat{\\boldsymbol{Y}}^{\\boldsymbol{o}}(k+P) + B(k), \\
                                    = \\boldsymbol{\\Theta} \\Delta U(k)+ \\boldsymbol{\\Lambda}(k),`} />
                <BlockMath math={`U(k) = \\boldsymbol{K}^{-1}(\\Gamma \\tilde{U}(k-1) + \\Delta U(k)), \\quad B(k) = Y(k) - \\hat{Y}(k), \\quad \\boldsymbol{K} \\succ 0`} />
                <BlockMath math={`Y(k+j) = Y(k+j) + B(k), \\quad j \\in\\left\\{W, \\ldots, P\\right\\}`} />
                <BlockMath math={`\\Delta \\underline{U} \\leq \\Delta U(k+j) \\leq \\Delta \\overline{U}, \\quad j \\in\\left\\{0, \\ldots, M-1\\right\\}`} />
                <BlockMath math={`\\underline{U} \\leq U(k+j) \\leq \\bar{U}, \\quad j \\in\\left\\{0, \\ldots, M-1\\right\\}`} />
                <BlockMath math={`\\underline{Y}- \\epsilon_l \\leq Y(k+j) \\leq \\bar{Y}+ \\epsilon_h, \\quad \\epsilon_h \\geq 0, \\epsilon_l \\geq 0, \\quad j \\in\\left\\{W, \\ldots, P\\right\\}`} />

                <Typography variant="subtitle1" sx={{fontWeight: "bold"}}> Condensed Form: </Typography>
                <Typography variant="body1" gutterBottom>
                The condensed formulation solves a smaller optimalization problem, obtained using the Nullspace method. This method reduces the number of optimalization variabled by defining a linear transform. <InlineMath math={"z_{st} = A z_{cd} + C \\quad"} /> 
                The transform cancels optimalization variables with the given constraints, yielding a computationally easier problem. </Typography>
                <Typography>
                The original optimalization vector, <InlineMath math={"z_{st} = \\begin{bmatrix} \\Delta U \\\\ U \\\\ Y \\\\ \\epsilon_h \\\\ \\epsilon_l \\end{bmatrix} \\quad"} />, is reduced to 
                <InlineMath math={"z_{cd} = \\begin{bmatrix} \\Delta U \\\\ \\epsilon_h \\\\ \\epsilon_l \\end{bmatrix}."} />
                </Typography>

                <Typography>
                    The consdenced formulation is formulated followingly:
                </Typography>
                <BlockMath math={`\\min_{z_{cd}} \\quad \\frac{1}{2} z_{cd}^T \\boldsymbol{G_{cd}} z_{cd}+q^T_{cd} z_{cd}`} />
                <BlockMath math={`\\boldsymbol{G_{cd}} = 2 \\cdot blkdiag( \\boldsymbol{\\Theta}^{T} \\boldsymbol{\\bar{Q}} \\boldsymbol{\\Theta} + \\boldsymbol{\\bar{R}}, \\boldsymbol{0}, \\boldsymbol{0}),`} />
                <BlockMath math={`q_{cd}(k) = \\begin{bmatrix} 2 \\cdot \\boldsymbol{\\Theta}^T\\boldsymbol{\\bar{Q}}(\\Lambda(k) - \\mathcal{T}(k)) \\\\ \\rho_{h} \\\\ \\rho_{l} \\end{bmatrix},`} />
                <Typography sx={{pl:"30%"}}> such that: </Typography>
                <BlockMath math={`\\underline{z}_{cd}(k) = \\begin{bmatrix}
                                \\Delta \\underline{U} \\\\ \\underline{U} \\\\ -\\infty \\\\ \\underline{Y} \\\\ 0 \\\\ 0
                                \\end{bmatrix} - \\begin{bmatrix} 0 \\\\ \\mathbf{K}^{-1} \\mathbf{\\Gamma} U(k-1) \\\\ \\mathbf{\\Lambda}(k) \\\\ \\mathbf{\\Lambda}(k)\\\\ 0 \\\\ 0 \\end{bmatrix} \\leq 
                                \\begin{bmatrix}
                                \\mathbf{I} & \\mathbf{0} & \\mathbf{0} \\\\
                                \\mathbf{K^{-1}} & \\mathbf{0} & \\mathbf{0} \\\\
                                \\mathbf{\\Theta} & \\mathbf{-I} & \\mathbf{0} \\\\
                                \\mathbf{\\Theta} & \\mathbf{0} & \\mathbf{I} \\\\
                                \\mathbf{0} & \\mathbf{I} & \\mathbf{0} \\\\
                                \\mathbf{0} & \\mathbf{0} & \\mathbf{I} \\\\
                                \\end{bmatrix} z_{cd} \\leq \\begin{bmatrix}
                                \\Delta \\bar{U} \\\\ \\bar{U} \\\\ \\bar{Y} \\\\ \\infty \\\\ \\infty \\\\ \\infty
                                \\end{bmatrix} - \\begin{bmatrix} 0 \\\\ \\mathbf{K}^{-1} \\mathbf{\\Gamma} U(k-1) \\\\ \\mathbf{\\Lambda}(k) \\\\ \\mathbf{\\Lambda}(k) \\\\ 0 \\\\ 0 \\end{bmatrix} = \\bar{z}_{cd}(k) `} />

                <Typography sx={{pt: "2%"}} variant="h5"> Overview of the dimensions used in the controller: </Typography>               
                <Box sx={{pl: "30%", pt: "2%"}}> 
                    <TableContainer sx={{width: "50%"}} component={Paper}>
                        <Table sx={{ width: "100%" }} aria-label="simple table">
                            <TableHead>
                                <TableRow>
                                    <TableCell>Matrix Expression</TableCell>
                                    <TableCell > Condensed dimensions </TableCell>
                                </TableRow>
                            </TableHead>
                            <TableBody>
                                {rows.map((row) => (
                                    <TableRow
                                        key={row.name}
                                        sx={{ '&:last-child td, &:last-child th': { border: 0 } }}
                                        >
                                        <TableCell component="th" scope="row"> <InlineMath math={row.name}/> </TableCell>
                                        <TableCell align="center"> <InlineMath math={row.num} /> </TableCell>
                                    </TableRow>
                                ))}
                            </TableBody>
                        </Table>
                    </TableContainer>
                </Box>    
                <Box sx={{pt: "2%"}}/>
            </Box>
        </div>
    );
}