#include <stdio.h>
#include <winsock2.h> 
#include <ws2tcpip.h> 

#pragma comment(lib, "ws2_32.lib") 

int factorial(int n);

int main()
{
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int recv_len;
    int cliaddr_len = sizeof(cliaddr);
    int number, result;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    else
    {
        printf("UDP Socket is created\n");

        // Configure server address
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(43451);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        // Bind the socket
        if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        {
            printf("Bind failed with error: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        else
        {
            printf("Bind successful. Waiting for data...\n");

            // Receive data from client
            recv_len = recvfrom(sockfd, (char*)&number, sizeof(number), 0, (struct sockaddr*)&cliaddr, &cliaddr_len);
            if (recv_len == SOCKET_ERROR)
            {
                printf("recvfrom failed with error: %d\n", WSAGetLastError());
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }

            printf("Received number %d from client.\n", number);

            // Calculate factorial
            result = factorial(number);

            // Send the result back to the client
            if (sendto(sockfd, (char*)&result, sizeof(result), 0, (struct sockaddr*)&cliaddr, cliaddr_len) == SOCKET_ERROR)
            {
                printf("sendto failed with error: %d\n", WSAGetLastError());
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }

            printf("Sent factorial %d back to client.\n", result);
        }
    }

    // Close socket and cleanup
    closesocket(sockfd);
    WSACleanup();
    return 0;
}

int factorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}
