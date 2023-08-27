#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <time.h>

#include <arpa/inet.h>

#include <pthread.h>



#define PORT 8080

#define BACKLOG 5



int debugging_mode = 0; 

FILE* log_file = NULL;



// structure to hold usage statistics

struct ServerStats {

    int total_requests;

    double total_response_time;

    int num_threads;

};



// Function to handle HTTP requests

void handle_http_request(int client_socket) {

    char buffer[1024] = {0};

    char response[1024] = {0};

    int valread = read(client_socket, buffer, sizeof(buffer));

    if (valread <= 0) {

        perror("Read failed");

        close(client_socket);

        return;

    }



    // Accept the HTTP request 

    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello, World!", (int)strlen("Hello, World!"));

    write(client_socket, response, strlen(response));



    close(client_socket);

}



// Function to handle client connections

void* handle_client(void* arg) {

    int client_socket = *(int*)arg;



    if (debugging_mode) {

        printf("Handling client connection on socket %d\n", client_socket);

    }



    // response timer

    struct timespec start_time;

    clock_gettime(CLOCK_MONOTONIC, &start_time);



    

    handle_http_request(client_socket);



    // Calc response time

    struct timespec end_time;

    clock_gettime(CLOCK_MONOTONIC, &end_time);



    double response_time = (end_time.tv_sec - start_time.tv_sec) +

                           (end_time.tv_nsec - start_time.tv_nsec) / 1e9;



    // Update usage statistics

    struct ServerStats* stats = (struct ServerStats*)arg;

    stats->total_requests++;

    stats->total_response_time += response_time;



    if (debugging_mode) {

        printf("Closing client socket %d\n", client_socket);

    }

}

void write_log(const char* message) {

    if (log_file) {

        time_t now;

        struct tm* timeinfo;

        char timestamp[20];

        time(&now);

        timeinfo = localtime(&now);

        strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", timeinfo);

        fprintf(log_file, "%s %s\n", timestamp, message);

        fflush(log_file);

    }

}

void display_usage_statistics(const struct ServerStats* stats) {

    printf("Usage Statistics:\n");

    printf("Total Requests: %d\n", stats->total_requests);

    printf("Average Response Time: %.6f seconds\n", stats->total_response_time / stats->total_requests);

    printf("Number of Threads: %d\n", stats->num_threads);

    

}









int main(int argc, char* argv[]) {

    int server_fd, new_socket;

    struct sockaddr_in address;

    int opt = 1;

    int addrlen = sizeof(address);



    int num_threads = 4; // Default number of threads in the pool

    int enable_stats=0; //Flag to  enable  display stats



    // Mode loop

    for (int i = 1; i < argc; ++i) {

        if (strcmp(argv[i], "--debug") == 0) {

            debugging_mode = 1;

            printf("Debugging mode enabled.\n");

        } 

        else if (strcmp(argv[i], "--threads") == 0 && i + 1 < argc) {

            num_threads = atoi(argv[i + 1]);

            if (num_threads <= 0) {

                printf("Invalid number of threads. Using the default value.\n");

            } else {

                printf("Number of threads set to %d.\n", num_threads);

            }

        }

        else if (strcmp(argv[i], "--logfile") == 0 && i + 1 < argc) {

            log_file = fopen(argv[i + 1], "a");

            if (!log_file) {

                perror("Log file could not be opened");

            } else {

                printf("Logging requests to: %s\n", argv[i + 1]);

            }

        }

        else if (strcmp(argv[i], "--stats") == 0) {

            enable_stats = 1;

        }

    

        

    }





    printf("Server is listening on port %d...\n", PORT);



    // Initialize server statistics

    struct ServerStats stats = {0, 0.0, num_threads};



    // Create worker threads in the thread pool

    pthread_t thread_pool[num_threads];

    for (int i = 0; i < num_threads; ++i) {

        if (pthread_create(&thread_pool[i], NULL, handle_client, &stats) != 0) {

            perror("Thread creation failed");

            exit(EXIT_FAILURE);

        }

    }

    



    while (1) {

        

         if (enable_stats) {

            display_usage_statistics(&stats); 

            enable_stats = 0; 

        }

        handle_http_request(new_socket);

    }



    return 0;

}

