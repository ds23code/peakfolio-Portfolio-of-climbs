// Program name CS CLIMB
//
//NAME:Dhruv Sharma

//Description of program
/*The program, named "CS CLIMB," allows users to add, view,
and manipulate climbing routes, attempts, and the overall logbook, 
providing a comprehensive tool for tracking and analyzing climbing experiences.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Constants       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define MAX_STR_LEN 20
#define HELP_COMMAND '?'

// This enum only has to be used in Stage 3.1 onwards
enum attempt_type {FIRST_GO, SUCCESS, FAIL, INVALID_TYPE};

// TODO: Your #defines/enums can go here:
#define MIN_DIFF 0
#define MAX_DIFF 39
#define MIN_LEN 0
#define MAX_LEN 60
#define ERROR 404
#define TRUE 1
#define FALSE 0
#define MIN_RATING 0
#define MAX_RATING 3
#define APPEND_ROUTE 'r'
#define PRINT_ROUTE 'p'
#define FILTER_ROUTE 'f'
#define INSERT_ROUTE 'i'
#define CHANGE_LENGTH 'l'
#define SWAP_ROUTE 's'
#define ADD_ATTEMPT 'a'
#define PRINT_LOGBOOK 'P'
#define REMOVE_ROUTE 'R'
#define DELETE_ATTEMPT 'D'
#define DUPLICATE_ATTEMPTS 'd'

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct route {
    // The name of the climbing route
    char name[MAX_STR_LEN];
    // The difficulty of the route
    int difficulty;
    // The length of the route in metres
    int length;
    // A pointer to the next `struct route` in the logbook 
    struct route *next;
    // Stores a pointer to the head of the `attempts` list
    struct attempt *attempts;
};

// Represents the logbook that contains info on each climbing route
struct logbook {
    struct route *routes;
    int counter;
    int difficulty_min;
    int difficulty_max;
};

struct attempt {
    char climber[MAX_STR_LEN];
    enum attempt_type type;
    int rating;
    struct attempt *next;
};

void print_usage(void); 
void print_one_route(int position, struct route *route);
void scan_string(char string[MAX_STR_LEN]);
enum attempt_type scan_attempt_type();
void print_one_attempt(
    char climber[MAX_STR_LEN], 
    enum attempt_type type, 
    int rating
);

////////////////////////////////////////////////////////////////////////////////
int scan_token(char *buffer, int buffer_size);
enum attempt_type string_to_type(char *type_str);
void type_to_string(char *buf, enum attempt_type type);
struct logbook *create_logbook();
struct route *create_route(char name[MAX_STR_LEN], int difficulty, int length);
void append_route(struct logbook *logbook);
void print_routes(struct logbook *logbook);
int errorcheck(char name_entered[MAX_STR_LEN], int difficuly_entered, 
int length_entered, struct logbook *logbook);
void filter(struct logbook *logbook);
void insert(struct logbook *logbook);
int change_length_errorcheck(int length_entered, char route_1[MAX_STR_LEN], 
char route_2[MAX_STR_LEN], struct logbook *logbook);
void change_length(struct logbook *logbook);
void swap_routes(struct logbook *logbook);
int swap_route_error_check(char route_1[MAX_STR_LEN], char route_2[MAX_STR_LEN], 
struct logbook *logbook);
void add_attempt(struct logbook *logbook);
int add_attempt_error_check(char climber[MAX_STR_LEN], enum attempt_type type, 
int rating, char route[MAX_STR_LEN], struct logbook *logbook);
struct route *find_route(struct logbook *logbook, char route[MAX_STR_LEN]);
void print_logbook(struct logbook *logbook); 
struct attempt *create_attempt(char climber[MAX_STR_LEN], 
enum attempt_type type, int rating);
void remove_route(struct logbook *logbook);
void clean_logbook(struct logbook *logbook);
void delete_attempt(struct logbook *logbook);
void duplicate(struct logbook *logbook);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");
    struct logbook *logbook = create_logbook();
    char user_letter;

    printf("Enter command: ");
    while (scanf(" %c", &user_letter) != -1) {
        if (user_letter == HELP_COMMAND) {
            print_usage();
        }
        if (user_letter == APPEND_ROUTE) {
            append_route(logbook);
        }
        if (user_letter == PRINT_ROUTE) {
            print_routes(logbook);
        }
        if (user_letter == FILTER_ROUTE) {
            filter(logbook);
        }
        if (user_letter == INSERT_ROUTE) {
            insert(logbook);
        }
        if (user_letter == CHANGE_LENGTH) {
            change_length(logbook);
        }
        if (user_letter == SWAP_ROUTE) {
            swap_routes(logbook);
        }
        if (user_letter == ADD_ATTEMPT) {
            add_attempt(logbook);
        }
        if (user_letter == PRINT_LOGBOOK) {
            print_logbook(logbook);
        }
        if (user_letter == REMOVE_ROUTE) {
            remove_route(logbook);
        }
        if (user_letter == DELETE_ATTEMPT) {
            delete_attempt(logbook);
        }
        if (user_letter == DUPLICATE_ATTEMPTS) {
            duplicate(logbook);
        }
        printf("Enter command: ");
    }
    //Clean up the logbook and exits
    clean_logbook(logbook);
    printf("\nGoodbye\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////


// Allocate memory for a new struct logbook and return a pointer to it
struct logbook *create_logbook() {
    // Allocate memory for a new logbook
    struct logbook *new_logbook = malloc(sizeof(struct logbook));
    // Initialize logbook properties
    new_logbook->routes = NULL;
    new_logbook->counter = 0;
    new_logbook->difficulty_min = 0;
    new_logbook->difficulty_max = 0; 
    // Return the new logbook
    return new_logbook;
}

// Allocate memory for a new route and initialize its properties
struct route *create_route(char name[MAX_STR_LEN], int difficulty, int length) {
    // Allocate memory for a new route
    struct route *new_route = malloc(sizeof(struct route));
    // Copy the name to the new route
    strcpy(new_route->name, name);
    // Set the difficulty and length of the route
    new_route->difficulty = difficulty;
    new_route->length = length;
    // Initialize the next and attempts pointers to NULL
    new_route->next = NULL;
    new_route->attempts = NULL;
    // Return the newly created route
    return new_route;
}

// Append a route to the logbook
void append_route(struct logbook *logbook) {
    // Variables to store user inputs
    char user_enters_name[MAX_STR_LEN];
    int user_enters_difficulty;
    int user_enters_length;
    // Read user inputs
    scan_string(user_enters_name);
    scanf(" %d", &user_enters_difficulty);
    scanf(" %d", &user_enters_length);

    // Check for errors in user inputs
    if (errorcheck(user_enters_name, user_enters_difficulty, 
    user_enters_length, logbook) == 404)
        return;

    printf("Route '%s' added successfully!\n", user_enters_name);
    logbook->counter++;

    // Find the last route in the logbook
    struct route *curr_route = logbook->routes;
    if (logbook->routes == NULL) {
        // If the logbook is empty, create a new route 
        //and set it as the first route
        logbook->routes = create_route(user_enters_name, 
        user_enters_difficulty, user_enters_length);
        return;
    }
    // Traverse to the last route
    while (curr_route->next != NULL) {
        curr_route = curr_route->next;
    }
    // Create a new route and link it to the last route
    struct route *new_route = create_route(user_enters_name, 
    user_enters_difficulty, user_enters_length);
    curr_route->next = new_route;

}

// Print all routes in the logbook
void print_routes(struct logbook *logbook) {
    if (logbook->counter == 0) {
        printf("There are no routes in this logbook!\n");
        return;
    }
    if (logbook->counter > 0) {
        struct route *curr_route = logbook->routes;
        for (int i = 1; i <= logbook->counter; i++) {
            print_one_route(i, curr_route);
            curr_route = curr_route->next;
        }
    }
}

//Filter routes in the logbook
void filter(struct logbook *logbook) {
    scanf(" %d", &logbook->difficulty_min);
    scanf(" %d", &logbook->difficulty_max);

    if (logbook->difficulty_min <= MIN_DIFF || 
        logbook->difficulty_min > MAX_DIFF || 
        logbook->difficulty_max <= MIN_DIFF || 
        logbook->difficulty_max > MAX_DIFF ||
        logbook->difficulty_min > logbook->difficulty_max) {
        printf("ERROR: Difficulty range invalid!\n");
        return;
    }

    printf("Routes between difficulty %d and %d:\n", 
    logbook->difficulty_min, logbook->difficulty_max);
    struct route *curr_route = logbook->routes;
    for (int i = 1; i <= logbook->counter; i++) {
        if (logbook->difficulty_min <= curr_route->difficulty && 
            logbook->difficulty_max >= curr_route->difficulty)
            print_one_route(i, curr_route);
        curr_route = curr_route->next;
    }
}

// Insert a route into the logbook
void insert(struct logbook *logbook) {
    // Variables to store user inputs
    char user_enters_name[MAX_STR_LEN];
    int user_enters_difficulty;
    int user_enters_length;
    char route_to_insert_before[MAX_STR_LEN];
    // Read user inputs
    scan_string(user_enters_name);
    scanf(" %d", &user_enters_difficulty);
    scanf(" %d", &user_enters_length);
    scan_string(route_to_insert_before);
    // Check for errors in user inputs
    struct route *curr_route = logbook->routes;
    struct route *new_route = create_route(user_enters_name, 
        user_enters_difficulty, user_enters_length);
    // Check for errors
    if (errorcheck(user_enters_name, user_enters_difficulty, 
        user_enters_length, logbook) == ERROR) {
        return;
    }
    // Check if the logbook is empty
    if (logbook->counter == 0) {
        printf("ERROR: No route with the name '%s' exists "
        "in this logbook\n", route_to_insert_before);
        return;
    }
    // Check if the route to insert before exists
    if (strcmp(route_to_insert_before, curr_route->name) == 0 || 
    logbook->routes == NULL) {
        new_route->next = logbook->routes;
        logbook->routes = new_route;
        logbook->counter++;
        printf("Route '%s' inserted successfully!\n", user_enters_name);
        return;
    }
    // Traverse to the route to insert before
    while (curr_route->next != NULL) {
        if (strcmp(curr_route->next->name, route_to_insert_before) == 0) {
            new_route->next = curr_route->next;
            curr_route->next = new_route;
            logbook->counter++;
            if (curr_route  == logbook->routes)
            break;
        }
        curr_route = curr_route->next;
    }
    printf("Route '%s' inserted successfully!\n", user_enters_name);
}

// Error check for changing route length
int errorcheck(char name_entered[MAX_STR_LEN], int difficuly_entered, 
int length_entered, struct logbook *logbook) {
    struct route *curr_route = logbook->routes;
    // Check for errors
    if (MIN_DIFF >= difficuly_entered || MAX_DIFF < difficuly_entered) {
        printf("ERROR: Route difficulty must be between 1 and 39\n");
        return ERROR;
    }
    else if (MIN_DIFF >= length_entered || MAX_LEN < length_entered) {
        printf("ERROR: Route length must be between 1m and 60m\n");
        return ERROR;
    }
    else if (logbook->routes == NULL) {
        return 999;
    }
    // Check if the route name already exists
    while (curr_route != NULL) {
        if (strcmp(curr_route->name, name_entered) == 0) {
            printf("ERROR: A route with the name '%s' already"
            " exists in this logbook\n", name_entered);
            return ERROR;
        }
        curr_route = curr_route->next;
    }
    return 0;
}

// Change route length
void change_length(struct logbook *logbook) {
    // Variables to store user inputs
    char route_1[MAX_STR_LEN];
    char route_2[MAX_STR_LEN];
    int length_change;
    // Read user inputs
    scan_string(route_1);
    scan_string(route_2);
    scanf("%d", &length_change);
    // Check for errors
    if (change_length_errorcheck(length_change, route_1, route_2, logbook) == 404) {
        return;
    }
    // Traverse the logbook
    struct route *curr_route = logbook->routes;
    struct route *start_route = NULL;
    struct route *end_route = NULL;
    // Find the routes
    while (curr_route != NULL) {
        if (strcmp(route_1, curr_route->name) == 0 || 
        strcmp(route_2, curr_route->name) == 0) {
            if (start_route == NULL) {
                start_route = curr_route;
            }
            else {
                end_route = curr_route;
            }
        } 
        curr_route = curr_route->next;
    }
    // Update the length
    if (end_route == NULL) {
        end_route = start_route;
    }
    // Update the length
    curr_route = start_route;
    while (curr_route != end_route) {
        // Routes are the same, update the length directly
        curr_route->length += length_change;
        printf("Length of '%s' updated from %dm to %dm\n",
               curr_route->name, curr_route->length - length_change, curr_route->length);
        curr_route = curr_route->next;
    }
    // Update the length
    curr_route->length += length_change;
    printf("Length of '%s' updated from %dm to %dm\n",
        curr_route->name, curr_route->length - length_change, curr_route->length);
}

// Error check for changing route length
int change_length_errorcheck(int length_entered, char route_1[MAX_STR_LEN], 
    char route_2[MAX_STR_LEN], struct logbook *logbook) {
    struct route *curr_route = logbook->routes;
    int route_1_exists = 0;
    int route_2_exists = 0;
    // Check for errors
    while (curr_route != NULL) {
        if (strcmp(curr_route->name, route_1) == 0) {
            route_1_exists = 1;
        }
        if (strcmp(curr_route->name, route_2) == 0) {
            route_2_exists = 1;
        }
        curr_route = curr_route->next;
    }
    curr_route = logbook->routes;
    // Check if the route name already exists
    if (!route_1_exists) {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1);
        return ERROR;
    }
    if (!route_2_exists) {
        printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2);
        return ERROR;
    }
    // Check if the new length is valid
    while (curr_route != NULL) {
        int new_length = curr_route->length + length_entered;
        if (new_length <= 0 || new_length > 60) {
            printf("ERROR: Invalid length change! No route lengths have been changed\n");
            return ERROR;
        }
        curr_route = curr_route->next;
    }
    return 0;
}

// Swap routes
void swap_routes(struct logbook *logbook) {
    char route_1[MAX_STR_LEN];
    char route_2[MAX_STR_LEN];
    scan_string(route_1);
    scan_string(route_2);
    // Check for errors
    if (swap_route_error_check(route_1, route_2, logbook) == 404) {
        return;
    }
    // Traverse the logbook
    struct route *curr_route = logbook->routes;
    struct route *route_1_ptr = NULL;
    struct route *route_2_ptr = NULL;
    // Find the routes to swap
    while (curr_route != NULL) {
        if (strcmp(curr_route->name, route_1) == 0) {
            route_1_ptr = curr_route;
        }
        else if (strcmp(curr_route->name, route_2) == 0) {
            route_2_ptr = curr_route;
        }
        curr_route = curr_route->next;
    }
    // Swap names
    if (route_1_ptr != NULL && route_2_ptr != NULL) {
        char temp[MAX_STR_LEN];
        strcpy(temp, route_1_ptr->name);
        strcpy(route_1_ptr->name, route_2_ptr->name);
        strcpy(route_2_ptr->name, temp);
        // Swap lengths and difficulties
        int temp_length = route_1_ptr->length;
        int temp_difficulty = route_1_ptr->difficulty;
        route_1_ptr->length = route_2_ptr->length;
        route_1_ptr->difficulty = route_2_ptr->difficulty;
        // Swap lengths and difficulties
        route_2_ptr->length = temp_length;
        route_2_ptr->difficulty = temp_difficulty;
        printf("'%s' swapped positions with '%s'!\n", route_1, route_2);
    } 
}

// Error check for swapping routes
int swap_route_error_check(char route_1[MAX_STR_LEN], 
    char route_2[MAX_STR_LEN], struct logbook *logbook) {
    struct route *curr_route = logbook->routes;
    int route_1_exists = FALSE;
    int route_2_exists = FALSE;
    // Check for errors
    while (curr_route != NULL) {
        if (strcmp(curr_route->name, route_1) == 0) {
            route_1_exists = TRUE;
        }
        if (strcmp(curr_route->name, route_2) == 0) {
            route_2_exists = TRUE;
        }
        curr_route = curr_route->next;
    }
    // Check if the route name already exists
    if (strcmp(route_1, route_2) == 0) {
        printf("ERROR: Cannot swap '%s' with itself\n", route_1);
        return ERROR;
    }
    curr_route = logbook->routes;
    // Check if the route name already exists
    if (route_1_exists == FALSE) {
        printf("ERROR: No route with the name '%s' "
        "exists in this logbook\n", route_1);
        return ERROR;
    }
    if (route_2_exists == FALSE) {
        printf("ERROR: No route with the name '%s' "
        "exists in this logbook\n", route_2);
        return ERROR;
    }
    return 0;
}

// Create an attempt
struct attempt *create_attempt(char climber[MAX_STR_LEN], 
    enum attempt_type type, int rating) {
    struct attempt *new_attempt = malloc(sizeof(struct attempt));
    if (new_attempt != NULL) {
        strcpy(new_attempt->climber, climber);
        new_attempt->type = type;
        new_attempt->rating = rating;
        new_attempt->next = NULL;
    }
    return new_attempt;
}

// Function to add an attempt to a logbook
void add_attempt(struct logbook *logbook) {
    // Get user input
    char climber[MAX_STR_LEN];
    enum attempt_type type;
    int rating;
    char user_enter_route[MAX_STR_LEN];
    scan_string(climber);
    type = scan_attempt_type();
    scanf(" %d", &rating);
    scan_string(user_enter_route);
    // Check for errors in user input
    if (add_attempt_error_check(climber, type, rating, 
    user_enter_route, logbook) == ERROR) {
        return;
    }
    // Find the route in the logbook
    struct route *route = find_route(logbook, user_enter_route);
    // If the route is not found, exit the function
    if (route == NULL) {
        return;
    }

    // Create a new attempt with the user input
    struct attempt *new_attempt = create_attempt(climber, 
        type, rating);
    struct attempt *current = route->attempts;
    struct attempt *last_attempt = NULL;
    
    // Iterate through existing attempts to find the 
    //correct position for the new attempt
    while (current != NULL && strcmp(current->climber, 
        climber) <= 0) {
        // If the climber has already attempted the route
        if (strcmp(current->climber, climber) == 0) {
            new_attempt->next = current;
            
            // Check for a special case
            if (type == FIRST_GO) {
                printf("ERROR: %s has already attempted '%s' - "
                "they can't get it on their first go!\n",
                climber, user_enter_route);
                return;
            }
            // Insert the new attempt at the correct position
            if (last_attempt == NULL) {
                route->attempts = new_attempt;
            } else {
                last_attempt->next = new_attempt;
            }
            printf("Logged attempt of '%s' by %s\n", user_enter_route,
            climber);
            return;
        }
        // Move to the next attempt
        last_attempt = current;
        current = current->next;
    }
    // Insert the new attempt at the correct position
    new_attempt->next = current;
    if (last_attempt == NULL) {
        route->attempts = new_attempt;
    } else {
        last_attempt->next = new_attempt;
    }
    printf("Logged attempt of '%s' by %s\n", user_enter_route, climber);
}

// Function to find a route
struct route *find_route(struct logbook *logbook, char route[MAX_STR_LEN]) {
    struct route *curr_route = logbook->routes;
    int route_exists = FALSE;
    // Check if the route exists
    while (curr_route != NULL) {
        if (strcmp(curr_route->name, route) == 0) {
            route_exists = TRUE;
            break;
        }
        curr_route = curr_route->next;
    }
    // If the route does not exist, exit the function
    if (route_exists == FALSE) {
        printf("ERROR: No route with the name '%s' exists "
        "in this logbook\n", route);
        return NULL;
    } else {
        return curr_route;
    }
} 

// Check for errors in user input
int add_attempt_error_check(char climber[MAX_STR_LEN], enum attempt_type type, 
int rating, char route[MAX_STR_LEN], struct logbook *logbook) {
    if (type == INVALID_TYPE) {
        printf("ERROR: Attempt type invalid\n");
        return ERROR;
    }
    if (rating < MIN_RATING || rating > MAX_RATING) {
        printf("ERROR: Rating must be between 0 and 3\n");
        return ERROR;
    }
    return 0;
}

// Print the logbook
void print_logbook(struct logbook *logbook) {
    // Check if the logbook is empty
    if (logbook->counter == 0) {
        printf("There are no routes in this logbook!\n");
        return;
    }
    // Print each route in the logbook
    int i = 1;
    struct route *curr_route = logbook->routes;
    // Traverse the logbook
    while (curr_route != NULL) {
        struct attempt *curr_attempt = curr_route->attempts;
        float sum_rating = 0;
        float num_attempts = 0;

        while (curr_attempt != NULL) {
            sum_rating += curr_attempt->rating;
            num_attempts++;
            curr_attempt = curr_attempt->next;
        }
        // Calculate the average rating
        double avg_rating = 0.0;
        if (num_attempts > 0) {
            avg_rating = sum_rating / num_attempts;
        }

        printf("Route #%d: %s\nDifficulty: %d | "
        "Length: %dm | Avg rating: %.1lf/3.0\n",
        i, curr_route->name, curr_route->difficulty, 
        curr_route->length, avg_rating);
        curr_attempt = curr_route->attempts;
        // Print each attempt
        while (curr_attempt != NULL) {
            print_one_attempt(curr_attempt->climber,
            curr_attempt->type, curr_attempt->rating);
            curr_attempt = curr_attempt->next;
        }
        // Move to the next route
        curr_route = curr_route->next;
        i++;
    }
}

// Clean the logbook
void clean_logbook(struct logbook *logbook) {
    struct route *current_route = logbook->routes;
    while (current_route != NULL) {
        struct route *next_route = current_route->next;
        struct attempt *current_attempt = current_route->attempts;
        while (current_attempt != NULL) {
            struct attempt *next_attempt = current_attempt->next;
            free(current_attempt);
            current_attempt = next_attempt;
        }
        free(current_route);
        current_route = next_route;
    }
    free(logbook);
}

// Remove a route
void remove_route(struct logbook *logbook) {
    char user_enter_route[MAX_STR_LEN];
    scan_string(user_enter_route);
    struct route *curr_route = logbook->routes;
    struct route *last_route = NULL;
    // Find the route to be removed
    while (curr_route != NULL && strcmp(curr_route->name, user_enter_route) != 0) {
        last_route = curr_route;
        curr_route = curr_route->next;
    }
    // Check if the route exists
    if (curr_route == NULL) {
        printf("ERROR: No route with the name '%s' exists in this logbook\n",
               user_enter_route);
        return;
    }
    // Remove the route from the linked list
    if (last_route != NULL) {
        last_route->next = curr_route->next;
    } else {
        logbook->routes = curr_route->next;
    }
    // Free memory for attempts associated with the removed route
    struct attempt *current_attempt = curr_route->attempts;
    while (current_attempt != NULL) {
        struct attempt *next_attempt = current_attempt->next;
        free(current_attempt);
        current_attempt = next_attempt;
    }
    // Free memory for the removed route
    free(curr_route);
    logbook->counter--;
    printf("Removed route '%s' from logbook\n", user_enter_route);
}

// Remove an attempt
void delete_attempt(struct logbook *logbook) {
    char climber[MAX_STR_LEN];
    scan_string(climber);
    int num_delete = 0;
    // Traverse the logbook
    for (struct route *route = logbook->routes; route != NULL;
    route = route->next) {
        struct attempt **current = &(route->attempts);
        while (*current != NULL) {
            if (strcmp((*current)->climber, climber) == 0) {
                struct attempt *to_delete = *current;
                *current = (*current)->next;
                free(to_delete);
                num_delete++;
            } else {
                current = &((*current)->next);
            }
        }
    }
    // Check if any attempts were deleted
    if (num_delete == 0) {
        printf("ERROR: %s has not logged any attempts\n", climber);
    } else {
        printf("Deleted %d attempt(s) logged by %s\n", 
        num_delete, climber);
    }
}

// Duplicate attempts
void duplicate(struct logbook *logbook) {
    char climber_1[MAX_STR_LEN];
    char climber_2[MAX_STR_LEN];
    scan_string(climber_1);
    scan_string(climber_2);
    // Check if the two climbers are the same
    if (strcmp(climber_1, climber_2) == 0) {
        printf("ERROR: Cannot duplicate attempts made by the same climber\n");
        return;
    }
    int duplicated = FALSE;
    for (struct route *route = logbook->routes; route != NULL; 
    route = route->next) {
        enum attempt_type climber_1_type = INVALID_TYPE;
        for (struct attempt *curr_attempt = route->attempts; curr_attempt != NULL; 
        curr_attempt = curr_attempt->next) {
            if (strcmp(curr_attempt->climber, climber_1) == 0) {
                climber_1_type = curr_attempt->type;
            }
            if (strcmp(curr_attempt->climber, climber_2) == 0) {
                enum attempt_type duplicate_type = curr_attempt->type;
                if (duplicate_type == FIRST_GO && 
                    climber_1_type != INVALID_TYPE) {
                    duplicate_type = SUCCESS;
                }
                struct attempt *new_attempt = create_attempt(climber_1,
                     duplicate_type, curr_attempt->rating);
                new_attempt->next = route->attempts;
                route->attempts = new_attempt;
                printf("Logged attempt of '%s' by %s\n", route->name, 
                    climber_1);
                duplicated = TRUE;
            }
        }
    }
    // Check if any attempts were duplicated
    if (duplicated == FALSE) {
        printf("ERROR: %s has not logged any attempts\n", climber_2);
    }
}



////////////////////////////////////////////////////////////////////////////////


// Prints the 1511 Climb usage instructions,
// displaying the different commands and their arguments.
//
// Parameters:
//      None
// 
// Returns: 
//      None
void print_usage(void) {
    printf(
        "=====================[          Climb     ]=====================\n"
        "      ===============[     Usage Info     ]===============      \n"
        "  r [name] [difficulty] [length]                                \n"
        "    Append a climbing route to the end of the logbook           \n"
        "  p                                                             \n"
        "    Print out all the climbing routes in the logbook            \n"
        "                                                                \n"
        "  f [difficulty_min] [difficulty_max]                           \n"
        "    Filter routes by difficulty                                 \n"
        "  i [name] [difficulty] [length] [route_to_insert_before]       \n"
        "    Insert a climbing route into the logbook before             \n"
        "    `route_to_insert_before`                                    \n"
        "  l [route_1] [route_2] [length_change]                         \n"
        "    Change length of all routes between `route_1` and `route_2` \n"
        "  s [route_1] [route_2]                                         \n"
        "    Swap 2 routes                                               \n"
        "                                                                \n"
        "  a [climber] [attempt_type] [rating] [route]                   \n"
        "    Add an attempt of `route` by `climber`                      \n"
        "  P                                                             \n"
        "    Print out all the routes and attempts in the logbook        \n"
        "  R [route]                                                     \n"
        "    Remove a route and all associated attempts                  \n"
        "  D [climber]                                                   \n"
        "    Delete all attempts by `climber`                            \n"
        "  d [climber_1] [climber_2]                                     \n"
        "    Duplicate all of `climber_2`s attempts for `climber_1`      \n"
        "                                                                \n"
        "  c [route_1] [route_2]                                         \n"
        "    Combine 2 routes                                            \n"
        "  L [route]                                                     \n"
        "    Print the climber leaderboard for `route`                   \n"
        "                                                                \n"
        "  ?                                                             \n"
        "    Show help                                                   \n"
        "================================================================\n"
    );
}

// Prints a single route, displaying its position in the logbook, name, grade 
// and length. 
//
// Parameters:
//      position    - The position of the route in the logbook (the first 
//                        route will be route_number 1, second will be 2, etc)
//      route       - A pointer to the struct route to be printed
//
// Returns:   
//      None
void print_one_route(int position, struct route *route) {
    int line_length = MAX_STR_LEN + 2;
    printf("/--------- \\/ ---------\\\n");

    char position_str[MAX_STR_LEN];
    sprintf(position_str, "Route #%d", position);
    int padding = line_length - strlen(position_str);
    printf(
        "|%*s%s%*s|\n", padding / 2, "", position_str, (padding + 1) / 2, ""
    );

    padding = line_length - strlen(route->name);
    printf("|%*s%s%*s|\n", padding / 2, "", route->name, (padding + 1) / 2, "");

    printf("|    Difficulty: %2d    |\n", route->difficulty);
    printf("|    Length (m): %2d    |\n", route->length);
    printf("\\--------- || ---------/\n");
}

// Scans a string into the provided char array, placing a '\0' at the end
// 
// Parameters:
//      string      - a char array of length MAX_STR_LEN to store the string 
//                    that is scanned in
// 
// Returns: 
//      None
// 
// Usage: 
// ```
//      char string[MAX_STR_LEN];
//      scan_string(string);
// ```
void scan_string(char string[MAX_STR_LEN]) {
    scan_token(string, MAX_STR_LEN);
}

// Scans a string and converts it to an enum attempt_type.
//
// Parameters: 
//      None
// 
// Returns:
//      The corresponding attempt_type, if the string was valid,
//      Otherwise, returns INVALID_TYPE.
// 
// Usage: 
// ```
//      enum attempt_type type = scan_attempt_type();
// ```
enum attempt_type scan_attempt_type(void) {
    char type[MAX_STR_LEN];
    scan_token(type, MAX_STR_LEN);
    return string_to_type(type);
}

// Prints a single attempt, displaying the climber name, attempt type and rating
//
// Parameters:
//      climber     - The name of the climber who attempted to climb a route
//      type        - The attempt type that was made (should be either 
//                    FIRST_GO, SUCCESS, or FAIL)
//      rating      - The star rating that the climber gave the route
// 
// Returns: 
//      None
void print_one_attempt(
    char climber[MAX_STR_LEN], 
    enum attempt_type type, 
    int rating
) {
    char type_string[MAX_STR_LEN];
    type_to_string(type_string, type);
    printf("        --> %s\n", type_string);
    printf("            Climber: %s\n", climber);
    printf("            Rating:  ");
    for (int i = 0; i < rating; i++) {
        printf("*");
    }
    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////  

// Scans a single word into `buffer`, ignoring whitespace
// 
// Parameters: 
//      buffer      - the string to store the scanned word
//      buffer_size - the maximum size of string that can be scanned (including 
//                    the null terminator)
// Returns: 
//      The number of chars scanned successfully
int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // Consume all leading whitespace
    scanf(" ");

    // Scan in characters until whitespace
    while (i < buffer_size - 1
        && (num_scanned = scanf("%c", &c)) == 1 
        && !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

// Converts a string to the corresponding enum attempt_type
//
// Parameters: 
//      type_str    - The string to be converted
// 
// Returns: 
//      The matching enum attempt_type, or `INVALID_TYPE` if `type_str` matches 
//      no valid attempt type
enum attempt_type string_to_type(char *type_str) {
    int len = MAX_STR_LEN;

    if (strncasecmp(type_str, "first_go", len) == 0) {
        return FIRST_GO;
    }
    if (strncasecmp(type_str, "success", len) == 0) {
        return SUCCESS;
    } 
    if (strncasecmp(type_str, "fail", len) == 0) {
        return FAIL;
    } 

    return INVALID_TYPE;
}

// Converts an enum attempt_type to a string
// 
// Parameters:
//      buf         - The string to store the attempt type
//      type        - The enum attempt_type to be converted
// 
// Returns:
//      None
void type_to_string(char *buf, enum attempt_type type) {
    if (type == FIRST_GO) {
        strcpy(buf, "FIRST GO");
    } else if (type == SUCCESS) {
        strcpy(buf, "SUCCESS");
    } else if (type == FAIL) {
        strcpy(buf, "FAIL");
    } else {
        strcpy(buf, "INVALID");
    }
}
