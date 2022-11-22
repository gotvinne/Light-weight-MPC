from matplotlib import pyplot as plt

from SimulationData import *


def PlotPrediction(sim_data, title):
    """
    Plotting the n_CV different predicted outputs along with the reference model output
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    t = np.arange(0, sim_data.T, dtype=int)
    for i in range(sim_data.n_CV):
        plt.subplot(1, sim_data.n_CV, i + 1)
        # plt.plot(t, sim_data.y[i, :], "b", label="System output")
        plt.plot(t, sim_data.y_pred[i, :], "m", label="Predicted output")

        plt.xlabel("MPC horizon, t")
        plt.ylabel(sim_data.cv_units[i])
        plt.legend()

        plt.title(sim_data.outputs[i])
        plt.grid()
    plt.suptitle(title)
    plt.show()


def PlotInput(sim_data, title):
    """
    Plotting the n_MV different optimized inputs
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    t = np.arange(0, sim_data.T, dtype=int)
    for i in range(sim_data.n_MV):
        plt.subplot(1, sim_data.n_MV, i + 1)
        plt.step(t, sim_data.u[i, :], "b", label="Optimized actuation")

        plt.xlabel("MPC horizon, t")
        plt.ylabel(sim_data.mv_units[i])
        plt.legend()

        plt.title(sim_data.inputs[i])
        plt.grid()
    plt.suptitle(title)
    plt.show()
