#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <libsocketcan.h>

int main (int argc, char *argv[]){

    int s;
    int nbytes;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_frame frame;

    s = socket(PF_CAN,SOCK_RAW, CAN_RAW);
    if (s < 0){
        perror("socket");
        return 1;
    }

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind");
        return 1;
    }

    nbytes = read(s, &frame, sizeof(struct can_frame));

    if (nbytes < 0){
        perror("read");
        return 1;
    }

    printf("0x%03X [%d] ",frame.can_id, frame.can_dlc);
    for (int i = 0; i < frame.can_dlc; i++)
        printf("%02X ",frame.data[i]);
    printf("\r\n");

    if (close(s) < 0) {
        perror("close");
        return 1;
    }

}
