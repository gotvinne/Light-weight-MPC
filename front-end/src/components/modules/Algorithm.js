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
    createData('Y', "P x n_CV")
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
                <BlockMath math={`\\min \\quad \\frac{1}{2}z_{cd}^T(2\\Theta^T\\bar{Q}\\Theta + 2\\bar{R})z_{cd} + 2\\Theta^TQ(\\Lambda(k)-\\tau(k))z_{cd} \\
                                = \\frac{1}{2} z_{cd}^T  \\boldsymbol{H} z_{cd}+q^T z_{cd}`} />

                <BlockMath math={`\\Lambda(k) = \\boldsymbol{\\Phi} \\Delta \\tilde{U}(k)+\\Psi \\tilde{U}(k-N)+B(k)`} />

                <TableContainer sx={{width: "30%"}} component={Paper}>
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
                                    <TableCell component="th" scope="row">
                                        {row.name}
                                    </TableCell>
                                    <TableCell align="center">{row.num}</TableCell>
                                </TableRow>
                            ))}
                        </TableBody>
                    </Table>
                </TableContainer>    
            </Box>
        </div>
    );
}