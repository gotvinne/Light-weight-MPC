import React from "react";

import MenuBar from "./components/MenuBar";
import Footer from "./components/Footer"
import "./css/App.css"

function App() {  
  return (    
  <div className="App"> 
      <div className="Header">
          <h1> Light-weight MPC </h1>
      </div>   
      <MenuBar />
      <Footer />
  </div>  
  );
}
    
export default App;