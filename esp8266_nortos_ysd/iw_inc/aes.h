#ifndef	__AES_H__
#define	__AES_H__


#ifdef __cplusplus
extern "C" {
#endif

#define	AES128_EKY_LEN	16

/*
	����:
		һ�¶��ǿպ���û��ʹ��
	�������:

	�������:

	����ֵ:

	˵��:
*/
extern void enc_block(unsigned char *data, int len, unsigned char *passwd);
extern void dec_block(unsigned char *data, int len, unsigned char *passwd);
extern void dec_block2(unsigned char *dst, unsigned char *src, int len, unsigned char *passwd);
extern void aes_enc_with_key(unsigned char *data, int len, void *key);
extern void aes_dec_with_key(unsigned char *data, int len, void *key);
extern void *aes_init_key_buf(unsigned char *passwd, void *buf, int size);

#ifdef __cplusplus
}
#endif


#endif

