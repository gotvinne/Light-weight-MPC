
# React based Front-End:

This front end is implemented using *React JavaScript* and library [MaterialUI](https://mui.com/). 
The front-end communicates with the back-end via Webassembly.

### Modules

Folder structure: 
```console
public
src
    components
    css
    img
package-lock.json
package.json
```

**public**: 

**src**: 

**package.json**: This is a json file describing all react packages needed in order to deploy the application. 

### React packages
In order to run the application, Download Node.js and npm from [here](https://nodejs.org/en/download)

Insider the *frontend* folder, download reack packages 
```console
npm install @mui/material @emotion/react @emotion/styled
npm install @mui/icons-material
npm install react-katex
```
### create-react-app 
In order to build a React application, [create-react-app](https://create-react-app.dev/docs/getting-started/) was utilized. 

*node_modules* directory needs to be installed in order to run the app in create-react-app. 

```console
npm install                        # Install needed react-packages
npm start
```

### Deploying the app to production/server

```console
npm run build
```