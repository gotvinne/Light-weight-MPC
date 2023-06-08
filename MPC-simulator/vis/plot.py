from plot_func import PlotMPC
from SimulationData import *
from OpenLoopData import *
import argparse
from matplotlib import pyplot as plt

########################################################
# Script plotting simulation data via terminal
########################################################
parser = argparse.ArgumentParser(description="MPC plot")
parser.add_argument("-s", "--simulation", type=str, help="Simulation file", dest="simulation")

args = parser.parse_args()
open_loop = False
if (args.simulation[0:13] == "sim_open_loop"):
    open_loop = True

simulation_name = "/data/simulations/" + args.simulation + ".json"
if open_loop:
    sim_data = OpenLoopData(simulation_name)
    title = "Open loop simulation, CV (purple) MV (blue), " + args.simulation
else:
    sim_data = SimulationData(simulation_name)
    title = "MPC simulation data, CV (purple) MV (blue), " + args.simulation

fontsize = 12
figsize = 12
plt.rcParams.update({'font.size': fontsize})


PlotMPC(sim_data, title, figsize)
input("")