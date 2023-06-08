import numpy as np
import json
from pathlib import Path

# Simulation file specifiers
SCENARIO = "scenario"
T = "T"
N_CV = "n_CV"
N_MV = "n_MV"
P = "P"
M = "M"

CV = "CV"
MV = "MV"
OUTPUT = "output"
INPUT = "input"
UNIT = "unit"
Y = "y"
Y_PRED = "y_pred"
U = "u"


class OpenLoopData():
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
        self.P = json_data[P]
        self.M = json_data[M]

        self.y_pred = np.empty([self.n_CV, self.T])
        self.u = np.ndarray([self.n_MV, self.T])

        self.ParseCVData(json_data)
        self.ParseMVData(json_data)

    def ReadSimulation(self, filepath: str) -> json:
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
        outputs, cv_units = [], []
        for index, elem in enumerate(cv_data):
            outputs.append(elem[OUTPUT])
            cv_units.append(elem[UNIT])
            # Fill nparray
            # self.y.T[..., index] = elem[Y] # Have not implemented reference model yet
            self.y_pred.T[..., index] = elem[Y_PRED]
        self.outputs, self.cv_units = outputs, cv_units

    def ParseMVData(self, json_data):
        """
        Parsing the manipulated variable simulation
        :param json_data: json object holding parsed simulation data
        """
        mv_data = json_data[MV]
        inputs, mv_units = [], []
        for index, elem in enumerate(mv_data):
            inputs.append(elem[INPUT])
            mv_units.append(elem[UNIT])

            self.u.T[..., index] = elem[U]
        self.inputs, self.mv_units = inputs, mv_units
