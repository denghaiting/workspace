/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mul_port.c
**  File:    mul_port.c
**  Author:  xianxiaojun
**  Date:    09/21/2015
**
**  Purpose:
**    服务器多端口选择.
**************************************************************************/

#include "iw_priv.h"
#include "mul_port.h"
#include "iw_user_config.h"


#if 0
#undef INFO
#undef DEBUG
#undef ERROR

#define INFO(x...) DBGPRINT_MGMT(DEBUG_ERROR, x)
#define DEBUG(x...) DBGPRINT_MGMT(DEBUG_ERROR, x)
#define ERROR(x...) DBGPRINT_MGMT(DEBUG_ERROR, x)
#endif


/* 上次连接成功的短多本次最多重试次数 */
#define PORT_RETRY_CONNECT_TIMES  3

/* 端口排列顺序以更优端口依次排列至右 */
static const int tab_serverport[] = {
    DFL_UDP_CTRL_DEV_PORT2,
    DFL_UDP_CTRL_DEV_PORT1,
    DFL_UDP_CTRL_DEV_PORT
};
static bool had_connected = FALSE;

/* 读取flash中保存的最优服务器端口索引 */
static int ICACHE_FLASH_ATTR get_savingServerPort(void)
{
    #define INVALID_PORT  -1

    config_t *acfg = NULL;

    acfg = conf_dev_get(DEV_SERVER_PORT_INDEX);
    
    if (NULL != acfg && NULL != acfg->value)
    {
        return (*((int *)(acfg->value)));
    }

    return INVALID_PORT;
}

/* 返回服务器端口在表中的索引，未找到就返回无效值 */
static UINT8 ICACHE_FLASH_ATTR getPort_Index(int port, const int *tab, UINT8 tabLen)
{
    UINT8 i;
    
    for (i = 0; i < tabLen; i++)
    {
        if (port == tab[i])
            return i;
    }

    return tabLen;
}

/* 会话获取设备连接服务器的端口 */
void ICACHE_FLASH_ATTR get_serverport(ucdc_session_t *s)
{
    #define SERVERPORT_TAB_LEN  (sizeof(tab_serverport)/sizeof(tab_serverport[0]))
    
    static INT8 prio_port_retrytimes = PORT_RETRY_CONNECT_TIMES;
    static UINT8 index = 0;
    int port = 0;


    /* 已经连接成功过，则需重试之前连成功的端口 */
    if (had_connected)
    {
        prio_port_retrytimes = PORT_RETRY_CONNECT_TIMES;
        had_connected = FALSE;
    }

    if (PORT_RETRY_CONNECT_TIMES == prio_port_retrytimes)
    {
        /* 第一次读取上次连接成功的端口的索引，读取成功则连接次数为 PORT_RETRY_CONNECT_TIMES */
        port = get_savingServerPort();
        ERROR("read flash port=%d\n", port);
        
        index = getPort_Index(port, tab_serverport, SERVERPORT_TAB_LEN);
        if (SERVERPORT_TAB_LEN <= index)
        {
            ERROR("read flash fail\n");
            /* 未读取成功，则从表中第一个端口开始 且只连接一次 */
            index = 0;
            prio_port_retrytimes = 1;
        }
    }

    if (0 < prio_port_retrytimes)
    {
        /* 控制从flash读取端口时的连接次数 */
        prio_port_retrytimes--;
        ERROR("remain retry times=%d\n", prio_port_retrytimes);
    }
    else
    {
        /* 切换端口 */
        index++;
        if (index >= SERVERPORT_TAB_LEN)
        {
            index = 0;
        }
    }

    s->port = tab_serverport[index];

    ERROR("get index=%d, port=%d\n", index, s->port);
}

/* 连接设备成功，保存端口 */
void ICACHE_FLASH_ATTR save_serverport(ucdc_session_t *s)
{
    had_connected = TRUE;

    if (s->port != get_savingServerPort())
    {
        conf_dev_set(DEV_SERVER_PORT_INDEX, (UINT8 *)&s->port, sizeof(s->port));
        ERROR("save port=%d\n", s->port);
    }
    else
        ERROR("save port: no differ\n");
}

