import React from "react";
import { Box, Typography, TextField } from '@mui/material';
import { BlockMath } from 'react-katex';

const SYMBOLS = [`P =`, `M =`,`W =`,`Q =`,`R =`,`\\rho_h =`,`\\rho_l =`];

/**
 * Displaying the MPC tuning TextFields
 * @param {Number} ncv Number of controlled variables
 * @param {Number} nmv Number of manipulated variables  
 * @param {React.useState} error hook
 * @param {React.useState} sce hook
 * @param {Function} handler callback function
 */
export default function Tuning({ncv, nmv, error, sce, handler}) {
    return (
        <div> 
            <Box align="left" sx={{pl: "20%", pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
        
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: "3%"}}>
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[0]} />
                        <TextField sx={{pl:"2%"}} error={error["P"]} id={"P"} variant="outlined" helperText={"Prediction horizon, int"} value={sce["P"]} onChange={handler} required/>
                    </Box>
                    <Box />
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[1]} />
                        <TextField sx={{pl:"2%"}} error={error["M"]} id={"M"} variant="outlined" helperText={"Control horizon, int"} value={sce["M"]} onChange={handler} required/>
                    </Box>
                    <Box />
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[2]} />
                        <TextField sx={{pl:"2%"}} error={error["W"]} id={"W"} variant="outlined" helperText={"Start horizon, int"} value={sce["W"]} onChange={handler} required/>
                    </Box>
                </Box>

                <Box sx={{pt:2, pl: "3%"}}>
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[3]} />
                        <TextField sx={{pl:"2%"}} error={error["Q"]} id={"Q"} variant="outlined" helperText={"vector<double>, length: "+ ncv.toString()} value={sce["Q"]} onChange={handler} required/>
                    </Box>
                    <Box />
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[4]} />
                        <TextField sx={{pl:"2%"}} error={error["R"]} id={"R"} variant="outlined" helperText={"vector<double>, length: "+ nmv.toString()} value={sce["R"]} onChange={handler} required/>
                    </Box>
                    <Box />
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[5]} />
                        <TextField sx={{pl:"2%"}} error={error["RoH"]} id={"RoH"} variant="outlined" helperText={"vector<double>, length: "+ ncv.toString()} value={sce["RoH"]} onChange={handler} required/>
                    </Box>
                    <Box />
                    <Box sx= {{display: "flex", flexDirection: "colum"}}> 
                        <BlockMath sx={{pt: "2%"}} math={SYMBOLS[6]} />
                        <TextField sx={{pl:"2%"}} error={error["RoL"]} id={"RoL"} variant="outlined" helperText={"vector<double>, length: "+ ncv.toString()} value={sce["RoL"]} onChange={handler} required/>
                    </Box>
                </Box>
            </Box>
        </div>
    );
}