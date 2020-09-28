#ifndef PACKETS_H_
# define PACKETS_H_
# include <stdint.h>
# include "network.h"
# define ETH_HEADER_LENGTH 14
# define ARP_HEADER_LENGTH 28

typedef struct	s_ethernet_packet
{
  uint8_t	destination_mac_address[HARDWARE_LENGTH];
  uint8_t	source_mac_address[HARDWARE_LENGTH];
  uint16_t	ether_type;
}		t_ethernet_packet;

typedef struct	s_arp_packet
{
  uint16_t	hardware_type;
  uint16_t	protocol_type;
  uint8_t	hardware_len;
  uint8_t	protocol_len;
  uint16_t	opcode;
  uint8_t	sender_mac[HARDWARE_LENGTH];
  uint8_t	sender_ip[IP_LENGTH];
  uint8_t	target_mac[HARDWARE_LENGTH];
  uint8_t	target_ip[IP_LENGTH];
}		t_arp_packet;

t_arp_packet	*create_arp_packet(const uint16_t opcode,
				   const uint8_t *my_mac_address, const char *spoofed_ip_source,
				   const uint8_t *destination_mac_address, const char *destination_ip);
t_ethernet_packet	*create_ethernet_packet(const uint8_t *src_mac, const uint8_t *dest_mac, const t_arp_packet *arp_packet);

#endif
