
from plot_func import PlotPrediction, PlotInput
from SimulationData import *
import argparse
from matplotlib import pyplot as plt

parser = argparse.ArgumentParser(description="MPC plot")
parser.add_argument("-s", "--simulation", type=str, help="Simulation file", dest="simulation")

args = parser.parse_args()

simulation_name = "/data/simulations/sim_" + args.simulation + ".json"
Data = SimulationData(simulation_name)

fontsize = 10
figsize = 12
plt.rcParams.update({'font.size': fontsize})

PlotPrediction(Data, "Controlled Variables", figsize)
PlotInput(Data, "Manipulated Variables", figsize)
input("")