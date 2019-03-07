# Topological Oscillator

This program simulates the topological oscillator model. This is a simple one-dimensional model that has non-trivial characteristics, such as a topological charge. A topological charge is also present in lattice QCD models, therefore this model can be used to test ideas for new methods that should later be applied to four-dimensional lattice QCD.
The topological osciallator describes a particle with some mass M moving on a cirle with radius R (moment of inertia of particle I=M*R^2) in time. Time is discretized in d time slices with distance a (periodic boundaries are used) and the position of the particle at time slice i is defined by an angle phi \in [0, 2pi). The action of the model is given by

![equation](https://latex.codecogs.com/gif.latex?S%5B%5Cvarphi%5D%20%3D%20%5Cfrac%7BI%7D%7Ba%7D%20%5Csum_%7Bi%3D1%7D%5Ed%20%281%20-%20%5Ccos%28%5Cvarphi_%7Bi&plus;1%7D%20-%20%5Cvarphi_i%29%29)

The simulation computes the expectation value of an observable in the model, here for example the topological charge,

![equation](https://latex.codecogs.com/gif.latex?Q%5B%5Cvarphi%5D%20%3D%20%5Cfrac%7B1%7D%7B2%5Cpi%7D%20%5Csum_%7Bi%3D1%7D%5Ed%20%28%5Cvarphi_%7Bi&plus;1%7D%20-%20%5Cvarphi_i%29%20%5Cmod%202%5Cpi)

Computing the expectation value is done by integrating the weighted observable over all possible model-configurations,

![equation](https://latex.codecogs.com/gif.latex?%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_1%20...%20%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_d%20%5C%2C%20Q%5B%5Cvarphi%5D%20p%5B%5Cvarphi%5D), with

![equation](https://latex.codecogs.com/gif.latex?p%5B%5Cvarphi%5D%20%3D%20%5Cfrac%7B%5Cmathrm%7Be%7D%5E%7B-S%5B%5Cvarphi%5D%7D%7D%7B%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_1%20...%20%5Cint%20%5Cmathrm%7Bd%7D%5Cvarphi_d%20%5C%2C%20%5Cmathrm%7Be%7D%5E%7B-S%5B%5Cvarphi%5D%7D%7D)

Running the simulation is a two-step process:


## Creating the sampling points (*configurations*) for the integration
This is done using a Markov-chain Monte Carlo (MCMC) method. This results in N_{steps} different sampling points [phi]_j, j in {1,...,N_{steps}} that are distributed according to the distribution p[phi].

You can choose here between using the well known Metropolis algorithm or the Cluster algorithm, which works very efficient (especially for small a) for this model, see <https://journals.aps.org/prl/abstract/10.1103/PhysRevLett.62.361>. The MCMC methods need some thermalization phase before giving some reasonable values.

```cpp
./createConfig input/createConfig.in
```
(You can define the model parameters, MCMC parameters and the algorithm in the file input/createConfig.in)

## Evaluate the observables on the computed configurations
The observable is computed for all configurations, Q[phi]_j. Because the configurations are distributed according to p[phi], the expectation value is given by the average of all computed observables

![equation](https://latex.codecogs.com/gif.latex?%3CQ%3E%20%5Capprox%20%5Cfrac%7B1%7D%7BN_%7Bsteps%7D%7D%20%5Csum_%7Bi%3Dj%7D%5E%7BN_%7Bsteps%7D%7D%20Q%5B%5Cvarphi%5D_j).

```cpp
./computeCharge_MC input/computeCharge.in
```
(The model and MCMC parameters specified in this file have to be the ones of the computed configurations.)


## Comments
I also wrote code for methods to test alternative integration methods, but haven't found time until now to refactor and put it here.