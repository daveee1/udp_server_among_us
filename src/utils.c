#include "headers/utils.h"

// define it in utils.c so both client and server can use it
// (avoids linker error)
int verbose = 0;

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;


/* Red error output for Server */
void print_server_error(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 0 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[SERVER ERROR] ", COLOR_RED);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);

}

void print_server_warning(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 1 || format == NULL) return;
    
    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[SERVER WARNING] ", COLOR_YELLOW);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);


    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);
}


void print_server(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 2 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[SERVER] ", COLOR_ORANGE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);
}


/* output for SERVER SHUTDOWN */
void print_server_unlocked(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 2 || format == NULL) return;

    // 2. Print prefix with colors
    printf("%s[SERVER] ", COLOR_ORANGE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

}

/* error for SERVER SHUTDOWN */
void print_server_error_unlocked(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 0 || format == NULL) return;

    // 2. Print prefix with colors
    printf("%s[SERVER ERROR] ", COLOR_RED);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);
}




/* Blue error output for Client */
void print_client(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 2 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[CLIENT] ", COLOR_LIGHT_BLUE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);

}

// to remove a client
void print_client_unlocked(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 2 || format == NULL) return;

    // 2. Print prefix with colors
    printf("%s[CLIENT] ", COLOR_LIGHT_BLUE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

}


void print_client_warning(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 1 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[CLIENT WARNING] ", COLOR_PURPLE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);
}

void print_client_error(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 0 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[CLIENT ERROR] ", COLOR_BLUE);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);
}


/*Green for analysis test*/
void print_analysis(const char *format, ...) {
    // 1. Fast Check: Skip if verbosity level isn't met
    if (verbose < 4 || format == NULL) return;

    pthread_mutex_lock(&log_mutex);

    // 2. Print prefix with colors
    printf("%s[ANALYSIS] ", COLOR_LIGHT_GREEN);

    // 3. Process formatted parameters directly
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // 4. Reset colors and newline
    printf("%s\n", COLOR_RESET);

    fflush(stdout);

    pthread_mutex_unlock(&log_mutex);
}
