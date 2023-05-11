import numpy as np
import json
import os
from pathlib import Path

# Simulation file specifiers
SCENARIO = "scenario"
T = "T"
N_CV = "n_CV"
N_MV = "n_MV"

CV = "CV"
MV = "MV"
OUTPUT = "output"
INPUT = "input"
UNIT = "unit"
CONSTRAINT = "c"
Y = "y"
Y_PRED = "y_pred"
U = "u"


class SimulationData():
    def __init__(self, filepath: str):
        """
        Constructor for the SimulationData object parsing a simulation file
        :param filepath: The local filepath to simulation data
        """
        simulation_filepath = str(Path(__file__).parent.parent) + filepath  # Relative filepath
        json_data = self.ReadSimulation(simulation_filepath)

        self.scenario = json_data[SCENARIO]
        self.T = json_data[T]
        self.n_CV = json_data[N_CV]
        self.n_MV = json_data[N_MV]

        if (json_data[CV][0].get(CONSTRAINT) is None): # Assume if this is correct for the first then the rest aswell
            self.plot_constraint = False
        else:
            self.plot_constraint = True
        
        self.P = len(json_data[CV][0][Y_PRED]) - self.T

        self.y = np.empty([self.n_CV, self.T + self.P])
        self.y_pred = np.empty([self.n_CV, self.T + self.P])
        self.u = np.ndarray([self.n_MV, self.T])

        self.ParseCVData(json_data)
        self.ParseMVData(json_data)

    def ReadSimulation(self, filepath):
        """
        Read simulation data json file
        :param filepath: relative path to simulation data
        :return: a json object holding the parsed simulation data
        """
        with open(filepath, "r") as file:
            return json.load(file)

    def ParseCVData(self, json_data):
        """
        Parsing the controlled variable simulation
        :param json_data: json object holding parsed simulation data
        """
        cv_data = json_data[CV]
        outputs, cv_units, cv_constraints = [], [], []
        for index, elem in enumerate(cv_data):
            outputs.append(elem[OUTPUT])
            cv_units.append(elem[UNIT])

            if self.plot_constraint:
                cv_constraints.append((elem[CONSTRAINT][0], elem[CONSTRAINT][1])) # Constraints are stored as python tuples

            # Fill nparray
            # self.y.T[..., index] = elem[Y] # Have not implemented reference model yet
            self.y_pred.T[..., index] = elem[Y_PRED]
        self.outputs, self.cv_units, self.cv_constraints = outputs, cv_units, cv_constraints

    def ParseMVData(self, json_data):
        """
        Parsing the manipulated variable simulation
        :param json_data: json object holding parsed simulation data
        """
        mv_data = json_data[MV]
        inputs, mv_units, mv_constraints = [], [], []
        for index, elem in enumerate(mv_data):
            inputs.append(elem[INPUT])
            mv_units.append(elem[UNIT])

            if self.plot_constraint:
                mv_constraints.append((elem[CONSTRAINT][0], elem[CONSTRAINT][1]))

            self.u.T[..., index] = elem[U]
        self.inputs, self.mv_units, self.mv_constraints = inputs, mv_units, mv_constraints


def ListSimulations(sim_dirpath):
    """
    Listing all avaliable simulations
    :param sim_dirpath: Local directory path
    """
    simulation_filepath = str(Path(__file__).parent.parent) + sim_dirpath
    files = os.listdir(simulation_filepath)

    for simulation in files:
        print(sim_dirpath + "/" + simulation)
