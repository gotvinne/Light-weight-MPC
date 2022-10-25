
import json
import os

SIMULATIONS_FILEPATH = "/data/simulations"

def ReadSimulation(filepath):
    with open(filepath,"r") as file: 
        return json.load(file)

def ListSimulations():
    simulation_filepath = os.path.abspath(os.curdir) + SIMULATIONS_FILEPATH
    files = os.listdir(simulation_filepath)

    for simulation in files:
        print(simulation)

