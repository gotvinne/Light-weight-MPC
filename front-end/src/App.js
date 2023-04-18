import React from "react";

import Modules from "./components/Modules";
import Footer from "./components/Footer"
import "./css/App.css"

function App() {  
  return (    
  <div className="App"> 
      <div className="Header">
          <h1> Light-weight MPC </h1>
      </div>   
      <Modules />
      <Footer />
  </div>  
  );
}
    
export default App;