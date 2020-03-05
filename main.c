#include <stdio.h>
#include <linux/input.h>
#include <sys/file.h>
#include <unistd.h>

#if 0 //use poll
#include <poll.h>

char *path = "/dev/input/by-id/usb-Logitech_USB_Receiver-event-kbd";
int main(int argc, char *argv[])
{
    struct input_event event;
    int ret = 0;
    int nfds = 1;//one fd poll
    int timeout = 0;//0:donot timeout

    struct pollfd pfd;
    
 
    int fd;

    fd = open(path, O_RDONLY);
  
    if (fd > 0){


        pfd.fd = fd;
        pfd.events = POLLIN;

        while(1){
            ret = poll(&pfd, nfds, timeout);

            if(pfd.events & POLLIN) {
                if(read(fd, &event, sizeof(event))){
                    if(event.type != EV_SYN){
                        printf("event time %ld.%ld\ttype:%d\tcode:%d\tvalue:%d\n", 
                            event.time.tv_sec, 
                            event.time.tv_usec,
                            event.type,
                            event.code,
                            event.value
                            );
                    }
                }
            }
           

        }
    }
    printf("hello world\n");
    
    return 0;
}


#else //use select


#include <sys/select.h>
#include <error.h>

char *path = "/dev/input/by-id/usb-Logitech_USB_Receiver-event-kbd";
int main(int argc, char *argv[])
{
    struct input_event event;
    int ret = 0;
    fd_set rfds;

    struct timeval timeout;
    int fd;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    fd = open(path, O_RDONLY);
    
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    if (fd > 0){
     
        while(1){
            
            ret = select(fd+1, &rfds, NULL, NULL, &timeout);
            if(ret < 0) {
                perror("select error");
            } else if(ret == 0){
                perror("select timeout");
            } else if(FD_ISSET(fd, &rfds)){

                if(read(fd, &event, sizeof(event))){
                    if(event.type != EV_SYN){
                        printf("event time %ld.%ld\ttype:%d\tcode:%d\tvalue:%d\n", 
                            event.time.tv_sec, 
                            event.time.tv_usec,
                            event.type,
                            event.code,
                            event.value
                            );
                    }
                }
              
            }
            printf("hello key\n");
           

        }
    }

    
    return 0;
}

#endif