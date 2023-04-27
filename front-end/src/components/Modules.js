import React, { useState, useMemo } from "react";
import { AppBar, Tab, Tabs, Toolbar, Typography, Box } from "@mui/material";

import About from "./modules/About";
import Scenario from "./modules/Scenario";
import Algorithm from "./modules/Algorithm";
import Simulation from "./modules/Simulation";
import TabPanel from "./TabPanel";

// Mapping MODULES:
const MODULES = {"scenario": 0, "simulation": 1, "algorithm": 2, "models": 3, "about": 4};

export default function Modules() {
    const keys = Object.keys(MODULES);
    const [module, setModule] = useState(MODULES["scenario"]); 
    const [sim, setSim] = useState("");
    const handleChange = (event: React.SyntheticEvent, newModule: number) => { 
        setModule(newModule); 
    };

    useMemo(() => { // Change module when simulation is received
        if (sim === "") {
            return; 
        } else {
            setModule(MODULES["simulation"]); 
        }
    }, [sim]);
    
    return (
        <React.Fragment>
            <AppBar position="sticky" sx={{height: 50, background: "#b4b4b4"}}>
                <Toolbar sx={{borderColor: 'divider'}}>
                    <Tabs value={module} onChange={handleChange} textColor={"inherit"} sx={{
                            '& .MuiTabs-indicator': { backgroundColor: "#212121" },
                            '& .MuiTab-root': { color: "#212121" },
                            '& .Mui-selected': { color: "#212121" },
                        }}>
                        <Tab label={keys[0]} />
                        <Tab label={keys[1]} />
                        <Tab label={keys[2]} />
                        <Tab label={keys[3]} />
                        <Tab label={keys[4]} />
                    </Tabs>
                </Toolbar>
            </AppBar>

            <TabPanel value={module} width={"inherit"} index={0}>
                <Scenario simHook={setSim} />
            </TabPanel>
            <TabPanel value={module} index={1}>
                <Simulation sim_data={sim}/>
            </TabPanel>
            <TabPanel value={module} index={2}>
                <Algorithm/>
            </TabPanel>
            <TabPanel value={module} index={3}>
                <Box sx={{pt: 2, pl: 2 }}>
                    <Typography> Grete er kjempesøt </Typography>
                </Box>
            </TabPanel>
            <TabPanel value={module} index={4}>
                <About/>
            </TabPanel>

        </React.Fragment>
    )
}
