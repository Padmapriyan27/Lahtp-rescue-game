#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ANSI color codes */
#define COLOR_RESET  "\033[0m"
#define COLOR_RED    "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_CYAN   "\033[36m"

/* Constants */
#define GRID_SIZE 8

typedef enum {
	TERR_KILLED,
	HOST_RESCUED
} CountType;

typedef struct {
	int grid[GRID_SIZE + 1][GRID_SIZE + 1];
} Grid;

typedef struct {
	size_t row;
	size_t col;
} Coordinate;

/* Function prototypes */
static void print_help(void);
static void print_grid(const Grid *g);
static void set_grid(Grid *g, Coordinate coord);
static void result(const Grid *g);
static size_t count(const Grid *g, CountType type);

int main(int argc, char *argv[])
{
	Grid g = { { { 0 } } };
	Coordinate coords[GRID_SIZE] = { { 0 } };
	size_t chopper = 0, coord_index = 0;
	size_t i;

	if (argc < 2) {
		fprintf(stderr, COLOR_RED "Usage: %s [-h] -Cc <chopper_count> -C <x1,y1> <x2,y2> ...\n" COLOR_RESET,
			argv[0]);
		return EXIT_FAILURE;
	}

	for (i = 1; i < (size_t)argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			print_help();
			return EXIT_SUCCESS;
		} else if (strcmp(argv[i], "-Cc") == 0) {
			if (i + 1 < (size_t)argc) {
				char *endptr;
				chopper = strtol(argv[++i], &endptr, 10);
				if (*endptr != '\0' || chopper < 1 || chopper > GRID_SIZE) {
					fprintf(stderr, COLOR_RED "Error: Invalid value for -Cc option. "
						"Must be between 1 and %d.\n" COLOR_RESET, GRID_SIZE);
					return EXIT_FAILURE;
				}
			} else {
				fprintf(stderr, COLOR_RED "Error: No value provided for -Cc option.\n" COLOR_RESET);
				return EXIT_FAILURE;
			}
		} else if (strcmp(argv[i], "-C") == 0) {
			while (i + 1 < (size_t)argc && strchr(argv[i + 1], ',') != NULL) {
				if (coord_index < GRID_SIZE) {
					if (sscanf(argv[++i], "%zu,%zu",
						&coords[coord_index].row,
						&coords[coord_index].col) != 2 ||
					        coords[coord_index].row < 1 ||
					        coords[coord_index].row > GRID_SIZE ||
					        coords[coord_index].col < 1 ||
					        coords[coord_index].col > GRID_SIZE) {
						fprintf(stderr, COLOR_RED "Error: Invalid coordinate "
							"format or out of bounds: %s.\n" COLOR_RESET,
							argv[i]);
						return EXIT_FAILURE;
					}
					coord_index++;
				} else {
					fprintf(stderr, COLOR_RED "Error: Too many coordinates "
						"provided. Maximum is %d.\n" COLOR_RESET,
						GRID_SIZE);
					return EXIT_FAILURE;
				}
			}
		} else {
			fprintf(stderr, COLOR_RED "Invalid option. Use -h for help\n" COLOR_RESET);
			return EXIT_FAILURE;
		}
	}

	if (chopper != coord_index) {
		fprintf(stderr, COLOR_RED "Error: Number of coordinates provided does not match "
			"the chopper count.\n" COLOR_RESET);
		return EXIT_FAILURE;
	}

	printf(COLOR_GREEN "In this 8x8 house, each room has 1 terrorist and 1 hostage...\n" COLOR_RESET);
	printf(COLOR_GREEN "---------------------------MISSION...!---------------------------\n" COLOR_RESET);

	/* Initialize the array */
	for (i = 1; i <= GRID_SIZE; i++) {
		size_t j;

		for (j = 1; j <= GRID_SIZE; j++)
			g.grid[i][j] = 1;
	}

	print_grid(&g);
	printf(COLOR_GREEN "----------------------------------------------------------------------------\n" COLOR_RESET);

	for (i = 0; i < chopper; i++) {
		Coordinate coord = coords[i];

		if ((coord.row >= 1 && coord.row <= GRID_SIZE) &&
		    (coord.col >= 1 && coord.col <= GRID_SIZE)) {
			printf(COLOR_GREEN "----------------------------------------------------------------------------\n" COLOR_RESET);
			printf(COLOR_GREEN "Landing chopper #%zu at location (%zu,%zu)\n" COLOR_RESET,
				i + 1, coord.row, coord.col);

			set_grid(&g, coord);
			print_grid(&g);
			printf(COLOR_GREEN "----------------------------------------------------------------------------\n" COLOR_RESET);
			result(&g);
		} else {
			fprintf(stderr, COLOR_RED "Error: Invalid value for coordinates (%zu,%zu).\n" COLOR_RESET,
				coord.row, coord.col);
		}
	}

	return EXIT_SUCCESS;
}

/* Function to print help */
static void print_help(void)
{
	printf(COLOR_YELLOW "Usage: lahtp-rescue-tool [options]\n" COLOR_RESET);
	printf(COLOR_YELLOW "Options:\n" COLOR_RESET);
	printf(COLOR_YELLOW "\t-h                      Show this help message and exit\n" COLOR_RESET);
	printf(COLOR_YELLOW "\t-Cc <chopper_count>     Specify the number of choppers for the rescue operation\n" COLOR_RESET);
	printf(COLOR_YELLOW "\t-C <x1,y1> <x2,y2> ...  Specify the coordinates for the choppers to land\n" COLOR_RESET);
	printf(COLOR_YELLOW "\nExample:\n" COLOR_RESET);
	printf(COLOR_YELLOW "\t./rescue_mission -Cc 2 -C 2,3 3,4\n" COLOR_RESET);
	printf(COLOR_YELLOW "\tThis will perform a rescue operation with 2 choppers landing at coordinates (2,3) and (3,4)\n" COLOR_RESET);
}

/* Function to print the current state of the grid */
static void print_grid(const Grid *g)
{
	size_t i, j;

	for (i = 1; i <= GRID_SIZE; i++) {
		for (j = 1; j <= GRID_SIZE; j++) {
			printf(COLOR_CYAN "%d " COLOR_RESET, g->grid[i][j]);
		}
		printf("\n");
	}
}

/* Function to update the grid based on the chopper's landing position */
static void set_grid(Grid *g, Coordinate coord)
{
	size_t i;

	for (i = 1; i <= GRID_SIZE; i++) {
		g->grid[coord.row][i] = 0;
		g->grid[i][coord.col] = 0;
	}
}

/* Function to print the result of the rescue operation */
static void result(const Grid *g)
{
	size_t terrorists = count(g, TERR_KILLED);
	size_t hostages = count(g, HOST_RESCUED);

	printf(COLOR_GREEN "The total number of terrorists killed: %zu\n" COLOR_RESET, terrorists);
	printf(COLOR_GREEN "The total number of hostages rescued: %zu\n" COLOR_RESET, hostages);
}

/* Function to count the number of terrorists killed or hostages rescued */
static size_t count(const Grid *g, CountType type)
{
	size_t count = 0, i, j;

	for (i = 1; i <= GRID_SIZE; i++) {
		for (j = 1; j <= GRID_SIZE; j++) {
			if ((type == TERR_KILLED || type == HOST_RESCUED) &&
			    g->grid[i][j] == 0)
				count++;
		}
	}

	return count;
}
