import React from 'react'
import { Box, Typography, Link } from '@mui/material';

import "../../css/Modules.css"
/**
 * Module describing the background of the project
 */
export default function About() {
  return (
    <div className='About'>
      <Box sx={{width: "70%", pl: "15%"}}>
        <Typography variant="body1" gutterBottom>
          This is a repo for implementing the master's thesis for the study programme Cybernetics & Robotics at NTNU. 
          The thesis is handed out by Equinor, and aims on implementing a simpler software framework simulating optimized control on step response models.
          The software reads a model definition defined in the system definition JSON format. Followingly, it parses the UX defined controller tuning and simulates the specified controller method.
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
          Prof. Lars Struen Imsland (ITK) & Dr. Gisle Otto Eikrem (Equinor)
        </Typography>
        <Typography variant="body1" align="left" gutterBottom>
          The source code can be found at <Link href="https://github.com/Light-weight-MPC" underline="hover"> {'Github'} </Link> and is distributed under the <Link href="https://github.com/gotvinne/Light-weight-MPC/blob/main/LICENSE" underline="hover"> {"BSD-3-Clause license."} </Link>
        </Typography>

        <Typography variant="subtitle1" align='left' sx={{fontWeight: "bold"}} gutterBottom>
          Software usage:
        </Typography>

        <Typography align="left" component="div">
          <ul>
            <li> Navigate to "Scenario" page and define you MPC controller. </li>
            <li> A valid controller definition yields a green "Run Simulation" button. </li>
            <li> After clicking the button, the simulation results shall appear in the "Simulation" page, othervise an error message occurs.</li>
            <li> The mathematical description of the controller and the model structure is respectively defined in the pages "Algorithm" and "Models". </li>
          </ul>
        </Typography>
      </Box>

      <Box sx={{pt: "2%", pl:"15%", display: "flex", flexDirection: "row"}}>
        <Box> 
          <img src={require("./../../img/Logo.png")} alt="Logo" width="120%" />
        </Box>
        <Box sx={{pl: "10%"}}> 
          <img src={require("./../../img/mpc_control_loop.png")} alt="loop" width="130%"/>
          <Typography sx={{pl: "20%"}}> 
            fig: Illustration of the MPC simulation loop
          </Typography>
        </Box>
      </Box>

    </div>
  );
}
