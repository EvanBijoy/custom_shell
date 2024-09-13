#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096
#define HOST "man.he.net"
#define PORT 80

void iManCommand(char *args[]) 
{
    struct sockaddr_in server_addr;
    struct hostent *server;
    int sockfd;
    char buffer[BUFFER_SIZE];

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Could not create socket");
        return;
    }

    // Get the host by name
    server = gethostbyname(HOST);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: No such host\n");
        close(sockfd);
        return;
    }

    // Zero out the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    // Build the GET request
    snprintf(buffer, sizeof(buffer), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", args[1], HOST);

    // Send the request
    if (send(sockfd, buffer, strlen(buffer), 0) < 0)
    {
        perror("Send failed");
        close(sockfd);
        return;
    }

    // Receive the response and handle it
    int received_bytes;
    while ((received_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[received_bytes] = '\0';  // Null-terminate the buffer
        
        int i = 0;
        bool inside_tag = false; // Track whether we're inside an HTML tag

        while (i < received_bytes)
        {
            if (buffer[i] == '<')
            {
                inside_tag = true; // Entering an HTML tag
            }

            if (!inside_tag)
            {
                printf("%c", buffer[i]); // Only print characters outside of tags
            }

            if (buffer[i] == '>')
            {
                inside_tag = false; // Exiting an HTML tag
            }

            i++;
        }
    }

    if (received_bytes < 0)
    {
        perror("Receive failed");
    }

    // Close the socket
    close(sockfd);
}
