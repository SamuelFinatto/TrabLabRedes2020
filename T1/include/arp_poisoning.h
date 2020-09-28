#ifndef ARP_POISONING_H_
# define ARP_POISONING_H_

# define FALSE				0
# define TRUE				!FALSE
# define SPOOFED_PACKET_SEND_DELAY	1

void		usage(const char[const]);
uint8_t    *get_hardware_address(const char * const);

#endif
