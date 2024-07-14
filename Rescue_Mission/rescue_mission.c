#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define GRID_SIZE 8

typedef enum {
    TERR_KILLED,
    HOST_RESCUED
} CountType;

typedef struct {
    int grid[GRID_SIZE + 1][GRID_SIZE + 1];
} Grid;

typedef struct {
    int row;
    int col;
} Coordinate;

// Function prototypes
static void print_help(void);
static void print_grid(Grid *g);
static void set_grid(Grid *g, Coordinate coord);
static void result(Grid *g);
static int count(Grid *g, CountType type);

int main(int argc, char *argv[]) {
    Grid g = {{{0}}};
    Coordinate coords[GRID_SIZE] = {{0}};
    int chopper = 0;
    int coord_index = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-h] -R <chopper_count> -C <x1,y1> <x2,y2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-Cc") == 0) {
            if (i + 1 < argc) {
                chopper = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: No value provided for -Cc option.\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-C") == 0) {
            while (i + 1 < argc && strchr(argv[i + 1], ',') != NULL) {
                if (coord_index < GRID_SIZE) {
                    sscanf(argv[++i], "%d,%d", &coords[coord_index].row, &coords[coord_index].col);
                    coord_index++;
                } else {
                    fprintf(stderr, "Error: Too many coordinates provided.\n");
                    return EXIT_FAILURE;
                }
            }
        } else {
            fprintf(stderr, "Invalid option. Use -h for help\n");
            return EXIT_FAILURE;
        }
    }

    if (chopper != coord_index) {
        fprintf(stderr, "Error: Number of coordinates provided does not match the chopper count.\n");
        return EXIT_FAILURE;
    }

    printf("In this 8x8 house, each room has 1 terrorist and 1 hostage...\n");
    printf("---------------------------MISSION...!---------------------------\n");

    // Initialize the array
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            g.grid[i][j] = 1;
        }
    }

    print_grid(&g);
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < chopper; i++) {
        Coordinate coord = coords[i];

        if ((coord.row >= 1 && coord.row <= GRID_SIZE) && (coord.col >= 1 && coord.col <= GRID_SIZE)) {
            printf("----------------------------------------------------------------------------\n");
            printf("Landing chopper #%d at location (%d,%d)\n", i + 1, coord.row, coord.col);

            set_grid(&g, coord);
            print_grid(&g);
            printf("----------------------------------------------------------------------------\n");
            result(&g);
        } else {
            printf("Invalid value for coordinates (%d,%d)...\n", coord.row, coord.col);
        }
    }

    return EXIT_SUCCESS;
}

// Function to print help
static void print_help(void) {
    printf("Usage: lahtp-rescue-tool [options]\n");
    printf("Options:\n");
    printf("\t-h                      Show this help message and exit\n");
    printf("\t-Cc <chopper_count>     Specify the number of choppers for the rescue operation\n");
    printf("\t-C <x1,y1> <x2,y2> ...  Specify the coordinates for the choppers to land\n");
    printf("\nExample:\n");
    printf("\t./rescue_mission -Cc 2 -C 2,3 3,4\n");
    printf("\tThis will perform a rescue operation with 2 choppers landing at coordinates (2,3) and (3,4)\n");
}

// Function to print the current state of the grid
static void print_grid(Grid *g) {
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            printf("%d ", g->grid[i][j]);
        }
        printf("\n");
    }
}

// Function to update the grid based on the chopper's landing position
static void set_grid(Grid *g, Coordinate coord) {
    for (int i = 1; i <= GRID_SIZE; i++) {
        g->grid[coord.row][i] = 0;
        g->grid[i][coord.col] = 0;
    }
}

// Function to print the result of the rescue operation
static void result(Grid *g) {
    int terrorists = count(g, TERR_KILLED);
    int hostages = count(g, HOST_RESCUED);

    printf("The total number of terrorists killed: %d\n", terrorists);
    printf("The total number of hostages rescued: %d\n", hostages);
}

// Function to count the number of terrorists killed or hostages rescued
static int count(Grid *g, CountType type) {
    int count = 0;
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            if (type == TERR_KILLED && g->grid[i][j] == 0) {
                count++;
            } else if (type == HOST_RESCUED && g->grid[i][j] == 0) {
                count++;
            }
        }
    }
    return count;
}
