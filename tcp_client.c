// ARNAB NATH
// 21CS8131
// ASSIGNMENT 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LEN 100

int main()
{
    // Create TCP socket
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0)
    {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    // Connect TCP socket to server
    if (connect(tcp_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("TCP connection failed");
        exit(EXIT_FAILURE);
    }

    // Input string from user
    char user_input[MAX_LEN];
    printf("Enter a string to check if it is a palindrome: ");
    fgets(user_input, sizeof(user_input), stdin);
    // Remove trailing newline character
    user_input[strcspn(user_input, "\n")] = 0;

    // Send message via TCP
    if (write(tcp_sock, user_input, strlen(user_input)) < 0)
    {
        perror("TCP write failed");
        exit(EXIT_FAILURE);
    }

    // Receive result from TCP server
    char tcp_buffer[MAX_LEN];
    int tcp_bytes_received = read(tcp_sock, tcp_buffer, MAX_LEN);
    if (tcp_bytes_received < 0)
    {
        perror("TCP read failed");
        exit(EXIT_FAILURE);
    }
    tcp_buffer[tcp_bytes_received] = '\0';
    printf("TCP Server Response: %s", tcp_buffer);

    // Close TCP socket
    close(tcp_sock);

    return 0;
}