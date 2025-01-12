#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int
main (int argc, char *argv[])
{
  int server_fd, new_socket;
  char buffer[1024] = { 0 };
  char ip_buffer[1024] = { 0 };
  char port[5] = "2342";
  struct sockaddr_in address;
  int addrlen = sizeof (address);

  if (argc > 1)
    strcpy (port, argv[1]);

  // Create socket
  server_fd = socket (AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0)
  {
    perror ("socket");
    exit (EXIT_FAILURE);
  }

  // Define address and port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons (atoi (port));

  // Bind the socket
  if (bind (server_fd, (struct sockaddr *) &address, sizeof (address)) < 0)
  {
    perror ("bind");
    close (server_fd);
    exit (EXIT_FAILURE);
  }

  // Listen for connections
  if (listen (server_fd, 1) < 0)
  {
    perror ("listen");
    close (server_fd);
    exit (EXIT_FAILURE);
  }

  printf ("Server listening on port %s...\n", port);

  // Accept a connection
  new_socket =
    accept (server_fd, (struct sockaddr *) &address, (socklen_t *) & addrlen);
  if (new_socket < 0)
  {
    perror ("accept");
    close (server_fd);
    exit (EXIT_FAILURE);
  }
  while (1)
  {
    // Use getsockname to get the bound address
    if (getsockname
        (new_socket, (struct sockaddr *) &address,
         (unsigned *) &addrlen) == -1)
    {
      perror ("getsockname");
      close (new_socket);
      exit (EXIT_FAILURE);
    }

    // Convert the IP address to a human-readable string
    if (inet_ntop (AF_INET, &address.sin_addr, ip_buffer, sizeof (ip_buffer))
        == NULL)
    {
      perror ("inet_ntop");
      close (new_socket);
      exit (EXIT_FAILURE);
    }

    memset (buffer, 0, 1024);
    read (new_socket, buffer, 1024);
    printf ("%s:%d> %s", ip_buffer, ntohs (address.sin_port), buffer);
  }

  // Close sockets
  close (server_fd);
  close (new_socket);

  return 0;
}
