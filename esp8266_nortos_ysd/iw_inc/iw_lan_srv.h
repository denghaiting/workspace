#ifndef __IWUOS_LAN_SRV_HEADER__
#define __IWUOS_LAN_SRV_HEADER__

#include "os_type.h"
#include "stlc_list.h"
#include "udp_ctrl.h"
#include "iw_api.h"


#define MAX_VENDER 16

typedef struct {
	// �ػ����ͣ�������ڽṹ���һ���ط� ,UCD_ST_XXX
	u_int8_t ucd_session_type;	

	u_int8_t is_used;
	// �Ƿ��Ǿ�����ֱ��
	u_int8_t is_lan;
	// �Ƿ���ǰ̨
	u_int8_t is_front;
        
	//�Ƿ��Ǻ����������push
	u_int8_t ir_sound_push;
	//�Ƿ���Ҫ������Ϣ
	u_int8_t debug;
	
	struct stlc_list_head link;

	char name[32];
	// ��ǰ״̬��UCCS_xxx
	int status;

	// ������ucd.sock_server��Ҳ������ucd.sock_client
	//SOCKET sock;
	// ����ֻ�Ǳ���һ�������ڴ������Ǿ������������ͷ�
             struct espconn *conn;
        
	// �ỰID
	u_int32_t client_sid;

	// ����ID
	u_int8_t my_request_id;
	u_int8_t peer_request_id;
	u_int8_t my_last_request_id;
	u_int8_t peer_last_request_id;

	//��Ӧ�ͻ���ʱ���������ֻ�ʱ����һ�����⡣
	u_int8_t time_zone_for_client;
	//��ÿ��session�����һ�η�������
	pkt_t *last_pkt;
	//�Ƿ���Է���
	bool last_pkt_enable;

        
	//�Ƿ����wan��push
	bool wan_push_enable;
	//�Ƿ����lan��push
	bool need_push;
	//�Ƿ���ܵ���udp ctrl ����,�����ֻ����豸�������Ӳ�ͬ��,�����豸�������ӳɹ��ֻ���û�У���ʱ�����ֻ����ء�
	bool is_recv_udp;
	// ��ַ�˿ں�
	u_int16_t port;
	u_int16_t pad;
	u_int32_t ip;

	// �ӽ�����Կ. sizeof(symmetric_key) = 516
	u_int8_t key[520];
	// �����
	u_int8_t rand1[4];
	u_int8_t rand2[4];

	// ���ܡ����ܺ���
	ucs_enc_func_t enc;
	ucs_dec_func_t dec;
	
	// ʱ�������ָ�� ucd->time_param�еĶ�Ӧ��
	uc_time_param_item_t *time_param;

        
	bool is_enc;
	// ѡ��ļ��ܷ�ʽ
	u_int16_t select_enc;
	// ������ԭ��
	u_int16_t die_err;     
	bool issupport_tt;//֧��͸������  Transparent transmission

	// �ͻ��˵�UUID
	u_int8_t uuid[MAX_UUID_BIN_LEN];

	// ��������Ķ���
	struct stlc_list_head send_list;
	// ���͵����Դ���
	int send_retry;
	// ���Է��͵Ķ�ʱ��
	u_int16_t t_send;
	u_int16_t t_die;
}ucds_session_t;

extern int uci_send_index;
extern u_int8_t uci_send_buf[MAX_UC_PKT_SIZE];
extern u_int8_t uci_proc_num;

/*
	����:
		��λ���еĻỰ
	�������:
	�������:
	����ֵ:
	˵��:
*/
void other_client_session_reset(void);
/*
	����:
		�������ݿ��ƣ������Ϊsystem��ia
	�������:
		@info: ��Ϣ
		@uobj: ����ֵ
	�������:
	����ֵ:
		RS_OK: �ɹ�
		����: ʧ��
	˵��:
*/
rs_t  iw_do_obj_ctrl(iw_event_info_t *info, iw_obj_t *uobj);
/*
	����:
		�������ݻظ�������
	�������:
		@err: ͨ��Э������ �μ�ds_proto.h
		@uobj: ����ֵ
	�������:
	����ֵ:
	˵��:
*/
void uc_do_set_reply(ucp_obj_t *uobj, u_int16_t err);
/*
	����:
		UDP CTRL ͨ�ñ���׷�Ӷ������ڶԶ�����ر���
	�������:
		@buf: �����ַ
		@size: �����С
		@pos: �Ѵ���Ļ���λ�ñ��
		@object: ����
		@sub_object: �Ӷ���
		@attri: ����
		@len: ��������
		@value: ����ֵ
	�������:
	����ֵ:
		0: �ɹ�
	˵��:
*/
int uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value);
/*
	����:
		���籨�Ĺ�����ͨ��get��set��delete�¼��ص�ע����¼�
	�������:
		@s: �Ự
		@is_from_server: �Ƿ����������ֻ���
		@action: ����get��set��delete
		@uobj: �������ԣ� ��������
		@param: ����ֵ
	�������:
	����ֵ:
	˵��:
*/
void  do_udp_ctrl_core(void *s, bool is_from_server, u_int8_t action, ucp_obj_t *uobj, u_int8_t *param);
/*
	����:
		�õ����ߵĿͷ���
	�������:
	�������:
	����ֵ:
		int: ���ߵĿͷ��˸���
	˵��:
*/
int ucds_get_clients(void);
/*
	����:
		��λ��ds���������еĻỰ
	�������:
		@ds: ָ���ĻỰ
	�������:
	����ֵ:
	˵��:
*/
void other_session_reset(ucds_session_t *ds);
/*
	����:
		������ת�������ݣ��;������ֻ���ֱ�ӷ��͵����ݴ���
	�������:
		@conn: �豸�˵�����ṹ
		@data: ����ֵ
		@len: ���ݳ���
	�������:
	����ֵ:
	˵��:
*/
void ucds_proc_pkt(struct espconn *conn, u_int8_t *data, int len);
/*
	����:
		UDP CTRL ͨ�ñ���׷�Ӷ������ڶԶ�����ر���
	�������:
		@buf: �����ַ
		@size: �����С
		@pos: �Ѵ���Ļ���λ�ñ��
		@object: ����
		@sub_object: �Ӷ���
		@attri: ����
		@len: ��������
		@value: ����ֵ
	�������:
	����ֵ:
		0: �ɹ�
		<0:ʧ��
	˵��:
*/
int _uc_put_elemet(u_int8_t *buf, int size, int *pos, u_int16_t object, u_int16_t sub_object, u_int16_t attri, u_int16_t len, u_int8_t *value);
/*
	����:
		�����豸����
	�������:
		@info: ��Ϣ
		@uobj: ����ֵ
	�������:
	����ֵ:
	˵��:
*/
void ia_do_public(iw_event_info_t *info, iw_obj_t *uobj);

int ucds_request_add(ucds_session_t *s, pkt_t *pkt);
#endif