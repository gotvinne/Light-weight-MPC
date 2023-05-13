import * as React from 'react';
import CircularProgress from '@mui/material/CircularProgress';
import { Box, Typography } from '@mui/material';

import "../../css/Modules.css"

export default function ProgressCircle() {
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
