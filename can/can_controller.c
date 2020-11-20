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

#include "can_controller.h"

struct can_interface {
	//char *ifname;
    char ifname[5];
    int bitrate;
    int can_socket;
    //struct can_frame cf;
    struct sockaddr_can addr;
    struct ifreq ifr;  
};

struct can_interface *can_create_interface(char *ifname, int bitrate){

    struct can_interface *cif = calloc(1, sizeof(struct can_interface));
    
    if(!cif){
        return NULL;
    }

    can_interface_init(cif, ifname, bitrate);

    return cif;
}

void can_interface_init(struct can_interface *cif, char *ifname, int bitrate){

    strncpy(cif->ifname, ifname, strlen(ifname));
    cif->bitrate = bitrate;

    printf("can_prot_init: ifname [%s]\n", cif->ifname);
    printf("can_prot_init: bitrate [%d]\n", cif->bitrate);
}

int can_interface_start(struct can_interface *cif){
    int c;

    c = can_set_bitrate(cif->ifname, cif->bitrate);
    if(c < 0){
        perror("error while set bitrate");
        return -1;
    }

    c = can_do_start(cif->ifname);
    if(c < 0){
        perror("error while start can interface");
        return -1;
    }

    #ifdef debug
    printf("can interface started at %s\n", cif->ifname);
    #endif

    return c;
}

int can_interface_stop(struct can_interface *cif){
    int c;

    c = can_do_stop(cif->ifname);
    if(c < 0){
        perror("error while stop can inteface");
    }

    return c;
}

int can_create_socket(struct can_interface *cif){

    int rc;
    int on = 1;

    cif->can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(cif->can_socket  < 0) {
        perror("Error while opening socket");
        return - 1;
    }
    
    // test - set socket to be nonblocking
    rc = ioctl(cif->can_socket, FIONBIO, (char *)&on);
    if (rc < 0){
        perror("ioctl() failed");
        close(cif->can_socket);
        return -1;
    }

    strcpy(cif->ifr.ifr_name, cif->ifname);
    ioctl(cif->can_socket, SIOCGIFINDEX, &cif->ifr);     // SIOCGIFINDEX: Retrieve the interface index of the interface into

    cif->addr.can_family = AF_CAN;
    cif->addr.can_ifindex = cif->ifr.ifr_ifindex;

    #ifdef debug
    printf("%s at index %d\n", cif->ifname, cif->ifr.ifr_ifindex);
    #endif

    rc = bind(cif->can_socket, (struct sockaddr *)&cif->addr, sizeof(cif->addr));
    if (rc < 0){
        perror("bind() failed");
        close(cif->can_socket);
        return -1;
    }

    return cif->can_socket;

}

int can_read(struct can_frame *can_msg, int socket){
    
    int i, nbytes;
    //struct can_frame cf;
  
    nbytes = read(socket, can_msg, sizeof(struct can_frame));

    if (nbytes < 0){
        perror("can raw socket read");
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "read: incomplete CAN frame\n");
        return -1;
    }
    
    printf("can_id: %X [%d] data: ", can_msg->can_id);
    for (i = 0; i < can_msg->can_dlc; i++){
        printf("%02X ", can_msg->data[i]);
    }

    printf("\n");
}

int can_write(struct can_frame *can_msg, int socket){
    
    int nbytes;
  
    nbytes = write(socket, can_msg, sizeof(struct can_frame));
    
    if (nbytes < 0){
        perror("can raw socket write");
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "read: incomplete CAN frame\n");
        return -1;
    }

    return nbytes;
}
