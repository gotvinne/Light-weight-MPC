from matplotlib import pyplot as plt
import argparse
from SimulationData import *

def PlotPrediction(sim_data, title, FIG_SIZE = 14):
    """
    Plotting the n_CV different predicted outputs along with the reference model output
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    t = np.arange(0, sim_data.T, dtype=int)
    fig = plt.figure(num = 1, figsize=(FIG_SIZE, FIG_SIZE)) 
    for i in range(sim_data.n_CV):
        fig.add_subplot(1, sim_data.n_CV, i + 1)
        # plt.plot(t, sim_data.y[i, :], "b", label="System output")
        plt.plot(t, sim_data.y_pred[i, :], "m", label="Predicted output") # m is magneta color
        
        # Constraints 
        upper = sim_data.cv_constraints[i][1] * np.ones(sim_data.T)
        lower = sim_data.cv_constraints[i][0] * np.ones(sim_data.T)
        plt.plot(t, upper, "k", linestyle="--", label="Upper constraint")
        plt.plot(t, lower, "k", linestyle="--", label="Lower constraint")

        plt.xlabel("MPC horizon, t")
        plt.ylabel(sim_data.cv_units[i])
        plt.legend(loc='upper right')

        plt.title(sim_data.outputs[i])
        plt.grid()
    plt.suptitle(title)
    plt.show(block=False) # Avoid blocking 


def PlotInput(sim_data, title, FIG_SIZE = 14):
    """
    Plotting the n_MV different optimized inputs
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    t = np.arange(0, sim_data.T, dtype=int)
    fig = plt.figure(num = 2, figsize=(FIG_SIZE, FIG_SIZE)) 
    for i in range(sim_data.n_MV):
        fig.add_subplot(1, sim_data.n_MV, i + 1)
        plt.step(t, sim_data.u[i, :], "b", label="Optimized actuation")

        upper = sim_data.mv_constraints[i][1] * np.ones(sim_data.T)
        lower = sim_data.mv_constraints[i][0] * np.ones(sim_data.T)
        plt.plot(t, upper, "k", linestyle="--", label="Upper constraint")
        plt.plot(t, lower, "k", linestyle="--", label="Lower constraint")

        plt.xlabel("MPC horizon, t")
        plt.ylabel(sim_data.mv_units[i])
        plt.legend(loc='upper right')

        plt.title(sim_data.inputs[i])
        plt.grid()
    plt.suptitle(title)
    plt.show(block=False) # Avoid blocking

parser = argparse.ArgumentParser(description="MPC plot")
parser.add_argument("-s", "--scenario", type=str, help="Scenario file", dest="scenario")

args = parser.parse_args()

simulation_name = "/data/simulations/sim_" + args.scenario + ".json"
Data = SimulationData(simulation_name)

fontsize = 10
figsize = 12
plt.rcParams.update({'font.size': fontsize})

PlotPrediction(Data, "Controlled Variables", figsize)
PlotInput(Data, "Manipulated Variables", figsize)
input("")