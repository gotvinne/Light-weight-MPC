import React from "react";
import { Box, Typography, Link } from "@mui/material";
import { BlockMath, InlineMath } from 'react-katex';

import "../../css/Modules.css"
import 'katex/dist/katex.min.css';

/**
 * Informative component describing the dynamical models
 */
export default function Models() {
    return (
        <div className="Algorithm">
            <Box sx={{ width: '75%', pl: "5%", pt: "3%"}}>
                <Typography variant="h4" sx={{fontWeight: 'bold'}}> Finite Step Response Model, FSRM </Typography>
                <Typography variant="body1" gutterBottom>
                Implemented 
                using <Link href="https://eigen.tuxfamily.org/index.php?title=Main_Page" underline="hover"> {"Eigen"} </Link> software for linear algebra:
                </Typography>
                <Typography variant="body1" gutterBottom>
                    The FSRModel is a C++ object creating the dynamics of a general linear step response model. 
                    The following equations and matrices are avaliable by calling the FSRModel functionality.
                </Typography>

                <Typography variant="subtitle1" sx={{fontWeight: "bold"}}> Model definition: </Typography>
                <Typography>
                    Instead of using states to describe the dynamics for a given time <InlineMath math={"t"} /> the step response models uses step response coefficients, <InlineMath math={"s"} /> to describe the relation between the input and output. 
                    For an arbitrary FSR model, the model is describe by <InlineMath math={"N"} /> step response coefficients. 
                    Furthermore, in order to describe how the change in actuation affects the outputs and the predicted outputs, three matrices are defined: 
                    <InlineMath math={"\\quad \\boldsymbol{\\Theta, \\Phi, \\Psi}"} />. The first matrix determines how the step response coefficients are used to predict future outputs, while the others describes the dynamics of past actuation. 
                </Typography>
                <Typography>
                    In industrial MPC applications, output variables are denoted "Controlled variables (CV)" and input variables "Manipulated variables (MV)". Hence, <InlineMath math={"n_{CV}"} /> and <InlineMath math={"n_{MV}"} /> denote the number of inputs and outputs.
                </Typography>

                <Typography variant="subtitle1" sx={{fontWeight: "bold"}}> Output prediction equation: </Typography>
                <BlockMath math={`\\tilde{Y}(k+P) = \\boldsymbol{\\Theta} \\Delta U(k+M)+ \\boldsymbol{\\Phi} \\Delta \\tilde{U}(k)+\\boldsymbol{\\Psi} \\tilde{U}(k-N)+B(k) = \\boldsymbol{\\Theta} \\Delta U(k+M) + \\boldsymbol{\\Lambda}(k) `} />
                
                <Typography>
                    When using the FSRM in an MPC optimization problem, the model prediction matrices is defined as follows: 
                </Typography>
                <Box sx={{pl: "3%", pt: 2, display: "flex", flexDirection: "row"}}>
                    <BlockMath math={`\\boldsymbol{\\Theta} \\triangleq\\left[\\begin{array}{cccc}
                                        \\boldsymbol{S}_{11} & \\boldsymbol{S}_{12} & \\cdots & \\boldsymbol{S}_{1 n_{MV}} \\\\\n
                                        \\boldsymbol{S}_{21} & \\cdots & \\cdots & \\boldsymbol{S}_{2 n_{MV}} \\\\\n
                                        \\vdots & \\vdots & \\vdots & \\vdots \\\\\n
                                        \\boldsymbol{S}_{n_{CV} 1} & \\cdots & \\cdots & \\boldsymbol{S}_{n_{CV} n_{MV}} \\end{array}\\right]_{n_{CV} \\cdot (P-W) \\times M \\cdot n_{MV}},  `} />
                        <Box width={"2%"}/>
                        <BlockMath math={`\\boldsymbol{S_{i,j}} = \\left[\\begin{array}{cccc}
                                        s_W & s_{W-1} & \\cdots & 0 \\\\
                                        s_{W+1} & s_W & 0 & \\vdots \\\\
                                        \\vdots & \\vdots & \\ddots & 0 \\\\
                                        s_M & s_{M-1} & \\cdots & s_1 \\\\
                                        s_{M+1} & s_M & \\cdots & s_2 \\\\
                                        \\vdots & \\vdots & \\ddots & \\vdots \\\\
                                        s_P & s_{P-1} & \\cdots & s_{P-M}
                                        \\end{array}\\right]_{\\left(P - W\\right) \\times M}. `} />
                </Box>


                <Box sx={{pl: "3%", pt: 2, display: "flex", flexDirection: "row"}}>
                    <BlockMath math={`\\boldsymbol{\\Phi}=\\left[\\begin{array}{cccc}
                                    \\boldsymbol{\\Phi_{1,1}} & \\boldsymbol{\\Phi_{1,2}} & \\cdots & \\boldsymbol{\\Phi_{1, n_{M V}}} \\\\\n
                                    \\boldsymbol{\\Phi_{2,1}} & \\boldsymbol{\\Phi_{2,2}} & \\cdots & \\boldsymbol{\\Phi_{2, n_{M V}}} \\\\\n
                                    \\vdots & \\vdots & \\ddots & \\vdots \\\\\n
                                    \\boldsymbol{\\Phi_{n_{C V}, 1}} & \\boldsymbol{\\Phi_{n_{C V}, 2}} & \\cdots & \\boldsymbol{\\Phi_{n_{C V}, n_{M V}}}
                                    \\end{array}\\right]_{n_{C V}\\left(P-W\\right) \\times \\sum_{j=1}^{n_{M V}}\\left(N-W-1\\right)}, `} />
                    <Box width={"2%"}/>
                    <BlockMath math={`\\boldsymbol{\\Phi_{i, j}} =\\left[\\begin{array}{ccccc}
                                    s_{W+1} & s_{W+2} & \\ldots & s_{N-2} & s_{N-1} \\\\\n
                                    s_{W+2} & s_{W+3} & \\ldots & s_{N-1} & s_{N}\\\\\n
                                    \\vdots & \\vdots & \\vdots & \\vdots & \\vdots \\\\\n
                                    s_{P+1} & s_{P+2} & \\ldots & s_{N} & s_{N}
                                    \\end{array}\\right]_{\\left(P-W\\right) \\times N-W-1} `} />

                </Box>
                
                <Box sx={{pl: "3%", pt: 2, display: "flex", flexDirection: "row"}}>
                    <BlockMath math={`\\boldsymbol{\\Psi} =\\left[\\begin{array}{cccc}
                                    \\Psi_{1,1} & \\Psi_{1,2} & \\cdots & \\Psi_{1, n_{M V}} \\\\\n
                                    \\Psi_{2,1} & \\Psi_{2,2} & \\cdots & \\Psi_{2, n_{M V}} \\\\\n
                                    \\vdots & \\vdots & \\ddots & \\vdots \\\\\n
                                    \\Psi_{n_{C V}, 1} & \\Psi_{n_{C V}, 2} & \\cdots & \\Psi_{n_{C V}, n_{M V}}
                                    \\end{array}\\right]_{n_{C V}\\left(P-W\\right) \\times n_{M V}}, `} />
                    <Box width={"2%"}/>
                    <BlockMath math={`\\Psi_{i, j} =\\left[\\begin{array}{c}
                                    s_{N} \\\\\n
                                    s_{N} \\\\\n
                                    \\vdots \\\\\n
                                    s_{N}
                                    \\end{array}\\right]_{\\left(P-W\\right) \\times 1} `} />

                </Box>
            </Box>
        </div>
    );
}