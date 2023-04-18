
import { Typography } from "@mui/material"
import { InlineMath } from "react-katex"

export function variableRender(length, identifier) {
    if (length === 0) {
        return (<Typography sx={{pl: 5}} variant="h5">
             <InlineMath math={`n_{${identifier}} :`}/> 
            </Typography> );
    } else {
        return (<Typography sx={{pl: 5}} variant="h5"> 
            <InlineMath math={`n_{${identifier}} = ${length}:`}/> 
            </Typography> );
    }
}