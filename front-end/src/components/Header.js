import React from "react";
import { AppBar, Tab, Tabs, Toolbar, Typography, Box } from "@mui/material";

// Components
import About from "./About";
import Scenario from "./Scenario";
import Algorithm from "./Algorithm";
import Simulation from "./Simulation";

import '../css/Header.css'
const Pages = ["Scenario", "Simulation", "Algorithm", "Models", "about"];

interface TabPanelProps { // Struct
    children?: React.ReactNode;
    index: number;
    value: number;
  }

function TabPanel(props: TabPanelProps) {
const { children, value, index, ...other } = props;

return ( // Render react node based on value
    <div role="tabpanel" hidden={value !== index} id={`simple-tabpanel-${index}`}
    aria-labelledby={`simple-tab-${index}`} 
    {...other}
    > 
    {value === index && ( children )} 
    </div>
);
}

function Header(props: Props) {
    const [value, setValue] = React.useState(0); // Set initial value
    const handleChange = (event: React.SyntheticEvent, newValue: number) => { // Handle callback
        setValue(newValue);
    };
    
    return (
        <React.Fragment>
            <div className="Header">
                <h1> Light Weight MPC </h1>
            </div>
            <AppBar position="sticky" sx={{height: 50, background: "#b4b4b4"}}>
                <Toolbar sx={{borderColor: 'divider'}}>
                    <Tabs value={value} onChange={handleChange} textColor={"inherit"} sx={{
                            '& .MuiTabs-indicator': { backgroundColor: "#212121" },
                            '& .MuiTab-root': { color: "#212121" },
                            '& .Mui-selected': { color: "#212121" },
                        }}>
                        <Tab label={Pages[0]} />
                        <Tab label={Pages[1]} />
                        <Tab label={Pages[2]} />
                        <Tab label={Pages[3]} />
                        <Tab label={Pages[4]} />
                    </Tabs>
                </Toolbar>
            </AppBar>
            <TabPanel value={value} width={"inherit"} index={0}>
                <Scenario/>
            </TabPanel>
            <TabPanel value={value} index={1}>
                <Simulation />
            </TabPanel>
            <TabPanel value={value} index={2}>
                <Algorithm/>
            </TabPanel>
            <TabPanel value={value} index={3}>
                <Box sx={{pt: 2, pl: 2 }}>
                    <Typography> Hei </Typography>
                </Box>
            </TabPanel>
            <TabPanel value={value} index={4}>
                <About/>
            </TabPanel>
        </React.Fragment>
    )
}

export default Header
