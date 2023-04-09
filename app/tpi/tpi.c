#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <strings.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <sys/prctl.h>
#include <net/route.h>


#include <net/if.h>

#include <string.h>




#define B_RED(str)      "\033[1;31m" str "\033[0m"
#define B_GREEN(str)    "\033[1;32m" str "\033[0m"
#define B_YELLOW(str)   "\033[1;33m" str "\033[0m"


#define Printf(str,...)   		printf("%s(%d)[%s]: " str ,__FILE__, __LINE__,__FUNCTION__,##__VA_ARGS__)
#define ERR_Printf(str,...)   	printf(B_RED("[ERR ]")"%s(%d)[%s]: " str ,__FILE__, __LINE__,__FUNCTION__,##__VA_ARGS__)
#define INFO_Printf(str,...)   	printf(B_GREEN("[INFO]")"%s(%d)[%s]: " str ,__FILE__, __LINE__,__FUNCTION__,##__VA_ARGS__)
#define WARN_Printf(str,...)   	printf(B_YELLOW("[WARN]")"%s(%d)[%s]: " str ,__FILE__, __LINE__,__FUNCTION__,##__VA_ARGS__)



char host[64] = {0};



bool power(int arg)
{
    char cmd[128] = {0};
    if(arg==1)
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=set&type=power&node1=1&node2=1&node3=1&node4=1'",host);
    else if(0==arg)
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=set&type=power&node1=0&node2=0&node3=0&node4=0'",host);
    else if(2 == arg)
    {
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=get&type=power'",host);
    }
    system(cmd);
}

bool usb_ctrl(int mode,int node)
{
    char cmd[128] = {0};
    if(mode==2)
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=get&type=usb'",host);
    else 
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=set&type=usb&mode=%d&node=%d'",host,mode,node-1);
    system(cmd);
}

bool resetSW(void)
{
    char cmd[128] = {0};
    snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=set&type=network&cmd=reset'",host);
    system(cmd);
}

bool uart_ctrl(int mode,int node,char* data)
{
    char cmd[128] = {0};
    if(mode==0)
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=get&type=uart&node=%d'",host,node);
    else 
    {
        if(NULL==data)
            return false;
        snprintf(cmd,sizeof(cmd),"curl 'http://%s/api/bmc?opt=set&type=uart&node=%d&cmd=%s'",host,node,data);
    }
        
    system(cmd);
}


bool update_fw(char* file)
{
    bool ret = false;
    char cmd[128] = {0};
    char line[1024] = {0};
    snprintf(cmd,sizeof(cmd),"curl -F 'file=@%s' 'http://%s/api/bmc?opt=set&type=firmware'",file,host);

    FILE* pp = popen(cmd,"r");
    if(pp)
    {
        while (fgets(line, 1024, pp)) {
            if (strstr(line, "\"result\":\"ok\"") ) {
                ret = true;
                break;
            }
        }
        pclose(pp);
        pp = NULL;
    }
    return ret;
}


static char file_path[512];


int download_file(char* url)
{
    bool update_ret = false;
    char cmd[1024];
    char line[1024];
    if(NULL == url)
    {
        printf("Missing upgrade file \n");
        return -1;
    }
    if(access(url,F_OK)==0)
    {
        update_fw(url);
        return 1;
    }
    
    snprintf(cmd,sizeof(cmd),"wget -O fw.bin %s",url);
    FILE* pp = popen(cmd,"r");
    if(pp)
    {
        // while (fgets(line, 1024, pp)) {
        //     printf("wget ret = %s\n",line);
        //     if (strstr(line, "saved") ) {
        //         
        //         update_ret = true;
        //         break;
        //     }
        // }
        pclose(pp);
        pp = NULL;
    }
    
    sync();
    usleep(100*1000);

    // if(update_ret)
    {
        if(access("./fw.bin",F_OK)==0)
        {
            update_fw("./fw.bin");
            sleep(1);
            remove("./fw.bin");
            return 0;
        }
    }
}



int Socket(int domain, int type, int protocol)
{
    int sock = socket(domain, type, protocol);

    if (sock < 0)
    {
        ERR_Printf("socket\n");
        return -1;
    }
    return sock;
}
void CloseSocket(int fd)
{
    Printf("======> close fd %d\n", fd);

    if (fd > 0)
    {
        close(fd);
    }

}


int checkIpAddress(const char *ipaddr)
{
    char* delim = ".";
    int i = 0, flag = 0;
    char buf[64];
    char *tmp = NULL, *ptr;
    
    if (strlen(ipaddr) > 15)
        return(-1);
    strncpy(buf, ipaddr, 16);
    
    while (buf[i] != '\0')
    {
        switch (buf[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            case '.':
                flag++;
                break;
            default:
                return(-1);
        }
        i++;
    }
    
    if (flag != 3)
        return(-1);
    
    ptr = strdup(ipaddr);
    for (i = 0; i < 4; i++)
    {
        tmp = NULL;
        tmp = strsep(&ptr, delim);
        
        if (tmp == NULL)
        {
            if(ptr)
            free(ptr);
            return(-1);
        }
        
        if (atoi(tmp) > 255 || strlen(tmp) > 3 || (tmp[0] == '0' && strlen(tmp) != 1))
        {
            if(ptr)
            free(ptr);
            return(-1);
        }
            
            
    }
    
    if(ptr)
        free(ptr);
    return(0);
}




char* getIpByIface(const char* _iface, char* _ip, int len)
{
    if (_iface == NULL || _ip == NULL)
    {
        ERR_Printf("args\n");
        return NULL;
    }

    int sock = Socket(AF_INET, SOCK_STREAM, 0);
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, _iface, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        ERR_Printf("ioctl\n");
        CloseSocket(sock);
        return NULL;
    }

    struct sockaddr_in *addr;
    addr = (struct sockaddr_in *) & (ifr.ifr_addr);
    inet_ntop(AF_INET, &addr->sin_addr, _ip, len);
    return _ip;
}


void usage(void)
{
    printf("Usage: tpi [host] <options...>\n");
    printf("Options: \n");

    printf("\t-p, --power        (on off status) Power management \n");
    printf("\t-u, --usb          (host device status) USB mode,Must be used with the node command\n");
    printf("\t-n, --node         (1 2 3 4) USB selected node\n");
    printf("\t-r, --resetsw       reset switch\n");
    printf("\t-U, --uart         uart opt get or set\n");
    printf("\t-C, --cmd          uart set cmd\n");
    printf("\t-F, --upgrade      upgrade fw\n");
    printf("\t-f, --flash        todo\n");
    printf("\t-h, --help         usage\n");
    printf("example: \n");
    printf("\t$ tpi -p on //power on\n");
    printf("\t$ tpi -p off //power off\n");
    printf("\t$ tpi -u host -n 1 //USB uses host mode to connect to Node1\n");
    printf("\t$ tpi --uart=get -n 1 //get node1 uart info\n");
    printf("\t$ tpi --uart=set -n 1 --cmd=ls//set node1 uart cmd\n");
    printf("\t$ tpi --upgrade=/mnt/sdcard/xxxx.swu    //upgrade fw\n");
    printf("\t$ tpi -r  //reset switch\n");
    exit(1);
}


static struct option long_options[] =
{
    {"power", required_argument, NULL, 'p'},
    {"usb", optional_argument, NULL, 'u'},
    {"node", optional_argument, NULL, 'n'},
    {"flash", optional_argument, NULL, 'f'},
    {"resetsw", no_argument, NULL, 'r'},
    {"uart", optional_argument, NULL, 'U'},
    {"upgrade", optional_argument, NULL, 'F'},
    {"cmd", optional_argument, NULL, 'C'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};

int main(int argc, char *argv[])
{
    int opt;
    int digit_optind = 0;
    int option_index = 0;
    char *string = "p:u:U:n:f:F:hr";

    // test();
    char uart_cmd[512];
    char up_file[512];
    int mode = -1;
    int power_cmd = -1;
    int usb_mode = -1;
    int node = -1;
    int uart_mode = -1;
    while ((opt = getopt_long_only(argc, argv, string, long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'p':
            {
                mode = 0;
                if(strcasecmp(optarg,"off") == 0)
                {
                    power_cmd = 0;
                }
                else if(strcasecmp(optarg,"on") == 0)
                {
                    power_cmd = 1;
                }
                else if(strcasecmp(optarg,"status") == 0)
                {
                    power_cmd = 2;
                }
                else
                    usage();
                break;
            }
            case 'u':
            {
                mode = 1;
                if(strcasecmp(optarg,"host") == 0)
                {
                    usb_mode = 0;
                }
                else if(strcasecmp(optarg,"device") == 0)
                {
                    usb_mode = 1;
                }
                else if(strcasecmp(optarg,"status") == 0)
                {
                    usb_mode = 2;
                }
                else
                    usage();
                break;
            }
            case 'n':
            {
                if(optarg)
                    node = atoi(optarg);
                if(node<0||node>4)
                    usage();
                break;
            }
            case 'r':
            {
                mode = 2;
                break;
            }
            case 'U':
            {
                mode = 3;
                if(strcasecmp(optarg,"get") == 0)
                {
                    uart_mode = 0;
                }
                else if(strcasecmp(optarg,"set") == 0)
                {
                    uart_mode = 1;
                }
                break;
            }
            case 'F':
            {
                mode = 4;
                strncpy(up_file,optarg,sizeof(up_file));
            }
            case 'C':
            {
                strncpy(uart_cmd,optarg,sizeof(uart_cmd));
                break;
            }
            default:
                usage();
        }
    }

    if(optind != argc)
    {
        strncpy(host,argv[optind],sizeof(host));
        //check ip
        if(checkIpAddress(host))
        {
            usage();
        }
    }
    else
    {
        getIpByIface("eth0",host,sizeof(host));
        if(checkIpAddress(host))
        {
            usage();
        }

    }

    switch(mode)
    {
        case 0:
        {
            power(power_cmd);
            break;
        }
        case 1:
        {
            // Printf("todo\n");
            usb_ctrl(usb_mode,node);
            break;
        }
        case 2:
        {
            resetSW();
            break;
        }
        case 3:
        {
            uart_ctrl(uart_mode,node,uart_cmd);
            break;
        }
        case 4:
        {
            download_file(up_file);
        }
        default:
            usage();
    }
    
    
}