#include <stdio.h>
#include <winsock2.h>  
#include <ws2tcpip.h>  

#pragma comment(lib, "ws2_32.lib")  

int main()
{
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    int number, factorial;
    int servlen = sizeof(servaddr);
    int recv_len;

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

        // Get user input
        printf("Enter a number to calculate its factorial: ");
        scanf("%d", &number);

        // Send the number to the server
        if (sendto(sockfd, (char*)&number, sizeof(number), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR)
        {
            printf("sendto failed with error: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        printf("Number sent to server.\n");

        // Receive the factorial result from the server
        recv_len = recvfrom(sockfd, (char*)&factorial, sizeof(factorial), 0, NULL, NULL);
        if (recv_len == SOCKET_ERROR)
        {
            printf("recvfrom failed with error: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        printf("The factorial of %d is: %d\n", number, factorial);
    }

    // Close socket and cleanup
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
