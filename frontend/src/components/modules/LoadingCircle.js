import React from 'react';
import { Box, Typography, CircularProgress } from '@mui/material';

import "../../css/Modules.css"

/**
 * Displaying loading module
 */
export default function LoadingCircle() {
  return (
    <div className='Scenario'>
        <Box sx={{pt: "15%"}}>
            <Typography variant='h3' sx={{fontWeight: "bold"}}>
                Simulating...
            </Typography>
            <Box sx={{pt: "2%"}}/>
            <CircularProgress disableShrink size="5rem"/>
        </Box>
    </div>
  );
}
