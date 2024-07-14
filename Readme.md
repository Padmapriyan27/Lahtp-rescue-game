# Rescue Mission Tool

This project simulates a rescue mission in an 8x8 grid where each room contains a terrorist and a hostage. The objective is to rescue hostages by landing choppers at specified coordinates.

## Getting Started

### Prerequisites

To compile and run this project, you need:

- A C compiler (e.g., `gcc`)

### Compilation

Compile the project using the following command:

```bash
gcc -o rescue_mission rescue_mission.c
```

### Usage

To execute the compiled program, use the following command:

```bash
./rescue_mission [options]
```

#### Options

- `-h`: Show the help message and exit.
- `-Cc <chopper_count>`: Specify the number of choppers for the rescue operation.
- `-C <x1,y1> <x2,y2> ...`: Specify the coordinates for the choppers to land.

#### Example

```bash
./rescue_mission -Cc 2 -C 2,3 3,4
```

This command will perform a rescue operation with 2 choppers landing at coordinates (2,3) and (3,4).

## Project Structure

The project contains the following files:

- `rescue_mission.c`: The main source code file.

## Functions

### `main`

The entry point of the program. It parses command-line arguments, initializes the grid, and performs the rescue operation.

### `print_help`

Displays the help message.

### `print_grid`

Prints the current state of the grid.

### `set_grid`

Updates the grid based on the chopper's landing position.

### `result`

Prints the result of the rescue operation.

### `count`

Counts the number of terrorists killed or hostages rescued.

## Grid Representation

The grid is represented as an 8x8 matrix where:

- Each cell initially contains a terrorist and a hostage.
- Landing a chopper at a coordinate will neutralize all terrorists in the respective row and column.

