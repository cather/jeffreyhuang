/*! AI module

    Implements the Artifical Intelligence of the computer controlled snake by
    providing a function that determines the computer's next move based on the
    current game state.
    
    The AI's goal is simply to find the shortest path to the nearest food, and to
    then follow that path.  If food is inaccessible, then the AI's goal is to avoid
    hitting walls.
*/

#include "ai.h"

#include <limits.h>
#include <stdlib.h>

void find_food(board * cur_board, int * distance_map, int row, int col,
               int distance, int * closest_distance)
/*! Given a board (data structure specified in game.h), a starting row and column,
    and a given path distance traveled, recursively search the board to find the
    shortest path to food.  The closest_distance input/output parameter indicates
    the closest distance to food found thus far.  When closer food is found, update
    closest_distance to reflect that.

    The distance map parameter is an array of integers corresponding to the board
    cell array.  In order for the search of large boards to finish in a reasonable
    amount of time, each entry in the distance map should be updated with the shortest
    path distance found to the corresponding cell thus far and a branch of the search
    should be ignored if it finds a path to a cell that is as long or longer than the
    shortest distance recorded in the distance map for that cell. (because that
    search is going to produce the same or worse results)
*/
{   
    int *temp = &distance_map[row * cur_board->cols + col];     // grabs the distance map value as a temp just cuz its annoying to type

    if (row >= 0 && row < cur_board->rows && col >= 0 && col < cur_board->cols) {       // check the bounds

        if (*board_cell(cur_board, row, col) == CELL_FOOD || *board_cell(cur_board, row, col) == CELL_OPEN) {

            if (*temp > distance) {     // if the current distance map is more than the distance

                *temp = distance;       // set the distance

                if (*board_cell(cur_board, row, col) == CELL_FOOD && *temp < *closest_distance) {
                    *closest_distance = *temp;      // set the closest value
                }

                distance++;     // increment distance

                find_food(cur_board, distance_map, row - 1, col, distance, closest_distance);   // call find food in all directions
                find_food(cur_board, distance_map, row + 1, col, distance, closest_distance);
                find_food(cur_board, distance_map, row, col + 1, distance, closest_distance);
                find_food(cur_board, distance_map, row, col - 1, distance, closest_distance);

                return;
            }


        }
    }
   
}

int food_distance(board * cur_board, int * distance_map, int row, int col)
/*! Given a board (data structure defined in game.h), a distance map (format defined
    in find_food) which is only allocated, not initialized, and a starting row and
    column, return the closest path distance to food.
    If no food is found, return INT_MAX (defined in limits.h,
    see http://uw714doc.sco.com/en/man/html.4/limits.4.html )
*/
{

    int closest_distance = INT_MAX;
    int idx;

    for (idx = 0; idx < cur_board->rows * cur_board->cols; idx++) {
        distance_map[idx] = INT_MAX;
    }

    find_food(cur_board, distance_map, row, col, 0, &closest_distance);


    return closest_distance;

}

void avoid_walls(board * cur_board, snake * cur_snake)
/*! Given a board and a snake (see game.h for data structure definitions)
    modify the heading of the given snake to avoid crashing into walls or
    snakes, if possible.
    This function assumes that no food is in reach of the given snake.
*/
{

    int row, col;
    row = cur_snake->head->row;
    col = cur_snake->head->col;

    if (*board_cell(cur_board, row + 1, col) == CELL_OPEN)
        cur_snake->heading = SOUTH;
    else if (*board_cell(cur_board, row - 1, col) == CELL_OPEN)
        cur_snake->heading = NORTH;
    else if (*board_cell(cur_board, row, col + 1) == CELL_OPEN)
        cur_snake->heading = EAST;
    else if (*board_cell(cur_board, row, col - 1) == CELL_OPEN)
        cur_snake->heading = WEST;

    // if (*board_cell(cur_board, row - 1, col) != CELL_OPEN && cur_snake->heading == NORTH) {
    //     cur_snake->heading = EAST;
    //     avoid_walls(cur_board, cur_snake);
    // }
    // else if (*board_cell(cur_board, row + 1, col) != CELL_OPEN && cur_snake->heading == SOUTH) {
    //     cur_snake->heading = WEST;
    //     avoid_walls(cur_board, cur_snake);
    // }
    // else if (*board_cell(cur_board, row, col + 1) != CELL_OPEN && cur_snake->heading == EAST) {
    //     cur_snake->heading = SOUTH;
    //     avoid_walls(cur_board, cur_snake);
    // }
    // else if (*board_cell(cur_board, row, col - 1) != CELL_OPEN && cur_snake->heading == WEST) {
    //     cur_snake->heading = NORTH;
    //     avoid_walls(cur_board, cur_snake);
    // }


    return;
    
}

void ai_move(game * cur_game)
/*! Given a game state (game data structure defined in game.h), determine the
    computer's next move.  For each cell adjacent to the snake head, search for
    food.  If food is found, change the computer snake's heading to go to the cell
    that has the closest distance to food. (If two or more cells have the shortest
    distance, then you are free to choose one of them)

    If no food is found, then simply call the avoid_walls function.
*/
{

    int row = cur_game->computer_snake->head->row;
    int col = cur_game->computer_snake->head->col;
    int min;
    int i;
    direction *temp = &cur_game->computer_snake->heading;

    int * distance_map = (int *) malloc(sizeof(int) * cur_game->board->rows * cur_game->board->cols);
    int distanceVals[4];

    distanceVals[0] = food_distance(cur_game->board, distance_map, row + 1, col);   // south
    distanceVals[1] = food_distance(cur_game->board, distance_map, row - 1, col);   // north
    distanceVals[2] = food_distance(cur_game->board, distance_map, row, col + 1);   // east
    distanceVals[3] = food_distance(cur_game->board, distance_map, row, col - 1);   // west

    free(distance_map);

    min = distanceVals[0];      // set initial minimum value
    for (i = 0; i < 4; i++) {   // find the minimum value
        if (distanceVals[i] < min) {
            min = distanceVals[i];
        }
    }


    if (min == INT_MAX) {
        avoid_walls(cur_game->board, cur_game->computer_snake);
        return;
    }
    if (min == distanceVals[0]) {   // check for the minimum value
        *temp = SOUTH;
    }
    if (min == distanceVals[1]) {
        *temp = NORTH;
    }
    if (min == distanceVals[2]) {
        *temp = EAST;
    }
    if (min == distanceVals[3]) {
        *temp = WEST;
    }

    return;

}



