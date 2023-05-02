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
                <Typography variant="body1" gutterBottom>
                Implemented using <Link href="https://github.com/gotvinne/Light-weight-MPC/tree/main/src/OSQP-Eigen" underline="hover"> {"osqp-eigen"} </Link>
                C++ wrapper for the <Link href="https://osqp.org/" underline="hover"> {"OSQP"} </Link> software:
                </Typography>
                <Typography variant="body1" gutterBottom>
                The OSQP, operator splitting QP solver solves the problems of the following form:
                </Typography>
                <BlockMath math={`min \\frac{1}{2} z^T \\boldsymbol{H} z+q^T z \\quad s.t. \\quad \\ l \\leq \\boldsymbol{A} z \\leq u `} />
                <Typography variant="h5"> Step Response MPC solver: </Typography>
                <Typography variant="body1" gutterBottom>
                This solver solves the QP defining the MPC problem for a finite step response model (FSRM) in terms of only one optimization variable (condensed form). This form is obtained by using the null space method on the optimilization problem formulated by quadratic constraining the output <InlineMath math={`Y`} />and input <InlineMath math={`\\Delta Y`} />: 
                </Typography>

                <BlockMath math={`\\min \\sum_{j=W}^{P}\\left|(y(k+j \\mid k)-r_y(k+j))\\right|_{\\bar{Q}}^2+ \\sum_{j=0}^{(M-1)} \\left|\\Delta u(k+j)\\right|_{\\bar{R}}^2+\\bar{\\rho} \\bar{\\epsilon}+\\underline{\\rho} \\underline{\\epsilon}`} />
                <BlockMath math={`\\min \\quad Y(k+w)^TQY(k+w) + \\Delta U(k+i)^TR\\Delta U(k+i) `} />
                <Typography variant="h5"> Condensed Form: </Typography>
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