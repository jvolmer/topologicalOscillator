# Topological Oscillator

This program simulates the topological oscillator model. This is a simple one-dimensional model that has non-trivial characteristics, such as a topological charge. A topological charge is also present in lattice QCD models, therefore this model can be used to test ideas for new methods that should later be applied to four-dimensional lattice QCD.
The topological osciallator describes a particle with some mass M moving on a cirle with radius R (moment of inertia of particle I=M*R^2) in time. Time is discretized in $d$ time slices with distance a (periodic boundaries are used). The action of the model is given by

$S[\phi] = \frac{I}{a} \sum_{i=1}^d (1 - \cos(\phi_{i+1} - \phi_i))$

The simulation computes the expectation value of an observable in the model, here for example the topological charge,

$Q[\phi] = \frac{1}{2\pi} \sum_{i=1}^d (\phi_{i+1} - \phi_i) \mod 2\pi$

Computing the expectation value is done by integrating the weighted observable over all possible model-configurations,
![equation](https://latex.codecogs.com/gif.latex?%3CQ%3E%20%3D%20%5Cfrac%7B%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_1%20...%20%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_d%20Q%5B%5Cphi%5D%20%5Cmathrm%7Be%7D%5E%7B-S%5B%5Cphi%5D%7D%7D%7B%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_1%20...%20%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_d%20%5Cmathrm%7Be%7D%5E%7B-S%5B%5Cphi%5D%7D%7D)

Running the simulation is a two-step process:
## Creating the sampling points (*configurations*) for the integration
This is done using a Markov-chain Monte Carlo (MCMC) method. This results in N_{step} different sampling points [\phi]_i that are distributed according to the distribution $\frac{\e{-S[\phi]}}{\int d_1 ... \int d\phi_d \e{-S[\phi]}}$.

You can choose here between using the well known Metropolis algorithm or the Cluster algorithm, which works very well for this model. The MCMC methods need some thermalization phase before giving some reasonable values.

./createConfig input/createConfig.in
(You can define the model parameters, MCMC parameters and the algorithm in the file input/createConfig.in)

## Evaluate the observables on the computed configurations
The observable is computed for all configurations, $Q[\phi]_i, i \in \{1, ... , N_{step}\}$. Because the configurations are distributed according to $\frac{\e{-S[\phi]}}{\int d_1 ...}$, the expectation value is given by the average of all computed observables
$<Q> \approx \frac{1}{N_{steps}} \sum_{i=1}^{N_{steps}} Q[\phi]_i.

./computeCharge_MC input/computeCharge.in
(The model and MCMC parameters specified in this file have to be the ones of the computed configurations.)
