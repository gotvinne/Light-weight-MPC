
# Web-application: React JavaScript
Developed using application framework [Create-React-App](https://create-react-app.dev/). In order to run the application, Node.js and npm need to be installed. Download Node.js and npm from [here](https://nodejs.org/en/download).

Operating system: Windows, MacOS, Linux

This front end is implemented using *React JavaScript* and the React library [MaterialUI](https://mui.com/). 

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
### React packages
Check package.json for react dependancies.

**React dependancies**: 
- "@emotion/react": "^11.10.6",
- "@emotion/styled": "^11.10.6",
- "@mui/icons-material": "^5.11.16",
- "@mui/material": "^5.12.0",
- "@testing-library/jest-dom": "^5.16.5",
- "@testing-library/react": "^14.0.0",
- "@testing-library/user-event": "^14.4.3",
- "@types/jest": "^29.5.1",
- "@types/react": "^18.2.0",
- "@types/react-dom": "^18.2.1",
- "buffer": "^6.0.3",
- "plotly.js": "^2.22.0",
- "react": "^18.2.0",
- "react-app-polyfill": "^3.0.0",
- "react-dom": "^18.2.0",
- "react-katex": "^3.0.1",
- "react-native-web": "^0.19.4",
- "react-plotly.js": "^2.6.0",
- "react-scripts": "^5.0.1",
- "require-context": "^1.1.0",
- "stream": "^0.0.2",
- "web-vitals": "^3.3.1"

### create-react-app 
In order to build a React application, [create-react-app](https://create-react-app.dev/docs/getting-started/) was utilized. 
*node_modules* directory needs to be installed in order to run the app in create-react-app. 

```console
npm install                        # Install needed react-packages
npm start
```
### Plots 
[Plotly](https://plotly.com/javascript/react/) is the JavaScript framework used to plot the simulation data in the app. The most used chart is [Line Chart](https://plotly.com/javascript/line-charts/)

### Deploying the app to production/server

```console
npm run build
```
This command creates a build directory with a production build. The software can for instance be deployed using a static server
```console
npm install -g serve
serve -s build
```