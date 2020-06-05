/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: mul_port.c
**  File:    mul_port.c
**  Author:  xianxiaojun
**  Date:    09/21/2015
**
**  Purpose:
**    ��������˿�ѡ��.
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


/* �ϴ����ӳɹ��Ķ̶౾��������Դ��� */
#define PORT_RETRY_CONNECT_TIMES  3

/* �˿�����˳���Ը��Ŷ˿������������� */
static const int tab_serverport[] = {
    DFL_UDP_CTRL_DEV_PORT2,
    DFL_UDP_CTRL_DEV_PORT1,
    DFL_UDP_CTRL_DEV_PORT
};
static bool had_connected = FALSE;

/* ��ȡflash�б�������ŷ������˿����� */
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

/* ���ط������˿��ڱ��е�������δ�ҵ��ͷ�����Чֵ */
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

/* �Ự��ȡ�豸���ӷ������Ķ˿� */
void ICACHE_FLASH_ATTR get_serverport(ucdc_session_t *s)
{
    #define SERVERPORT_TAB_LEN  (sizeof(tab_serverport)/sizeof(tab_serverport[0]))
    
    static INT8 prio_port_retrytimes = PORT_RETRY_CONNECT_TIMES;
    static UINT8 index = 0;
    int port = 0;


    /* �Ѿ����ӳɹ�������������֮ǰ���ɹ��Ķ˿� */
    if (had_connected)
    {
        prio_port_retrytimes = PORT_RETRY_CONNECT_TIMES;
        had_connected = FALSE;
    }

    if (PORT_RETRY_CONNECT_TIMES == prio_port_retrytimes)
    {
        /* ��һ�ζ�ȡ�ϴ����ӳɹ��Ķ˿ڵ���������ȡ�ɹ������Ӵ���Ϊ PORT_RETRY_CONNECT_TIMES */
        port = get_savingServerPort();
        ERROR("read flash port=%d\n", port);
        
        index = getPort_Index(port, tab_serverport, SERVERPORT_TAB_LEN);
        if (SERVERPORT_TAB_LEN <= index)
        {
            ERROR("read flash fail\n");
            /* δ��ȡ�ɹ�����ӱ��е�һ���˿ڿ�ʼ ��ֻ����һ�� */
            index = 0;
            prio_port_retrytimes = 1;
        }
    }

    if (0 < prio_port_retrytimes)
    {
        /* ���ƴ�flash��ȡ�˿�ʱ�����Ӵ��� */
        prio_port_retrytimes--;
        ERROR("remain retry times=%d\n", prio_port_retrytimes);
    }
    else
    {
        /* �л��˿� */
        index++;
        if (index >= SERVERPORT_TAB_LEN)
        {
            index = 0;
        }
    }

    s->port = tab_serverport[index];

    ERROR("get index=%d, port=%d\n", index, s->port);
}

/* �����豸�ɹ�������˿� */
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

