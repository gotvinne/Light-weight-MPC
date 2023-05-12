from matplotlib import pyplot as plt
from math import ceil
from SimulationData import *

# Colors:
PURPLE = "m"
BLUE = "b"
RED = "r"
BLACK = "k"

def PlotMPC(sim_data, title, FIG_SIZE=14):
    if (sim_data.n_CV == 1):
        cv_rows = 1
    else:
        cv_rows = ceil(sim_data.n_CV / 2) 
    
    if (sim_data.n_MV == 1):
        mv_rows = 1
    else:
        mv_rows = ceil(sim_data.n_MV / 2) 

    if (mv_rows == 1 and cv_rows == 1):
        cols = 1
    else:
        cols = 2

    fig, axs = plt.subplots(cv_rows + mv_rows, cols, figsize=(FIG_SIZE, FIG_SIZE)) 
    fig.suptitle(title)
    PlotPrediction(axs, sim_data, toggling=(cols == 2))
    PlotActuation(axs, sim_data, toggling=(cols == 2))
    plt.show(block=False) # Avoid blocking 

    return fig

def PlotPrediction(axs, sim_data, toggling):
    """
    Plotting the n_CV different predicted outputs along with the reference model output
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    plot_horizon = sim_data.T + sim_data.P # Also plotting predictions
    t = np.arange(0, plot_horizon, dtype=int) 

    if toggling:
        toggle = 0
        for i in range(sim_data.n_CV):

            axs[i, toggle].axvline(x = sim_data.T, color = BLACK, label = "Prediction axis")
            # plt.plot(t, sim_data.y[i, :], "b", label="System output")
            axs[i, toggle].plot(t[0:sim_data.T], sim_data.y_pred[i, 0:sim_data.T], PURPLE, label="Output") 
            axs[i, toggle].plot(t[sim_data.T:], sim_data.y_pred[0, sim_data.T:], PURPLE, linestyle="--", label="Predicted output") 
            axs[i, toggle].plot(t, sim_data.ref[i, :], RED, label="Reference") 
            
            # Constraints 
            upper = sim_data.cv_constraints[i][1] * np.ones(plot_horizon)
            lower = sim_data.cv_constraints[i][0] * np.ones(plot_horizon)
            axs[i, toggle].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
            axs[i, toggle].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")

            axs[i, toggle].set_xlabel("MPC horizon, t")
            axs[i, toggle].set_ylabel(sim_data.cv_units[i])
            axs[i, toggle].legend()
            axs[i, toggle].grid()
            axs[i, toggle].set_title(sim_data.outputs[i])

            toggle = 1 - toggle
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

        axs[0].set_xlabel("MPC horizon, t")
        axs[0].set_ylabel(sim_data.cv_units[0])
        axs[0].legend()
        axs[0].grid()
        axs[0].set_title(sim_data.outputs[0])


def PlotActuation(axs, sim_data, toggling):
    """
    Plotting the n_MV different optimized inputs
    :param sim_data: SimulationData object holding the simulation information
    :param title: The main title of the plots
    """
    t = np.arange(0, sim_data.T, dtype=int)
    if toggling:
        toggle = 0
        for i in range(sim_data.n_MV):
            axs[i, toggle].step(t, sim_data.u[i, :], BLUE, label="Optimized actuation")

            upper = sim_data.mv_constraints[i][1] * np.ones(sim_data.T)
            lower = sim_data.mv_constraints[i][0] * np.ones(sim_data.T)
            axs[i, toggle].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
            axs[i, toggle].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")
            axs[i, toggle].set_xlabel("MPC horizon, t")
            axs[i, toggle].set_ylabel(sim_data.mv_units[i])   
            axs[i, toggle].legend()
            axs[i, toggle].grid()
            axs[i, toggle].set_title(sim_data.inputs[i])

            toggle = 1 - toggle
    else: 
        axs[1].step(t, sim_data.u[0, :], BLUE, label="Optimized actuation") 
        
        # Constraints 
        upper = sim_data.mv_constraints[0][1] * np.ones(sim_data.T)
        lower = sim_data.mv_constraints[0][0] * np.ones(sim_data.T)
        axs[1].plot(t, upper, BLACK, linestyle="--", label="Upper constraint")
        axs[1].plot(t, lower, BLACK, linestyle="--", label="Lower constraint")
        axs[1].set_xlabel("MPC horizon, t")
        axs[1].set_ylabel(sim_data.mv_units[0])   
        axs[1].legend()
        axs[1].grid()
        axs[1].set_title(sim_data.inputs[0])

