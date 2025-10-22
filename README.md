Of course. Here is a comprehensive `README.md` file for the `lem-in` project, based on the provided code and project description. This README explains the project's purpose, features, usage instructions, and the underlying algorithms.

---

# Lem-in | 42 Project

![42 Project](https://img.shields.io/badge/42-Project-00babc.svg)
![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Library](https://img.shields.io/badge/Library-Raylib-D53B83.svg)

**Lem-in** is an algorithmic project from the 42 school curriculum. The goal is to create an "ant farm manager" that finds the most efficient way to move a given number of ants from a starting room to an ending room through a network of tunnels. This project is a fascinating dive into graph theory, focusing on pathfinding and max-flow algorithms.

This implementation not only solves the problem efficiently but also includes a real-time graphical visualizer to watch the ants make their journey!

---

## Visualizer Demo

The visualizer launches automatically after the solution is printed to the terminal. It provides a graphical representation of the rooms, tunnels, and the animated movement of each ant along its assigned path.

_(This is a static representation of the visualizer in action.)_

## Features

- **Robust Map Parsing**: Reads and validates complex ant farm descriptions from standard input, correctly handling rooms, links, comments, and special commands (`##start`, `##end`).
- **Advanced Pathfinding**: Implements a max-flow algorithm (based on Edmonds-Karp) to find the maximum number of vertex-disjoint paths from the start to the end room.
- **Optimal Ant Dispatching**: A smart, greedy algorithm distributes ants across the discovered paths to minimize the total number of turns required for all ants to complete the journey.
- **Comprehensive Error Handling**: Thoroughly validates maps and exits gracefully with a clear `ERROR` message for invalid inputs like duplicate rooms, missing start/end points, or unsolvable maps.
- **Interactive Visualizer (Bonus)**: A real-time visualizer built with the **Raylib** library brings the simulation to life, showing the ant farm layout and the ants' movements.

## 🔬 Algorithm Explained

The solution is broken down into three main steps:

### 1. Parsing & Graph Construction

The program reads the map line by line and constructs a directed graph using an adjacency list.

- The number of ants is parsed first.
- Rooms are stored in an array, and a hash map (or linear search in this case) links room names to their IDs.
- Links (`roomA-roomB`) are added as edges in the graph.
- The `##start` and `##end` commands are processed to identify the source and sink nodes.

### 2. Finding Disjoint Paths (Max-Flow)

The core of the problem is finding the optimal set of paths. This is solved by treating the ant farm as a flow network and finding the maximum flow.
We use a variant of the **Edmonds-Karp algorithm**:

1.  Repeatedly run **Breadth-First Search (BFS)** to find the shortest available path from `##start` to `##end`.
2.  Each time a path is found, its intermediate nodes are marked as "used" for subsequent searches. This ensures that the next path found will be vertex-disjoint (it won't share any rooms, except for start and end).
3.  This process continues until no more paths can be found between the start and end rooms.

### 3. Ant Distribution

With a set of disjoint paths of varying lengths, we must distribute the ants efficiently.

- The time (number of turns) it takes for `k` ants to travel a path of length `L` is `L + k - 1`. The total time for the simulation is determined by the path that finishes last.
- To minimize this, a **greedy algorithm** is used: for each ant, it is assigned to the path that would result in the lowest overall turn count if that ant were added to it. This effectively balances the load across all available paths.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc` or `clang`)
- `make`
- On Linux, you will need the development libraries for Raylib's dependencies. You can install them with:
  ```bash
  # For Debian/Ubuntu-based systems
  sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
  ```

### Installation & Usage

1.  **Clone the repository** (make sure to include submodules for Raylib):

    ```bash
    git clone --recurse-submodules https://github.com/your-username/lem-in.git
    cd lem-in
    ```

2.  **Compile the project**:

    ```bash
    make
    ```

3.  **Run the simulation**:
    Pipe a map file into the executable. Several example maps are provided in the `/maps` directory.
    ```bash
    ./lem-in < maps/test.map
    ```

### Example

**Input (`maps/test.map`)**

```
3
# Ceci est un commentaire sur le nombre de fourmis
##start
start_room 1 1
roomA 5 5
# Un autre commentaire
roomB 1 5
##end
end_room 5 1
start_room-roomA
start_room-roomB
roomA-end_room
roomB-end_room
roomA-roomB
```

**Output**

```
3
##start
start_room 0 0
roomA 1 1
roomB 2 2
##end
end_room 3 3
start_room-roomA
start_room-roomB
roomA-end_room
roomB-end_room

L1-roomB L2-roomA
L1-end_room L2-end_room L3-roomB
L3-end_room
```

After this text output, the graphical visualizer window will open.
