import React from 'react'
import { Box, Typography, Link } from '@mui/material';

import "../css/About.css"

export default function About() {
  return (
    <div className='About'>
      <Box>
      <Typography variant="body1" gutterBottom>
        This is a repo for implementing the master's thesis for the study programme Cybernetics & Robotics at NTNU the fall 2022. The thesis is handed out by Equinor, and aims on implementing a simpler software framework simulating optimized control on step response models.
      </Typography>
      <Typography variant="body1" gutterBottom>
      <Link href="https://www.itk.ntnu.no/ansatte/imsland_lars/projects2022.html" underline="hover">
        {'Masters thesis description'}
        </Link>
      </Typography>
      <Typography variant="body1" gutterBottom>
        Master student: Geir Ola Tvinnereim 
      </Typography>
      <Typography variant="body1" gutterBottom>
        Supervisors:
        Prof. Lars Struen Imsland & Dr. Gisle Otto Eikrem
      </Typography>
    </Box>
    </div>
    
  );
}
