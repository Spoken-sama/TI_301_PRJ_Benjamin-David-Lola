# TI_301_PRJ_Benjamin-David-Lola
GRP 1: Benjamin Durame, David Dehondt, Lola Lievain
# Markov Graph Analyzer (T1301 Project)
## what is this project ?
This C language project implements data structures and algorithms for the comprehensive study and analysis of Markov Graphs. The application is designed to **analyze graph properties**, **identify structural components**, and **perform the matrix algebra necessary for probabilistic and long-term behavior analysis**. To exploit the matrix, a menu is available, guiding the user through the process of loading a graph and performing the various analyses defined in the project specifications. This project required us to use your knowledge seen in last year's Linear Algebra course.

## Key Features & Functionalities
The interface coverq three main areas of Markov graph analysis :
### 1. Graph Structure & SCC Identification
- Adjacency List Loading : Loads the graph structure and transition probabilities from a file, storing them in an ```Adjacency List (t_adjacency_list)```.
- Tarjan's Algorithm : Implements Tarjan's algorithm to efficiently find the Strongly Connected Components of the graph. These SCCs are stored as a ```t_partition``` of Classes.
- Hasse Diagram Construction: The ```removeTransitiveLinks``` function is implemented to simplify the reduced graph of classes by eliminating indirect paths, a critical step toward constructing the final Hasse Diagram.

### 2. Matrix Representation & Algebra
- Transition Matrix Creation : Converts the Adjacency List into the Transition Probability Matrix (```t_matrix```).
- Basic Matrix Operations : Creation and copying of square matrices.
- Matrix Multiplication : Computes matrix products, including matrix exponentiation M^n (```multiply_n_times```) to find $n$-step transition probabilities.
- Difference Matrix : Calculates the difference between two matrices (diff), typically used to check for convergence or stability in the Markov chain.
- Submatrix Extraction : Isolates the transition matrix for a specific class (subMatrix), essential for analyzing properties like periodicity.

### 3. Data Structures Used
- The core logic relies on several custom C structures defined in types.h :
- ```t_adjacency_list```: Graph representation.
- ```t_tarjan_vertex``` : Stores state for Tarjan's algorithm (index, low-link, on-stack status).
- ```t_partition``` : Collection of strongly connected components (classes).
- ```t_matrix```: Defined as ```float **```, used for the Transition Matrix and its derivatives.

## Project File Structure

File Description

- ```main.c``` Contains the main function and the interactive menu loop. Manages user flow and memory cleanup.

- ```matrix.c/matrix.h``` Core implementation for all matrix operations: creation, printing, multiplication (multiply_matrices, multiply_n_times), difference (diff), and submatrix extraction (subMatrix).

- ```utils.c/utils.h``` Implements graph data structure utilities (list management, file loading, vertex ID mapping), Tarjan's algorithm (tarjan, parcours), and partition management.

- ```hasse.c/hasse.h``` Implements functions for the class graph, notably removeTransitiveLinks for Hasse Diagram construction.types.hContains all project-specific structure definitions (```t_adjacency_list, t_partition, t_matrix, etc.```).

- ```CMakeLists.txt``` Build configuration file using CMake.

### Building and Running
#### Prerequisites
- A C compiler (e.g., GCC).
- The CMake build system.

#### Build Instructions
Use the provided ```CMakeLists.txt``` to compile the project:
- ```Bash# 1. ``` Create and enter a build directory
- ```mkdir``` build
- ```cd``` build

2. Configure the project :
   ```cmake ..```

3. Build the executable:
   ```make```

#### Execution
The command above will generate the executable, likely named TI_301_PJT.Run the application:Bash./TI_301_PJT
The program will display a menu. Select option 1 to load a graph file (expected in the ../data directory) and follow the menu to execute the analysis steps.

## Conclusion
This project represents the successful implementation of a multi-part challenge to analyze Discrete-Time Markov Chains using the C programming language.We effectively utilized dynamic memory allocation and complex data structures, such as the Adjacency List and the Transition Matrix, to accurately model the graph's structure and probabilities. Core analytical requirements were demonstrably met through the successful deployment of Tarjan's algorithm for Strongly Connected Component (Class) identification, and the dedicated logic for transitive link removal to construct the canonical Hasse Diagram.Furthermore, the robust suite of matrix operations, including efficient matrix exponentiation ($M^n$), confirms our competency in simulating the probabilistic evolution of these systems over time. The solid, modular structure of the codebase provides a reliable and extensible platform for the complete structural and probabilistic decomposition of various Markov graphs.