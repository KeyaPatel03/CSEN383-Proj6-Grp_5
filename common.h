#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 256 

void child_process(int child_id, int write_fd);

#endif 
