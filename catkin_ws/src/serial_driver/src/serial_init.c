#include <stdio.h>          //puts
#include <string.h>         //strlen
#include <stdlib.h>
#include <unistd.h>         /*Unix 标准函数定义*/
#include <sys/socket.h>
#include <arpa/inet.h>      //inet_addr
#include <unistd.h>         //write
#include <pthread.h>        //for threading , link with lpthread
#include <sys/time.h>
#include <pthread.h>


#include <assert.h>

//for tty
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int serial_init()
{
    int fd;
    fd = open("/dev/ttyUSB0",O_RDWR );
    if(-1 == fd){
        close(fd);
        printf("UBUNTU open /dev/ttyUSB0 error!\n");
        exit(0);
    }
    printf("Mcu_Recv open /dev/ttyUSB0 success!\n");
    sleep(1);

    struct termios opt;
    tcgetattr(fd, &opt);
    cfsetispeed(&opt, B115200);
    cfsetospeed(&opt, B115200);
    if(tcsetattr(fd, TCSANOW, &opt) != 0 )
    {
        perror("tcsetattr error");
        return -1;
    }

    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_cflag &= ~CSTOPB;
    opt.c_cflag &= ~PARENB;
    opt.c_cflag &= ~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);

    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    opt.c_oflag &= ~OPOST;
    opt.c_oflag &= ~(ONLCR | OCRNL);    //添加的

    opt.c_iflag &= ~(ICRNL | INLCR);
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);    //添加的

    opt.c_cc[VTIME] = 1;
    opt.c_cc[VMIN] = 0;

    tcflush(fd, TCIOFLUSH);

    printf("mcu_recv ttyUSB0 configure complete\n");

    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        perror("serial error");
        return -1;
    }
    return fd;
}




