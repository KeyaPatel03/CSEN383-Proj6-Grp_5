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

    srand(time(NULL) ^ (getpid() << 16)); // Unique seed

    gettimeofday(&start_tv, NULL);

    while (1) {
        gettimeofday(&current_tv, NULL);
        double elapsed_time = (current_tv.tv_sec - start_tv.tv_sec) +
                              (current_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

        if (elapsed_time >= 30.0) break;

        if (child_id == 4) {
            // Child 5: terminal interaction
            printf("Child %d: Enter a message: ", child_id + 1);
            fflush(stdout); // Ensure prompt appears immediately
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                break;
            }

            buffer[strcspn(buffer, "\n")] = 0;

            gettimeofday(&current_tv, NULL);
            elapsed_time = (current_tv.tv_sec - start_tv.tv_sec) +
                           (current_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

            if (elapsed_time >= 30.0) break;

            int minutes = (int)(elapsed_time / 60);
            int seconds = (int)(elapsed_time) % 60;
            int milliseconds = (int)((elapsed_time - (int)elapsed_time) * 1000);

            snprintf(message, sizeof(message), "%d:%02d.%03d: Child %d: %s\n",
                     minutes, seconds, milliseconds, child_id + 1, buffer);
        } else {
            int sleep_time = rand() % 3;

            if (elapsed_time + sleep_time >= 30.0) break;
            usleep(sleep_time * 1000000);

            gettimeofday(&current_tv, NULL);
            elapsed_time = (current_tv.tv_sec - start_tv.tv_sec) +
                           (current_tv.tv_usec - start_tv.tv_usec) / 1000000.0;

            if (elapsed_time >= 30.0) break;

            int minutes = (int)(elapsed_time / 60);
            int seconds = (int)(elapsed_time) % 60;
            int milliseconds = (int)((elapsed_time - (int)elapsed_time) * 1000);

            snprintf(message, sizeof(message), "%d:%02d.%03d: Child %d message %d\n",
                     minutes, seconds, milliseconds, child_id + 1, ++msg_count);
        }

        write(write_fd, message, strlen(message));
    }

    close(write_fd);
}
