#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include "common.h"

void child_process(int child_id, int write_fd) {
    struct timeval start_tv, current_tv;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE]; // Separate buffer for the final message
    int msg_count = 0;

    // Seed the random number generator with a unique value
    srand(time(NULL) ^ (getpid() << 16));

    // Get the start time
    gettimeofday(&start_tv, NULL);

    while (1) {
        // Get the current time
        gettimeofday(&current_tv, NULL);

        // Calculate elapsed time in seconds
        double elapsed_time = (current_tv.tv_sec - start_tv.tv_sec) +
                             (current_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

        // // Exit after 30 seconds
        // if (elapsed_time >= 7.0) {
        //     break;
        // }

        if (child_id == 4) {
            // Child 5 interacts with the terminal
            printf("Child %d: Enter a message: ", child_id + 1);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                break;
            }

            // Remove the newline character from the input
            buffer[strcspn(buffer, "\n")] = 0;

            // Format the timestamp as minutes:seconds.milliseconds
            int minutes = (int)(elapsed_time / 60);
            int seconds = (int)(elapsed_time) % 60;
            int milliseconds = (int)((elapsed_time - (int)elapsed_time) * 1000);

            // Generate the message with the correct format
            snprintf(message, sizeof(message), "%d:%02d.%03d: Child %d: %s\n",
                     minutes, seconds, milliseconds, child_id + 1, buffer);
        } else {
            // Other children sleep for a random amount of time (0, 1, or 2 seconds)
            int sleep_time = rand() % 3; // Random sleep time: 0, 1, or 2 seconds
            usleep(sleep_time * 1000000); // Sleep for sleep_time seconds

            gettimeofday(&current_tv, NULL);

            // Calculate elapsed time since the start
            double elapsed = (current_tv.tv_sec - start_tv.tv_sec) +
                            (current_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

            // Format the timestamp as minutes:seconds.milliseconds
            int minutes = (int)(elapsed / 60);
            int seconds = (int)(elapsed) % 60;
            int milliseconds = (int)((elapsed - (int)elapsed) * 1000);

            // Generate the message with the correct format
            snprintf(message, sizeof(message), "%d:%02d.%03d: Child %d message %d\n",
                     minutes, seconds, milliseconds, child_id + 1, ++msg_count);
        }

        write(write_fd, message, strlen(message));
    }

    // Close the write end of the pipe before exiting
    close(write_fd);
}
