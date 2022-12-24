/**
 * @file factory.h
 * @author   wenyi
 * @brief 
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef __FACTORY_H__
#define __FACTORY_H__

#define FAC_STR_LEN     (128)
typedef struct _FAC_INFO_t
{
    char pcie[FAC_STR_LEN];
    char ip[FAC_STR_LEN];
    char usbotg[FAC_STR_LEN];
    char disk1[FAC_STR_LEN];
    char disk2[FAC_STR_LEN];
    char usb[5][FAC_STR_LEN];
    // char usb2[FAC_STR_LEN];
    // char usb3[FAC_STR_LEN];
    char ssd[FAC_STR_LEN];
}FAC_INFO_t;

typedef struct _FAC_RESULT_t
{
    /* data */
    FAC_INFO_t node[4];
}FAC_RESULT_t;

int factory_mode(void);
FAC_RESULT_t* get_fac_result(void);
int factory_multiple(void);
void stop_fac(void);



#endif // !__FACTORY_H__

