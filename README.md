# Color Linez Game (C++ Implementation)

This project is a full-featured implementation of the Windows-style Color Linez game, completed as part of the *Advanced Language Programming Laboratory* course. The implementation was written entirely in C++ with strict constraints (e.g., no STL, no global variables, and no external libraries), and supports both array-based logic and a pseudo-graphical console interface using mouse input.

## Game Description

- **Board**: N × N grid (user-defined, typically 7–9)
- **Ball Colors**: 7 unique colors
- **Rules**:
  - 5 balls generated initially, 3 new balls generated after each non-scoring move
  - Move a ball by selecting a source and target cell (if reachable by a valid path)
  - Aligning 5+ same-color balls in a row/column/diagonal triggers elimination
  - Scoring: `(n - 1) * (n - 2)` for eliminating `n` balls
  - If board is full or empty, the game ends

## Features & Technical Highlights

### Algorithms
- **A\* Pathfinding Algorithm**  
  Implements an efficient A* search with a manually managed priority queue and Manhattan distance heuristic. This ensures smooth and intelligent path traversal for ball movements.

- **Ball Elimination Detection**  
  Recursively checks all 8 directions from the moved ball to detect consecutive segments of the same color. Supports overlapping and crossing lines with duplicate scoring.

- **Recursive Animation Rendering**  
  For pseudo-GUI version, uses recursive tracing of path to render ball animation smoothly from source to destination.

### Data Structures
- **Game Class**  
  Central data manager that stores grid state, ball colors, score, available positions, upcoming balls, and elimination statistics.

- **Custom Stack for Empty Cell Tracking**  
  Optimized random placement by pre-tracking available cells with an array-based stack to avoid repeated re-generation.

- **Node and Log Structures**  
  Used in the pathfinding logic to record visited states, parent relationships, and costs (F = G + H).

### Visualization
- **Pseudo-Graphical Interface**  
  Built entirely in the console using custom rendering functions. Color rendering, grid frames (with and without separators), animation, and mouse support are implemented using only permitted tools.

- **Mouse Controls**
  - Left-click to select a ball and destination
  - Real-time coordinate display (e.g., “Row A, Column 5”)
  - Right-click to exit game

- **Real-Time Information Panel**
  - Score display
  - Upcoming ball preview
  - Elimination summary by color

## Statistical Programming and Computing Aspects

This game is not just a software engineering exercise — it also showcases **statistical programming and computing competencies**, specifically:

- **Randomized Simulation**  
  - Ball positions and colors are generated using uniform pseudo-random number generation.  
  - Position selection is done efficiently to simulate probabilistic spatial distribution.

- **Pathfinding as Dynamic Optimization**  
  - The A* algorithm embodies discrete optimization over a dynamically changing state space — similar to how statistical algorithms search for best-fit paths or models.

- **Data Representation and Transformation**  
  - The game board is treated as a 2D matrix (like in statistical computing platforms), and frequently updated to reflect state transitions, much like time-series or simulation data.

- **Score Analytics and Game Metrics**  
  - Score updates follow a nonlinear function of eliminated balls.  
  - Real-time statistics panel mirrors dashboard-style analytics (counts by color, preview of future state).

- **Event-Driven State Simulation**  
  - The game evolves as a series of random events + rule-based state transitions — a model often used in simulation-based statistical methods.
