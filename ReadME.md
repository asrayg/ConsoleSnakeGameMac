# Console Snake Game (Mac)

A terminal-based Snake game implemented in C with multiple iterations, each adding more complexity and features. This repository includes three versions: Basic, Enhanced (with levels), and Cooler (with additional features).

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Compilation and Execution](#compilation-and-execution)
- [Game Controls](#game-controls)
- [Project Structure](#project-structure)
- [Learning Exercise](#learning-exercise)
- [Credits](#credits)

## Features

### Basic Version
- Simple Snake game
- Controls using arrow keys
- Food generation and consumption
- Score tracking

### Enhanced Version
- Game levels that increase as you score more points
- Obstacles to avoid
- Dynamic speed increase with levels
- Scoreboard displaying high scores

### Cooler Version
- Dynamic title screen with animation
- Special food items for extra points and speed boosts
- Game over animation
- Color differentiation for snake, food, and obstacles using `ncurses`
- Sound effects (limited to terminal capabilities)

## Prerequisites

Ensure you have the following installed on your system:

- GCC Compiler
- `ncurses` library

You can install these using the following commands:

```sh
sudo apt-get install build-essential
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Setup

Clone the repository and navigate to the project directory:

```sh
git clone <repository_url>
cd console-snake-game-mac
```

## Compilation and Execution

### Basic Version

Navigate to the `basic-game` directory and compile the game:

```sh
cd basic-game
gcc main-snake.c -o snake_game
./snake_game
```

### Enhanced Version (Game with Levels)

Navigate to the `GameWithLevels` directory and compile the game:

```sh
cd GameWithLevels
gcc -c main.c -o main.o
gcc -c snake.c -o snake.o
gcc -c utils.c -o utils.o
gcc main.o snake.o utils.o -o snake_game
./snake_game
```

### Cooler Version

Navigate to the `CoolerGame` directory and compile the game using the provided `Makefile`:

```sh
cd CoolerGame
make
./snake_game
```

## Game Controls

- **Arrow Keys**: Control the direction of the snake
- **P** or **ESC**: Pause the game
- **Enter**: Select menu options

## Project Structure

```
console-snake-game-mac/
├── basic-game/
│   └── main-snake.c          # Basic version of the game
├── GameWithLevels/
│   ├── main.c                # Main game loop and menu
│   ├── snake.c               # Game logic and functions
│   ├── snake.h               # Function declarations and constants
│   └── utils.c               # Utility functions
├── CoolerGame/
│   ├── main.c                # Main game loop and menu with cool features
│   ├── snake.c               # Enhanced game logic and functions
│   └── snake.h               # Function declarations and constants
└── README.md                 # Project documentation
```

### basic-game

Contains the basic version of the game in a single file.

### GameWithLevels

Implements the enhanced version with game levels, obstacles, and dynamic speed.

### CoolerGame

Implements the coolest version with a dynamic title screen, special food items, color differentiation using `ncurses`, and more.

## Learning Exercise

This project was undertaken as a learning exercise to understand the fundamentals of C programming. The development of the Snake game through multiple iterations allowed for hands-on experience with various programming concepts, including:

- **Basic Syntax and Structure**: Writing and compiling simple C programs.
- **Control Structures**: Using loops, conditionals, and functions to control the game flow.
- **Data Structures**: Managing arrays and custom data types for game elements like the snake and obstacles.
- **Dynamic Memory Management**: Understanding how to allocate and manage memory dynamically.
- **File I/O**: Reading from and writing to files for high score management (in future enhancements).
- **Using Libraries**: Incorporating the `ncurses` library to enhance the terminal user interface with colors and animations.
- **Debugging and Optimization**: Identifying and fixing bugs, optimizing the game for smoother performance.
- **Modular Programming**: Splitting the code into multiple files and using header files for better organization and maintainability.

This project provided a comprehensive introduction to C programming and offered practical experience in building a functional application from scratch.

## Credits

- **Original Author**: Matthew Vlietstra
- **Enhancements and Additional Features**: Asray Gopa

This project is an adaptation and enhancement of the original console-based Snake game by Matthew Vlietstra, with additional features, visual improvements, and better C coding techniques.
