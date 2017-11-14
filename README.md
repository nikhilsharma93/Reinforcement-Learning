# Reinforcement-Learning
**Reinforcement Learning for grid world using Value Iteration** 

This is a UI based implementation of reinforcement learning in a grid world (of user desired size). It works on the Value Iterations form of Bellman Equation. 
To see it working, run the executable **GridWorld**.

You can choose the number of blocked states, and the number of positive and negative terminal states. The rewards for non-terminal states, positive terminal states, and negative terminal states can be set at the beginning. The current implementation does not allow adding more than one type of blocked/non-terminal state, but the extension to that is trivial. 

The environment can be stochastic or completely deterministic. You can specify the following probabilities:

If you decide to go in a direction 'X', you would end up going:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1) In that direction with a probability <>, 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2) To the left of X with a probability <>, and

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3) To the right of X with a probability <>


The discount factor (gamma) can be set at the start. 
Sample screenshots can be found in the folder SampleOutput.

