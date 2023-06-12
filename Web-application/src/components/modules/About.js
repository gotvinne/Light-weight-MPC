import React from 'react'
import { Box, Typography, Link, Table, TableContainer, TableHead, TableRow, TableCell, TableBody, Paper } from '@mui/material';
import { InlineMath } from 'react-katex';
import "../../css/Modules.css"

function createData(
  name: string,
  num: string,
) {
  return { name, num };
}

const tableData = [
  createData('P', `P \\in [M, N_{*}]`),
  createData('M', `M \\in [W, P]`),
  createData('W', `W \\in [0, M]`),
  createData('T', `T > P`),
  createData('\\boldsymbol{Q}', `\\boldsymbol{Q} \\geq 0, \\quad \\boldsymbol{Q}.length == n_{CV}`),
  createData('\\boldsymbol{R}', `\\boldsymbol{R} \\geq 0, \\quad \\boldsymbol{R}.length == n_{MV}`),
  createData('\\rho_h', `\\rho_h \\geq 0, \\quad \\rho_{h}.length == n_{CV}`),
  createData('\\rho_l', `\\rho_l \\geq 0, \\quad \\rho_{l}.length == n_{CV}`),
  createData('\\overline{z}', `\\overline{z}_i > \\underline{z}_i, \\quad i \\in \\{0, \\ldots ,  2 \\cdot n_{MV} + n_{CV}\\} \\quad \\overline{z}.length == 2 \\cdot n_{MV} + n_{CV}`),
  createData('\\underline{z}', `\\underline{z}_i < \\overline{z}_i, \\quad i \\in \\{0, \\ldots ,  2 \\cdot n_{MV} + n_{CV}\\} \\quad \\underline{z}.length == 2 \\cdot n_{MV} + n_{CV}`)
];
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
      
        <Typography sx={{fontWeight: "bold"}} align="left" variant="subtitle1"> Parameter criteria for valid simulation: </Typography>
        <Box sx={{pt: "2%"}} align="center"> 
            <TableContainer  sx={{width: "70%"}} component={Paper}>
                <Table sx={{ width: "100%" }} aria-label="simple table">
                    <TableHead>
                        <TableRow>
                            <TableCell> Parameter </TableCell>
                            <TableCell sx={{pl: "20%"}}> Criteria </TableCell>
                        </TableRow>
                    </TableHead>
                    <TableBody>
                        {tableData.map((row) => (
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
      </Box> 
      <Box sx={{pt: "2%"}}/>

      <Box sx={{pt: "2%", pl:"15%", display: "flex", flexDirection: "row"}}>
        <Box sx={{pl: "10%"}}> 
          <img src={require("../../img/Logo.png")} alt="Logo" width="110%" />
          <Typography > 
            fig: Software logo
          </Typography>
        </Box>
        <Box sx={{pl: "10%"}}> 
          <img src={require("../../img/outputfeedback.png")} alt="loop" width="100%"/>
          <Typography > 
            fig: Illustration of the MPC simulation loop
          </Typography>
        </Box>
      </Box>

    </div>
  );
}
