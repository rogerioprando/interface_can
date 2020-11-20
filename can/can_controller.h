#ifndef _CAN_CONTROLLER_H_
#define _CAN_CONTROLLER_H_

#include "can_protocol.h"

struct can_interface;

struct can_interface *can_create_interface(char *ifname, int bitrate);

void can_interface_init(struct can_interface *cif, char *ifname, int bitrate);
int can_interface_start(struct can_interface *cif);
int can_interface_stop(struct can_interface *cif);
int can_create_socket(struct can_interface *cif);

int can_read(struct can_frame *can_msg, int socket);
int can_write(struct can_frame *can_msgn, int socket);

#endif
