# Travelling Salesman Problem using Genetic Algorithm
- Beginning population: random
- Selection method: rank selection - sort from smallest to largest objective function and then kill 75% of the worst individuals (elitism)
- Crossover method: OX, two random parents from old population
- Mutation methods: inversion and transposition mutation
- If there isn't crossover - then child is the same as parent
- Always 2 children are being made
- Succession: fill the old population with children by the moment it has the size of initial population
