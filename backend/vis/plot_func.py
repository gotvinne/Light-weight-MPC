from matplotlib import pyplot as plt
from math import ceil
from SimulationData import *

# Colors:
PURPLE = "m"
BLUE = "b"
RED = "r"
BLACK = "k"

def PlotMPC(sim_data: SimulationData, title: str, FIG_SIZE: int = 14) -> plt.figure:
    """
    Plot data in MPC-simulator format, CV plots are divided from MV.
    :param sim_data: SimulationData
    :param title: Figure title
    :param FIG_SIZE: figure size
    :return: figure reference
    """
    if (sim_data.n_CV == 1):
        cv_rows = 1
    else:
        cv_rows = ceil(sim_data.n_CV / 2) 
    
    if (sim_data.n_MV == 1):
        mv_rows = 1
    else:
        mv_rows = ceil(sim_data.n_MV / 2) 

    if (sim_data.n_CV == 1 and sim_data.n_MV == 1):
        cols = 1
    else:
        cols = 2

    fig, axs = plt.subplots(cv_rows + mv_rows, cols, figsize=(FIG_SIZE, FIG_SIZE)) 
    fig.suptitle(title)
    PlotPrediction(axs, sim_data, cv_rows, toggling=(cols == 2))
    PlotActuation(axs, sim_data, cv_rows, mv_rows, toggling=(cols == 2))
    plt.show(block=False) # Avoid blocking 

    return fig

def PlotPrediction(axs: plt.axis, sim_data: SimulationData, cv_rows: int, toggling: bool):
    """
    Plotting the n_CV different predicted outputs along with the reference model output
    :param axs: plt.axs reference
    :param sim_data: SimulationData object
    :param cv_rows: Number of CV rows to be plotted
    :param toggling: Indicator is SISO system is plotted
    """
    plot_horizon = sim_data.T + sim_data.P # Also plotting predictions
    t = np.arange(0, plot_horizon, dtype=int) 

    if toggling:
        index = 0
        for i in range(cv_rows):
            for toggle in range(2):
                axs[i, toggle].axvline(x = sim_data.T, color = BLACK, label = "Prediction axis")
                # plt.plot(t, sim_data.y[i, :], "b", label="System output")
                axs[i, toggle].plot(t[0:sim_data.T], sim_data.y_pred[index, 0:sim_data.T], PURPLE, label="Output") 
                axs[i, toggle].plot(t[sim_data.T:], sim_data.y_pred[index, sim_data.T:], PURPLE, linestyle="--", label="Predicted output") 
                axs[i, toggle].plot(t, sim_data.ref[index, :], RED, label="Reference") 
                
                # Constraints 
                upper = sim_data.cv_constraints[index][1] * np.ones(plot_horizon)
                lower = sim_data.cv_constraints[index][0] * np.ones(plot_horizon)
                axs[i, toggle].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
                axs[i, toggle].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")

                axs[i, toggle].set_xlabel("MPC horizon, T")
                axs[i, toggle].set_ylabel(sim_data.cv_units[index])
                axs[i, toggle].legend()
                axs[i, toggle].grid()
                axs[i, toggle].set_title(sim_data.outputs[index])
                index += 1
    else: 
        axs[0].axvline(x = sim_data.T, color = BLACK, label = "Prediction axis")
        # axs[0].plot(t, sim_data.y[i, :], "b", label="System output")
        axs[0].plot(t[0:sim_data.T], sim_data.y_pred[0, 0:sim_data.T], PURPLE, label="Output") 
        axs[0].plot(t[sim_data.T:], sim_data.y_pred[0, sim_data.T:], PURPLE, linestyle="--", label="Predicted output") 
        axs[0].plot(t, sim_data.ref[0, :], RED, label="Reference") 
        
        # Constraints 
        upper = sim_data.cv_constraints[0][1] * np.ones(plot_horizon)
        lower = sim_data.cv_constraints[0][0] * np.ones(plot_horizon)
        axs[0].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
        axs[0].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")

        axs[0].set_xlabel("MPC horizon, T")
        axs[0].set_ylabel(sim_data.cv_units[0])
        axs[0].legend()
        axs[0].grid()
        axs[0].set_title(sim_data.outputs[0])


def PlotActuation(axs: plt.axis, sim_data: SimulationData, cv_rows: int, mv_rows: int, toggling: bool):
    """
    Plotting the n_MV different optimized inputs
    :param axs: plt.axis reference object
    :param sim_data: SimulationData object holding the simulation information
    :param cv_rows: Number of CV rows to be plotted
    :param mv_rows: Number of MV rows to be plotted
    :param toggling: Indicator is SISO system is plotted
    """
    plot_horizon = sim_data.T + sim_data.M  # Also plotting predictions
    t = np.arange(0, plot_horizon, dtype=int)
    if toggling:
        index = 0
        for i in range(cv_rows, cv_rows + mv_rows):
            for toggle in range(2):
                axs[i, toggle].axvline(x=sim_data.T, color=BLACK, label="Prediction axis")
                axs[i, toggle].step(t[0:sim_data.T], sim_data.u[index, 0:sim_data.T], BLUE, label="Optimized actuation")
                axs[i, toggle].step(t[sim_data.T:], sim_data.u[index, sim_data.T:], BLUE, linestyle="--",
                                    label="Predicted actuation")

                upper = sim_data.mv_constraints[index][1] * np.ones(plot_horizon)
                lower = sim_data.mv_constraints[index][0] * np.ones(plot_horizon)
                axs[i, toggle].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
                axs[i, toggle].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")
                axs[i, toggle].set_xlabel("MPC horizon, T")
                axs[i, toggle].set_ylabel(sim_data.mv_units[index])   
                axs[i, toggle].legend()
                axs[i, toggle].grid()
                axs[i, toggle].set_title(sim_data.inputs[index])
                index += 1
    else:
        axs[1].axvline(x=sim_data.T, color=BLACK, label="Prediction axis")
        axs[1].step(t[0:sim_data.T], sim_data.u[0, 0:sim_data.T], BLUE, label="Optimized actuation")
        axs[1].step(t[sim_data.T:], sim_data.u[0, sim_data.T:], BLUE, label="Predicted actuation")

        # Constraints 
        upper = sim_data.mv_constraints[0][1] * np.ones(plot_horizon)
        lower = sim_data.mv_constraints[0][0] * np.ones(plot_horizon)
        axs[1].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
        axs[1].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")
        axs[1].set_xlabel("MPC horizon, T")
        axs[1].set_ylabel(sim_data.mv_units[0])   
        axs[1].legend()
        axs[1].grid()
        axs[1].set_title(sim_data.inputs[0])

