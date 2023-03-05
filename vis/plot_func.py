from matplotlib import pyplot as plt
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
        if sim_data.plot_constraint:
            upper = sim_data.cv_constraints[i][1] * np.ones(sim_data.T)
            lower = sim_data.cv_constraints[i][0] * np.ones(sim_data.T)
            plt.plot(t, upper, "k", linestyle="--", label="Upper constraint")
            plt.plot(t, lower, "k", linestyle="--", label="Lower constraint")

        plt.xlabel("MPC horizon, t")
        plt.ylabel(sim_data.cv_units[i])
        plt.legend(loc='upper right')

        plt.title(sim_data.outputs[i]+ ": " + str(sim_data.y_pred[i, sim_data.T-1]))
        plt.grid()
    plt.suptitle(title)
    plt.show(block=False) # Avoid blocking 

    return fig


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

        if sim_data.plot_constraint:
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

    return fig

