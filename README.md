# Project thesis: Light-weight-MPC

This is a repo for implementing the project thesis for the study programme Cybernetics & Robotics at **NTNU Trondheim** the fall 2022. The thesis is handed out by **Equinor**, and aims on implementing a simpler software framework simulating optimized control on step response models. 

[Project thesis description](https://www.itk.ntnu.no/ansatte/imsland_lars/projects2022.html)

#### Master student: 
- Geir Ola Tvinnereim gotvinne@stud.ntnu.no

#### Supervisors:
- Lars Struen Imsland (ITK) lars.imsland@ntnu.no
- Gisle Otto Eikrem (Equinor) gise@equinor.com

##### Input format
```json  
{
 "model": "modelname",
 "weights": [],
 "horizon": 10
}
``` 
[Json parser C++](https://linuxhint.com/parse-json-data-cpp/)

##### Output format
```json  
{
 "simulation": [
    "t0": value,
    "t1": value,
    "t2": value,
    "t3": value,
    "t4": value,
    ],
 "horizon": 10
}
``` 

### Dependancies:
- OSQP, QP-solver [https://osqp.org/]