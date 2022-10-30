
import numpy as np
import json
import os

# Simulation file specifiers
SCENARIO = "scenario"
T = "T"
N_CV = "n_CV"
N_MV = "n_MV"

CV = "CV"
MV = "MV"
OUTPUT = "Output"
INPUT = "Input"
UNIT = "Unit"
CONSTRAINT = "c"
Y = "y"
Y_HAT = "y_hat"
U = "u"

class SimulationData():
    def __init__(self, filepath):

        json_data = self.ReadSimulation(filepath)

        self.scenario = json_data[SCENARIO]
        self.T = json_data[T]
        self.n_CV = json_data[N_CV]
        self.n_MV = json_data[N_MV]

        self.y = np.empty([self.n_CV, self.T])
        self.y_hat = np.empty([self.n_CV, self.T])
        self.u = np.ndarray([self.n_MV, self.T])

        self.ParseCVData(json_data)
        self.ParseMVData(json_data)

    def ReadSimulation(self, filepath):
        """

        :param filepath:
        :return:
        """
        with open(filepath, "r") as file:
            return json.load(file)

    def ParseCVData(self, data):
        cv_data = data[CV]

        self.outputs = []
        self.cv_units = []
        self.cv_constraints = []
        for index, elem in enumerate(cv_data):
            self.outputs.append(elem[OUTPUT])
            self.cv_units.append(elem[UNIT])
            self.cv_constraints.append((elem[CONSTRAINT][0], elem[CONSTRAINT][1]))

            # Fill nparray
            self.y.T[..., index] = elem[Y]
            self.y_hat.T[..., index] = elem[Y_HAT]

    def ParseMVData(self, json_data):
        mv_data = json_data[MV]

        self.inputs = []
        self.mv_units = []
        self.mv_constraints = []

        for index, elem in enumerate(mv_data):
            self.inputs.append(elem[OUTPUT])
            self.mv_units.append(elem[UNIT])
            self.mv_constraints.append((elem[CONSTRAINT][0], elem[CONSTRAINT][1]))

            self.u.T[..., index] = elem[U]

def ListSimulations(sim_dirpath):
    """

    :return:
    """
    simulation_filepath = os.path.abspath(os.curdir) + sim_dirpath
    files = os.listdir(simulation_filepath)

    for simulation in files:
        print(simulation)