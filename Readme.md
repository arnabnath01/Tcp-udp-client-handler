# UDP and TCP Palindrome Check Server

This repository contains a simple UDP and TCP server implementation in C that checks whether a received string is a palindrome. The server uses the `select` system call to handle both UDP and TCP connections iteratively.

## Files

- `server.c`: Iterative server that listens on both UDP and TCP sockets, checks if a received string is a palindrome, and sends a response.
- `udp_client.c`: UDP client that sends a string to the server for palindrome check.
- `tcp_client.c`: TCP client that connects to the server and sends a string for palindrome check.

## Instructions

1. Compile the programs:

    ```bash
    gcc server.c -o server
    gcc udp_client.c -o udp_client
    gcc tcp_client.c -o tcp_client
    ```

2. Run the server in the background:

    ```bash
    ./server &
    ```
![image](https://github.com/arnabnath01/Tcp-udp-client-handler/assets/98618274/52ea9f1d-0e47-46ee-a919-205115c27edb)

    

3. Run the UDP client:

    ```bash
    ./udp_client
    ```
![image](https://github.com/arnabnath01/Tcp-udp-client-handler/assets/98618274/7a5cb99b-17fb-4aa4-abf1-860f2e8730d7)

4. Run the TCP client:

    ```bash
    ./tcp_client
    ```
![image](https://github.com/arnabnath01/Tcp-udp-client-handler/assets/98618274/6ff9d8ad-1797-4e11-949d-c422547a9bd0)

Make sure to run the server in the background before running the clients. Adjust the IP address and port number if needed. The provided example assumes the server and clients are running on the same machine with port 5000.

## Notes

- The server responds with a message indicating whether the received string is a palindrome or not.
- Feel free to modify the code to suit your requirements.

## License

This project is licensed under the [MIT License](LICENSE).
