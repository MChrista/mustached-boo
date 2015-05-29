/*
 * connect_tcp.c
 *
 * $Id: connect_tcp.c,v 1.2 2004/12/29 00:37:29 ralf Exp $
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "connect_tcp.h"


int
connect_tcp(const char *host, unsigned short port)
{
  struct addrinfo *result; /* ipv6 compatible struct */
  char cport[5];                /* port as char array */
  struct protoent  *ppe;        /* pointer to protocol information entry */
  struct sockaddr_in sin;       /* an Internet endpoint address		 */
  int s;                        /* socket descriptor                     */
  int retcode;

  sprintf(cport, "%u", port);   /* convert port */

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);

  if ((getaddrinfo(host, cport, NULL, &result)) == 0) {
    memcpy(&sin.sin_addr, &((struct sockaddr_in*)result->ai_addr)->sin_addr, sizeof(((struct sockaddr_in*)result->ai_addr)->sin_addr));
  } else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE ) {
    fprintf(stderr, "can't get \"%s\" host entry\n", host);
    perror("ERROR: client gethostbyname() ");
    return -1;
  } /* end if */

  ppe = getprotobyname("tcp");
  if (ppe == 0) {
    perror("ERROR: client getprotobyname(\"tcp\") ");
    return -1;
  } /* end if */

  s = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
  if (s < 0) {
    perror("ERROR: client socket() ");
    return -1;
  } /* end if */

  retcode = connect(s, (struct sockaddr *)&sin, sizeof(sin));
  if (retcode < 0) {
    perror("ERROR: client connect() ");
    return -1;
  } /* end if */

  return s;
} /* end of connect_tcp */
