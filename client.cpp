#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int main(int argc, char **argv)
{
    //This is a slight change
    char *message = argv[1];
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        std::cerr << "Socket failed to initialise!" << std::endl;
        exit(1);
    }
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0)
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        exit(1);
    }
    write(fd, message, strlen(message));

    char rbuffer[64] = {};
    ssize_t n = read(fd, rbuffer, sizeof(rbuffer) - 1);
    if (n < 0)
    {
        std::cerr << "Failed to read server message into buffer!" << std::endl;
        exit(1);
    }
    std::cout << "Server says " << rbuffer << std::endl;
    close(fd);
    //Oh look another :)
    if (23 > 56){
        return true;
    }
}