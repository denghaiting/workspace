#ifndef __IWUOS_MEM_HEADER__
#define __IWUOS_MEM_HEADER__

/*
	����:
		��̬�ڴ����
	�������:
		@size: �����ڴ�Ĵ�С
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		void *:������ڴ�
		NULL :ʧ��
	˵��:
*/
void *iw_malloc(u_int32_t size);
/*
	����:
		��̬�ڴ���� = size * n
	�������:
		@size: �����ڴ�Ĵ�С
		@n: �ڴ����
	�������:
		@info: ����ϵͳ��Ϣ
	����ֵ:
		void *:������ڴ�
		NULL :ʧ��
	˵��:
*/
void *iw_calloc(u_int32_t size, u_int32_t n);
/*
	����:
		�ͷŷ�����ڴ�
	�������:
		@p: �ڴ��ַ
	�������:
	����ֵ:
	˵��:
*/
void iw_free(void *p);

#endif
