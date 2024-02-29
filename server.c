



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_LEN 100

// Function to check if a string is palindrome
int isPalindrome(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            return 0; // Not a palindrome
        }
    }
    return 1; // Palindrome
}

int main()
{
    // Create TCP socket
    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock < 0)
    {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structures
    struct sockaddr_in tcp_server_addr, udp_server_addr;
    memset(&tcp_server_addr, 0, sizeof(tcp_server_addr));
    memset(&udp_server_addr, 0, sizeof(udp_server_addr));
    tcp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_addr.s_addr = INADDR_ANY;
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_server_addr.sin_port = htons(5000); // TCP port
    udp_server_addr.sin_port = htons(5000); // UDP port

    // Bind TCP socket
    if (bind(tcp_sock, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) < 0)
    {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    // Bind UDP socket
    if (bind(udp_sock, (struct sockaddr *)&udp_server_addr, sizeof(udp_server_addr)) < 0)
    {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for TCP connections
    if (listen(tcp_sock, 5) < 0)
    {
        perror("TCP listen failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(tcp_sock, &readfds);
        FD_SET(udp_sock, &readfds);
        int max_sd = (tcp_sock > udp_sock) ? tcp_sock : udp_sock;

        // Wait for any one of the sockets to be ready
        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Check if TCP socket is ready
        if (FD_ISSET(tcp_sock, &readfds))
        {
            // Accept TCP connection
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_tcp_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &addr_len);
            if (client_tcp_sock < 0)
            {
                perror("TCP accept failed");
                continue;
            }

            // tcp client connection confirmation at server end
            printf("TCP client connected from %s\n", inet_ntoa(client_addr.sin_addr));

            char buffer[MAX_LEN];
            int bytes_received = read(client_tcp_sock, buffer, MAX_LEN - 1);
            if (bytes_received < 0)
            {
                perror("TCP read failed");
                close(client_tcp_sock);
                continue;
            }
            else if (bytes_received == 0)
            {
                printf("TCP client disconnected\n");
                close(client_tcp_sock);
                continue;
            }
            buffer[bytes_received] = '\0';

            // Check if the received string is a palindrome
            int is_palindrome = isPalindrome(buffer);

            // Send result back to client
            if (is_palindrome)
            {
                write(client_tcp_sock, "Palindrome\n", 11);
                printf("TCP Server Response: Palindrome\n");
            }
                
            else
            {
                write(client_tcp_sock, "Not Palindrome\n", 15);
                printf("TCP Server Response: Not Palindrome\n");
            }
                

            close(client_tcp_sock); // Close TCP connection
        }

        // Check if UDP socket is ready
        if (FD_ISSET(udp_sock, &readfds))
        {
            char buffer[MAX_LEN];
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);

            // Receive message from UDP client
            int bytes_received = recvfrom(udp_sock, buffer, MAX_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);
            if (bytes_received < 0)
            {
                perror("UDP receive failed");
                continue;
            }

            // tcp client connection confirmation at server end
            printf("UPD client connected andresponse sent to : %s\n", inet_ntoa(client_addr.sin_addr));

            buffer[bytes_received] = '\0';

            // Check if the received string is a palindrome
            int is_palindrome = isPalindrome(buffer);

            // Send result back to client
            if (is_palindrome)
                sendto(udp_sock, "Palindrome\n", 11, 0, (struct sockaddr *)&client_addr, addr_len);
            else
                sendto(udp_sock, "Not Palindrome\n", 15, 0, (struct sockaddr *)&client_addr, addr_len);
        }
    }

    // Close sockets
    close(tcp_sock);
    printf("TCP socket closed\n");
    close(udp_sock);
    printf("UDP socket closed\n");

    return 0;
}