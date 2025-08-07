# Color Linez Game (C++ Implementation)

This project is a full-featured reimplementation of the classic Color Linez game in C++. It was developed as a capstone assignment for the *Advanced Language Programming Laboratory* course, and demonstrates comprehensive competencies in custom algorithm development, object-oriented design, randomized simulation, and pseudo-graphical interface programming—all implemented without using STL, global variables, or external libraries.

---

## Game Description

- **Board**: N × N grid (user-defined, typically 7–9)
- **Ball Colors**: 7 unique colors
- **Game Rules**:
  - Game starts with 5 randomly placed balls
  - 3 new balls are generated after each non-scoring move
  - Balls can be moved to empty positions if a valid path exists
  - Eliminate 5 or more same-color balls in a line to earn points
  - Score formula: `(n - 1) * (n - 2)`
  - Game ends if no moves or no balls remain

---

## Key Features & Technical Highlights

### Algorithms

- **A\* Pathfinding Algorithm**  
  Efficiently finds the shortest path between source and destination using a priority queue sorted by F = G + H (Manhattan distance). A custom `Astar` class encapsulates all logic.

- **Recursive Elimination Detection**  
  Detects consecutive balls in 8 directions using recursive adjacency checks. Supports overlapping and intersecting lines.

- **Animation System**  
  Movement path is animated step-by-step in the pseudo-graphical UI using recursion and frame rendering.

---

### Object-Oriented & Modular Design

- `Game` class: stores state, handles logic, rules, and interface
- `Astar` class: encapsulates all pathfinding logic
- `Node` and `node_log`: track position states and cost functions
- Code split across 5 modular `.cpp` files and a shared `.h` file

---

### Console-Based UI with Mouse Input

- Built entirely in command-line (cmd) using `cmd_console_tools`
- Pseudo-graphical grid with frame lines and colored balls
- Real-time mouse interaction:
  - Left-click to select/move balls
  - Right-click to exit
- Animated feedback: movement, elimination, game over

---

## Statistical Programming and Computing Aspects

This project demonstrates several principles aligned with **statistical programming and computing**, including:

- **Randomized Simulation**
  - Ball generation uses uniform pseudo-random numbers within dynamically tracked valid positions (pre-indexed array stack).

- **Stochastic State Simulation**
  - The game evolves through a sequence of randomized events followed by rule-based transitions—akin to a discrete event simulation model.

- **Algorithmic Decision Systems**
  - Custom A* pathfinding simulates optimization over a dynamically changing state space.

- **Matrix-Based Data Representation**
  - The game board is a 2D array, continuously updated and traversed for rule enforcement and display.

- **Statistical Data Visualization**
  - Real-time display of score, upcoming balls, and per-color elimination statistics mirrors typical analysis dashboards.

---

## 📚 Reproducibility and Software Engineering Practices

- **No global variables**, **no STL**, and **no unsafe I/O**
- All logic is modularized and encapsulated using classes
- Each public function and class member is documented
- Debugging was supported through:
  - Manual validation of edge cases (e.g., full board, blocked moves)
  - Step-by-step printed logs for pathfinding and animation
- Fully version-controlled using Git, with staged modular commits

---

## 💡 Relevance to Statistical Programming Curriculum

This project strongly aligns with the objectives of a statistical programming curriculum:

- Statistical simulation and stochastic processes (random ball generation)
- Custom computational logic and model-based rule enforcement
- Object-oriented design and code modularity
- Reproducible results and debugging support
- Matrix-based state modeling and real-time data presentation
- Use of version control (Git) and code documentation

While the game does not implement applied statistical models (e.g., regressions or hypothesis testing), it showcases simulation-based system design and computational problem-solving—both critical skills in modern statistical computing.

## Project Structure
| File                | Description                                  |
| ------------------- | -------------------------------------------- |
| `90-b2-main.cpp`    | Main function and menu selector              |
| `90-b2-base.cpp`    | Internal array logic and gameplay            |
| `90-b2-console.cpp` | Pseudo-graphical cmd interface logic         |
| `90-b2-tools.cpp`   | Shared utilities, A\* algorithm, board logic |
| `90-b2.h`           | Common declarations and class definitions    |
