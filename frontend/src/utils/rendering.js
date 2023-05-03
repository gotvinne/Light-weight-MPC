
import { Typography } from "@mui/material"
import { InlineMath } from "react-katex"

/**
 * Render variables in mathematical format
 * @param {number} length 
 * @param {string} identifier 
 * @returns 
 */
export function variableRender(length, identifier) {
    if (length === 0) {
        return (<Typography variant="h5">
             <InlineMath math={`n_{${identifier}} :`}/> 
            </Typography> );
    } else {
        return (<Typography variant="h5"> 
            <InlineMath math={`n_{${identifier}} = ${length}:`}/> 
            </Typography> );
    }
}