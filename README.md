AI-Assessment-Fungeon
=====================

 My custom implementation of a behaviour tree, hierarchical task network and blackboard system for an AI Assessment. In order to unlock a series of doors with appropriate keys, the planner posts tasks to a blackboard, and available agents subscribe to the tasks. An agent is then chosen to carry out the task (collect a key, open a door etc). The user can interact by clicking on the floor tiles to "electrify" them; the agents will try to avoid them, but will die and lose all their collected keys if they walk onto them.

An expanded explanation of the rules and logic of the agents is explained in Fungeon.pdf.

Note - Uses the AIE Framework, available at https://github.com/AcademyOfInteractiveEntertainment/AIEFramework
