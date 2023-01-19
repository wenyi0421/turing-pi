/**
 * @file 
 * @author   wenyi
 * @brief   测试使用串口的方式获取节点测试结果
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <stdint.h>
#include "comm_device_serial.h"
#include "factory.h"
#include "bmctest.h"

// #define FAC_STR_LEN     (128)

#define Printf(string...)   \
do{\
    printf("%s(%d)[%s]: ", __FILE__, __LINE__, __FUNCTION__);\
    printf(string);\
} while(0)


static char *g_serial_path[4] = {"/dev/ttyS2", "/dev/ttyS1", "/dev/ttyS4", "/dev/ttyS5"};
static int g_serial_num = 0;


pthread_mutex_t g_mutex_data;


// static FAC_INFO_t g_fac_info[4] = {0};

static FAC_RESULT_t g_fac_info = {0};

int readUart(int fd, uint8_t* data,int len, int time_out_msec)
{
    int read_len = 0;
    int cur_len = 0;
    fd_set fds;
    struct timeval timeout;
    timeout.tv_sec = time_out_msec/1000;
    timeout.tv_usec = time_out_msec%1000;

    if(NULL == data || len<=0)
        return -1;


    do{
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        switch (select(fd + 1, &fds, NULL, NULL, &timeout))
        {
            case 0:
                if(read_len)
                    Printf("read uart len:%d,buff:%s\n",read_len,data);
                return read_len;   //time out
            case -1:
            {
                printf("select error\n");
                return -1;
            }
            default:
            {
                if (FD_ISSET(fd, &fds))
                {
                    cur_len = read(fd, data+read_len, len);
                    if (cur_len <= 0)
                    {
                        fprintf(stderr, "server read cmd error\n");
                        return read_len;
                    }
                    read_len += cur_len;
                    if(strstr(data,"testoktpi")!=NULL)
                    {
                        timeout.tv_sec=0;
                        timeout.tv_usec=0;
                        break;
                    }
                        
                    // Printf("read uart len:%d,buff:%s\n",read_len,data);
                }
            }
        }
    }while(timeout.tv_sec!=0 || timeout.tv_usec!=0);
    Printf("read uart buff:%s\n",data);
    return read_len;
}


bool rpi_login(int fd)
{
    int try = 0;
    char buff[2048] = {0};
    // step1: login
    do{
        try++;
        write(fd,"\r",1);
        memset(buff,0,sizeof(buff));
        readUart(fd,buff,sizeof(buff),1000);
        if(strlen(buff)>5)
            printf("read uart buff:%s\n",buff);

        if(NULL != strstr(buff,"raspberrypi:") ||NULL != strstr(buff,"tpi-desktop:"))     //login suc-->tpi@raspberrypi:~$
        {
            return true;
        }
        if(NULL!=strstr(buff,"login"))
        {
            printf("enter next step,intput user\n");
            write(fd,"tpi\r",strlen("tpi\r"));
            memset(buff,0,sizeof(buff));
            readUart(fd,buff,sizeof(buff),1000);
            printf("read uart buff:%s\n",buff);
            if(NULL!=strstr(buff,"Password"))
            {
                write(fd,"123456\r",strlen("123456\r"));
                memset(buff,0,sizeof(buff));
                readUart(fd,buff,sizeof(buff),1000);
                if(NULL!=strstr(buff,"raspberry") || NULL != strstr(buff,"tpi-desktop:"))
                {
                    printf("login success\n");
                    readUart(fd,buff,sizeof(buff),1000);
                    return true;
                }
            }
        }
    }while(try<30);
    return false;
}

static int find_str(char *str, char* buffer_para, char *cut_str)
{
	char buffer[2048] = {0};
    memset(buffer,0,sizeof(buffer));
	strcpy(buffer, buffer_para);
	char *p = strstr(buffer,str);
	if(p == NULL)
		return -1;
	p += strlen(str);
	char *pp = strstr(p,"#");
	if(pp == NULL)
		return -2;
	*pp = 0;
	strcpy(cut_str,p);
	return 0;
}





int parse_data(char* data,int num)
{
    int ret = -1;
    int i;
    char usb[64];
    Printf("data = %s\n",data);
    if(NULL == data)
    {
        Printf("data is NULL\n");
        return -1;
    }
    char para[512] = {0};
    pthread_mutex_lock(&g_mutex_data);
    if((ret = find_str("pcistatus=",data,para))==0)
    {
        Printf("node%d,pcistatus = %s\n",num,para);
        memset(g_fac_info.node[num].pcie,0,sizeof(g_fac_info.node[num].pcie));
        strcpy(g_fac_info.node[num].pcie,para);
    }

    memset(para,0,sizeof(para));
    if((ret = find_str("ip=",data,para))==0)
    {
        Printf("node%d,ip = %s\n",num,para);
        memset(g_fac_info.node[num].ip,0,sizeof(g_fac_info.node[num].ip));
        strcpy(g_fac_info.node[num].ip,para);
    }
    memset(para,0,sizeof(para));
    if((ret = find_str("usbstatus=",data,para))==0)
    {
        Printf("node%d,usbotg = %s\n",num,para);
        if(strlen(para)>3)
        {
            memset(g_fac_info.node[num].usbotg,0,sizeof(g_fac_info.node[num].usbotg));
            strcpy(g_fac_info.node[num].usbotg,para);
        }
    }
    memset(para,0,sizeof(para));
    if((ret = find_str("sata1=",data,para))==0)
    {
        Printf("node%d,sata1 = %s\n",num,para);
        memset(g_fac_info.node[num].disk1,0,sizeof(g_fac_info.node[num].disk1));
        strcpy(g_fac_info.node[num].disk1,para);
    }
    memset(para,0,sizeof(para));
    if((ret = find_str("sata2=",data,para))==0)
    {
        Printf("node%d,sata2 = %s\n",num,para);
        memset(g_fac_info.node[num].disk2,0,sizeof(g_fac_info.node[num].disk2));
        strcpy(g_fac_info.node[num].disk2,para);
    }

    for(i=0;i<5;++i)
    {
        snprintf(usb,sizeof(usb),"usb%d=",i+1);
        memset(para,0,sizeof(para));
        if((ret = find_str(usb,data,para))==0)
        {
            Printf("node%d,%s %s\n",num,usb,para);
            memset(g_fac_info.node[num].usb[i],0,sizeof(g_fac_info.node[num].usb[i]));
            strcpy(g_fac_info.node[num].usb[i],para);
            if(strlen(g_fac_info.node[num].usbotg)<4 && strlen(g_fac_info.node[2].usbotg)>4 && NULL!=strstr(para,g_fac_info.node[2].usbotg))
            {
                memset(g_fac_info.node[num].usbotg,0,sizeof(g_fac_info.node[num].usbotg));
                strcpy(g_fac_info.node[num].usbotg,para);
            }
        }
    }
    
    memset(para,0,sizeof(para));
    if((ret = find_str("ssd=",data,para))==0)
    {
        Printf("node%d,ssd = %s\n",num,para);
        memset(g_fac_info.node[num].ssd,0,sizeof(g_fac_info.node[num].ssd));
        strcpy(g_fac_info.node[num].ssd,para);
    }
    pthread_mutex_unlock(&g_mutex_data);
    return 0;
}


int rpi_test(int fd,int num)
{
    char *cmd = "./test.sh && echo testok$USER\r";
    char buff[2048] = {0};
    write(fd,cmd,strlen(cmd));
    memset(buff,0,sizeof(buff));
    readUart(fd,buff,sizeof(buff),15000);
    parse_data(buff,num);
    memset(buff,0,sizeof(buff));
    readUart(fd,buff,sizeof(buff),1000);
    return 0;
}

int getFac(int fd)
{
    
    // step1: login
    if(false == rpi_login(fd))
    {
        printf("login rpi error\n");
        return -1;
    }
    sleep(1);   //waiting
    // step2: get fac result
    // rpi_test(fd);
}

// char serail_path[] = "/dev/ttyS2";

int readUartFacRet(char* path)
{
    // step1: open uart
    int fd = -1;
    if (0 != common_device_UartOpen(&fd, path))
    {
        fd = -1;
        printf("open %s faild\n", path);
        return -1;
    }
    // step2: set baud
    common_device_UartSetAttr(fd, 115200, 8, 'n', 1);
    
    // step3: read & write
    getFac(fd);
    common_device_UartClose(fd);
    return 0;
}
// char* pstr = "pcistatus= Qualcomm Atheros AR928X Wireless Network Adapter (PCI-Express) (rev 01),ip=192.168.0.110,usbstatus=1665 Kingston Technology Digital DataTraveler SE9,";

// int main(int argc, char *argv)
// {
//     // step 1 usb 控制
//     // todo
//     printf("plz usb connect ------>\n");
//     // step2: 打开串口执行脚本
//     // readUartFacRet();
//     //test
//     parse_data(pstr);
//     // step3: 结束节点测试
// }

int factory_mode(void)
{
    int i=0;
    // ctrl_usbconnet(0,i);
    for(i=1; i<4; ++i)
    {
        g_serial_num = i;
        if(i!=0)
            ctrl_usbconnet(0,i);
        sleep(1);
        readUartFacRet(g_serial_path[i]);
        sleep(1);
    }
    sleep(1);
    g_serial_num = 0;
    readUartFacRet(g_serial_path[0]);
    sleep(1);
    return 0;
}

int fac_node_init(int num)
{
    // step1: open uart
    int fd = -1;
    if (0 != common_device_UartOpen(&fd, g_serial_path[num]))
    {
        fd = -1;
        printf("open %s faild\n", g_serial_path[num]);
        return -1;
    }
    // step2: set baud
    common_device_UartSetAttr(fd, 115200, 8, 'n', 1);
    // step1: login
    if(false == rpi_login(fd))
    {
        common_device_UartClose(fd);
        printf("login rpi error\n");
        return -1;
    }
    return fd;
    // step3: read & write
    getFac(fd);
    common_device_UartClose(fd);
}


bool fac_run = false;
void* fac_thread_proc(void* arg)
{
    int fd = 0;
    pthread_detach(pthread_self());
    int nodenum = *(int *)arg;
    Printf("start fac node = %d\n",nodenum);
    sleep(10);
    
    fd  = fac_node_init(nodenum);
    if(fd < 0)
    {
        printf("no found node : %d\n",nodenum);
        sprintf(g_fac_info.node[nodenum].pcie,"ERROR:uart faild or no insert node");
        return ;
    }
    while(fac_run)
    {
        sleep(3);
        rpi_test(fd,nodenum);
    }
    common_device_UartClose(fd);
    return ;
}

void* usb_thread_proc(void* arg)
{
    int i;
    bool led_first = true;
    int usb_count[4] = {0};
    pthread_detach(pthread_self());
    
    while(fac_run)
    {
        if(strlen(g_fac_info.node[1].usbotg)<4 && ++usb_count[1]<15)
        {
            ctrl_usbconnet(0,1);
        }
        else if(strlen(g_fac_info.node[2].usbotg)<4 && ++usb_count[2] < 10)
        {
            ctrl_usbconnet(0,2);
        }else if(strlen(g_fac_info.node[3].usbotg)<4 && ++usb_count[3] < 10)
        {
            ctrl_usbconnet(0,3);
        }
        else
        {
            if(led_first)
            {   
                led_first = false;
                ctrl_led_blink(0);
            }
        }
        sleep(3);
    }
}


int factory_multiple(void)
{
    if(fac_run)
        return 0;
    fac_run = true;
    static bool frist = true;
    if(frist)
    {
        memset(&g_fac_info,0,sizeof(FAC_RESULT_t));
        frist = false;
        pthread_mutex_init(&g_mutex_data,NULL);
    }
    int i;
    int ret;
    pthread_t pid[4];
    pthread_t pid_usb;
    int chn[4] = {0, 1, 2, 3};
    for (i = 0; i < 4; ++i)
    {
        ret = pthread_create(&pid[i], NULL, fac_thread_proc, &chn[i]);
    }
    pthread_create(&pid_usb, NULL, usb_thread_proc, NULL);
    sleep(3);
    return 0;
}


void stop_fac(void)
{
    fac_run = false;
}

FAC_RESULT_t* get_fac_result(void)
{
    return &g_fac_info;
}

