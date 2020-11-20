#ifndef _CAN_PROTOCOL_H_
#define _CAN_PROTOCOL_H_

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_controller.h"

struct can_protocol {
    struct can_frame can_msg;
    int can_socket;
};

int can_recv(struct can_protocol *can_prot);
int can_send(struct can_protocol *can_prot);

#endif