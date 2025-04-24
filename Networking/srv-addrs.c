#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h> 
#include <net/if_dl.h>

int
main (int argc, char *argv[])
{
  int port = 2350;
  int srv_sock;
  struct sockaddr_in srv_sockaddr;
  struct ifaddrs *all_addrs, *addr;

  srv_sockaddr.sin_family = AF_INET;
  srv_sockaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  srv_sockaddr.sin_port = htons (port);

  srv_sock = socket (AF_INET, SOCK_STREAM, 0);

  bind (srv_sock, (struct sockaddr *) &srv_sockaddr, sizeof(srv_sockaddr));

  // enumerate all interfaces
  getifaddrs (&all_addrs);
  for (addr = all_addrs; addr != NULL; addr = addr->ifa_next) {
      if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_LINK) {
	  struct ifreq ifr;

	  // get the IP address of the interface
	  ifr.ifr_addr.sa_family = AF_INET;
	  strncpy (ifr.ifr_name, addr->ifa_name, IFNAMSIZ - 1);
	  ioctl (srv_sock, SIOCGIFADDR, &ifr);

	  // we now have the interface name and its IP address
	  printf ("%s %s\n", addr->ifa_name,
		  inet_ntoa (((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
      }
  }
  freeifaddrs (all_addrs);
}
