#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

static void read_into_buffer(int conn_fd)
{
    char rbuffer[64] = {};
    ssize_t n = read(conn_fd, rbuffer, sizeof(rbuffer) - 1);
    if (n < 0)
    {
        std::cout << "Failed to read into buffer!" << std::endl;
        return;
    }
    std::cout << "Client said " << rbuffer << std::endl;
    char wbuffer[] = "Hello back";
    write(conn_fd, wbuffer, strlen(wbuffer));
}

static int32_t read_full(int fd, char *buff, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = read(fd, buff, n);
        if (rv < 0)
        {
            return -1;
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buff, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = write(fd, buff, n);
        if (rv <= 0)
        {
            return -1; // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buff += rv;
    }
    return 0;
}

int main()
{
    struct sockaddr_in addr = {};
    addr.sin_family = PF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);

    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        std::cerr << "Socket failed to initialise!" << std::endl;
        exit(1);
    }
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));

    if (rv < 0)
    {
        std::cerr << "Failed to bind!" << std::endl;
        exit(1);
    }
    int listening = listen(fd, 5);
    if (listening < 0)
    {
        std::cerr << "Failed to listen!" << std::endl;
        exit(1);
    }
    std::cout << "Server running" << std::endl;
    while (true)
    {

        struct sockaddr_in client = {};
        socklen_t client_size = sizeof(client);
        int conn_fd = accept(fd, (struct sockaddr *)&client, &client_size);
        if (conn_fd < 0)
        {
            std::cout << "Failed to connect to client!" << std::endl;
            continue;
        }
        read_into_buffer(conn_fd);
    }
}