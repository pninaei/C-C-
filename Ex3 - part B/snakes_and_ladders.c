#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define NUM_OF_ARGS 3

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node,
                                     markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])->data;
                add_node_to_counter_list(from_node,
                                         to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * the function receive a Cell and prints his data - the number of it
 * @param cell_data
 */
static void print_data_cell (const void *cell_data){
    Cell *data = (Cell *)cell_data;
    if (data->number == BOARD_SIZE){
        printf("[%d]", data->number);
    }
    else if (data->snake_to == EMPTY && data->ladder_to == EMPTY){
    printf("[%d] -> ", data->number);
    } else if (data->ladder_to != EMPTY){
        printf("[%d]-ladder to %d -> ", data->number, data->ladder_to);
    }
    else{
        printf("[%d]-snake to %d -> ", data->number, data->snake_to);
    }
}
/**
 * the function receive 2 Cell structs and compare between then and return 0
 * if they equal, 1 if the first is bigger and -1 otherwise
 * @param cell1
 * @param cell2
 * @return
 */
static int compare_cells_data (const void *cell1, const void *cell2) {
    Cell *cell_1 = (Cell *) cell1;
    Cell *cell_2 = (Cell *) cell2;
    return cell_1->number - cell_2->number;
}
/**
 * the function receive a Cell and checks if this is cell number 100
 * @param cell
 * @return true if the is cell 100 and false otherwise
 */
static bool is_last (void *cell){
    Cell *cell_1 = (Cell *)cell;
    if (cell_1->number == BOARD_SIZE){
        return true;
    } else{
        return false;
    }
}
/**
 * the function gets Cell and allocate memory and copy the data from the
 * cell into it
 * @param cell
 * @return new allocated memory and NULL otherwise
 */
static void *allocate_and_copy (void *cell){
    Cell *cell_1 = (Cell *)cell;
    Cell *cell_dest = malloc(sizeof (Cell));
    if (!cell_dest){
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    cell_dest->number = cell_1->number;
    cell_dest->ladder_to = cell_1->ladder_to;
    cell_dest->snake_to = cell_1->snake_to;
    return (void *)cell_dest;
}

/**
 * the function free the memory that was allocated to the data field in the
 * Cell
 * @param data
 */
static void free_data_cell(void *data){
    Cell *cell = (Cell *)data;
    free(cell);
    cell = NULL;
}


/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGS){
        return EXIT_FAILURE;
    }
    long seed = strtol(argv[1], NULL, 10);
    srand(seed);
    long num_of_routes = strtol(argv[2], NULL, 10);

    LinkedList *lst = calloc(1, sizeof(LinkedList));
    if (!lst) {
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (!markov_chain) {
        free(lst);
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    lst->first = NULL;
    lst->last = NULL;
    lst->size = 0;
    markov_chain->database = lst;
    markov_chain->print_func = print_data_cell;
    markov_chain->comp_func = compare_cells_data;
    markov_chain->free_data = free_data_cell;
    markov_chain->copy_func = allocate_and_copy;
    markov_chain->is_last = is_last;
    int fill = fill_database(markov_chain);
    if (fill == 1){
        fprintf(stdout, "%s", ALLOCATION_ERROR_MASSAGE);
        free_markov_chain(&markov_chain);
        return EXIT_FAILURE;
    }

    for (int i=0; i<num_of_routes; i++){
        printf("Random Walk %d: ", i+1);
        MarkovNode *first_node = markov_chain->database->first->data;
        generate_random_sequence(markov_chain, first_node,
                                 MAX_GENERATION_LENGTH);
        printf("\n");


    }
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}
