#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "./hatwithc.h"

#define PORT 2342

int
main (void)
{
  int sock = 0;
  char buffer[1024] = { 0 };
  char hostname[1024] = { 0 };
  char ip_buffer[] = "192.168.75.101";
  struct sockaddr_in serv_addr;

  // Get the machine hostname
  if (gethostname (hostname, sizeof (hostname)) < 0)
  {
    perror ("gethostname");
    return EXIT_FAILURE;
  }

  // Create socket
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror ("socket");
    exit (EXIT_FAILURE);
  }

  // Define server address
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (PORT);

  // Convert address to binary form and connect
  if (inet_pton (AF_INET, ip_buffer, &serv_addr.sin_addr) <= 0)
  {
    perror ("inet_pton");
    close (sock);
    exit (EXIT_FAILURE);
  }

  if (connect (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
  {
    perror ("connect");
    close (sock);
    exit (EXIT_FAILURE);
  }

  while (1)
  {
    printf ("%s:%d> ", ip_buffer, PORT);
    fgets (buffer, 1024, stdin);
    if (strcmp (buffer, "exit") == 0)
    {
      printf ("Bye!\n");
      break;
    }
    else if (strlen (buffer) < 1)
      fprintf (stderr, "Empty message!\n");
    send (sock, buffer, strlen (buffer), 0);
    memset (buffer, 0, 1024);
  }

  // Close socket
  close (sock);

  return 0;
}
