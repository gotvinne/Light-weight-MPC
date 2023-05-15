
# React-based Front-End:
Developed using application framework [Create-React-App](https://create-react-app.dev/). In order to run the application, Node.js needs to be installed and can be done through a package manager **nvm (Node Version Manager)**. 

This front end is implemented using *React JavaScript* and library [MaterialUI](https://mui.com/). 

### Modules
Folder structure: 
```console
public
src
    components
    css
    img
    systems
    utils
    webassembly.mjs
package-lock.json
package.json
```

**public**: 

**src**: 
The source code defining the user interface. *Components* folder holds all react components rendered in the application 

**package.json**: This is a json file describing all react packages needed in order to deploy the application. 

### React packages
In order to run the application, Download Node.js and npm from [here](https://nodejs.org/en/download)

Install Reack packages
```console
npm install 
```



Insider the *frontend* folder, download react packages 
```console
npm install @mui/material @emotion/react @emotion/styled
npm install @mui/icons-material
npm install react-katex
```

1. Install the tool npm install -g npm-check-updates
2. Verify that your package.json is checked in to version control (the next command will overwrite the package.json)
3. Update the package.json ncu --upgrade
4. Validate the changes to your package.json
5. Install the new packages npm install

### create-react-app 
In order to build a React application, [create-react-app](https://create-react-app.dev/docs/getting-started/) was utilized. 
*node_modules* directory needs to be installed in order to run the app in create-react-app. 

```console
npm install                        # Install needed react-packages
npm start
```
### Plots 
*Plotly* is the JavaScript framework used to plot the simulation data in the app

### Deploying the app to production/server

```console
npm run build
```