#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <poll.h>

#include "../can/can_controller.h"
#include "../can/can_protocol.h"

int main (int argc, char *argv[]){

    int c, can_socket;
    struct can_interface *cif;
    struct can_protocol can_prot = {};

    // will set up remotely
    char *ifname = "vcan0";
    int bitrate = 250000;

    struct pollfd poll_events[1] = { 0 };
    int poll_res, timeout, nbytes;
    int wbytes;
    
    cif = can_create_interface(ifname, 250000);
    
    c = can_interface_start(cif);
    if (c < 0){
        perror("error while start can");
        exit(-1);
    }

    //can_socket = can_create_socket(cif);
    can_prot.can_socket = can_create_socket(cif);

    poll_events[0].fd = can_prot.can_socket;
    poll_events[0].events = POLLIN;

    timeout = 1000;

    for (;;){

        poll_res = poll(poll_events, 1, timeout);

        if(poll_res < 0){
            perror("Something wrong while trying to run the poll on can");
            return -1;
        }

        if(poll_events[0].revents & POLLIN) {
            poll_events[0].revents = 0;
            
            nbytes = can_recv(&can_prot);
        }

        // echo reply
        wbytes = can_send(&can_prot);

    }
}
