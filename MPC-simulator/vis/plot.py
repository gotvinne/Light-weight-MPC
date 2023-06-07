from plot_func import PlotMPC
from SimulationData import *
import argparse
from matplotlib import pyplot as plt

########################################################
# Script plotting simulation data via terminal
########################################################

parser = argparse.ArgumentParser(description="MPC plot")
parser.add_argument("-s", "--simulation", type=str, help="Simulation file", dest="simulation")

args = parser.parse_args()

simulation_name = "/data/simulations/" + args.simulation + ".json"
sim_data = SimulationData(simulation_name)

fontsize = 12
figsize = 12
plt.rcParams.update({'font.size': fontsize})
title = "MPC simulation data, CV (purple) MV (blue)"

PlotMPC(sim_data, title, figsize)
input("")