import React, { useState, useMemo } from "react";
import { AppBar, Tab, Tabs, Toolbar } from "@mui/material";

import About from "./modules/About";
import Scenario from "./modules/Scenario";
import Algorithm from "./modules/Algorithm";
import Models from "./modules/Models";
import Simulation from "./modules/Simulation";
import TabPanel from "./TabPanel";
import LoadingCircle from "./modules/LoadingCircle";

// Mapping MODULES:
const MODULES = {"scenario": 0, "simulation": 1, "algorithm": 2, "models": 3, "about": 4};
const keys = Object.keys(MODULES);
/**
 * Defining the modules of the frontend
 */
export default function Modules() {
    
    const [module, setModule] = useState(MODULES["scenario"]); 
    const [sim, setSim] = useState("");
    const [loading, setLoading] = useState(false);
    const [simError, setSimError] = useState("");

    const handleChange = (event: React.SyntheticEvent, newModule: number) => { 
        setModule(newModule); 
    };

    useMemo(() => { // Change module when simulation is received
        if (sim === "") {
            return; 
        } else if (sim === "Simulating") {
            setLoading(true);
        } else if (sim === "SIM ERROR") {
            setLoading(false);
        } else { 
            setModule(MODULES["simulation"]); 
            setLoading(false);
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
                {loading 
                    ? <LoadingCircle/>
                    : <Scenario simError={simError} simHook={setSim} setSimErrorHook={setSimError}/>
                    }
            </TabPanel>
            <TabPanel value={module} index={1}>
                <Simulation simData={sim}/>
            </TabPanel>
            <TabPanel value={module} index={2}>
                <Algorithm/>
            </TabPanel>
            <TabPanel value={module} index={3}>
                <Models />
            </TabPanel>
            <TabPanel value={module} index={4}>
                <About/>
            </TabPanel>

        </React.Fragment>
    )
}
