#include "can_protocol.h"
#include "can_controller.h"

#include <stdio.h>

int can_recv(struct can_protocol *can_prot){
    struct can_frame *can_msg = &can_prot->can_msg;

    int nbytes = 0;
    nbytes = can_read(can_msg, can_prot->can_socket);

    return nbytes;
}

int can_send(struct can_protocol *can_prot){
    struct can_frame *can_msg = &can_prot->can_msg;

    int nbytes = 0;

    nbytes = can_write(can_msg, can_prot->can_socket);
    
    return nbytes;
}

