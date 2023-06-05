import React from "react";

import Modules from "./components/Modules";
import Header from  "./components/Header";
import Footer from "./components/Footer";
import "./css/App.css"

function App() {  
  localStorage.clear(); // Clear local storage when starting server
  return (    
  <div className="App"> 
      <Header /> 
      <Modules />
      <Footer />
  </div>  
  );
}
    
export default App;