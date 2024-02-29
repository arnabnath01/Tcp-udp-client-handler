
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
    // Create UDP socket
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    // Input string from user
    char user_input[MAX_LEN];
    printf("Enter a string to check if it is a palindrome: ");
    fgets(user_input, sizeof(user_input), stdin);
    // Remove trailing newline character
    user_input[strcspn(user_input, "\n")] = 0;

    // Send message via UDP
    if (sendto(udp_sock, user_input, strlen(user_input), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("UDP sendto failed");
        exit(EXIT_FAILURE);
    }

    // Receive result from UDP server
    char udp_buffer[MAX_LEN];
    struct sockaddr_in server_udp_addr;
    socklen_t addr_len = sizeof(server_udp_addr);
    int udp_bytes_received = recvfrom(udp_sock, udp_buffer, MAX_LEN, 0, (struct sockaddr *)&server_udp_addr, &addr_len);
    if (udp_bytes_received < 0)
    {
        perror("UDP recvfrom failed");
        exit(EXIT_FAILURE);
    }
    udp_buffer[udp_bytes_received] = '\0';
    printf("UDP Server Response: %s", udp_buffer);

    // Close UDP socket
    close(udp_sock);

    return 0;
}
