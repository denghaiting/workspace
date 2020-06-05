#ifndef	__MD5_H__
#define	__MD5_H__


#ifdef __cplusplus
extern "C" {
#endif 

#include "c_types.h"

typedef struct {
  unsigned int state[4];                                   /* state (ABCD) */
  unsigned int count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

extern void Md5_Init(MD5_CTX *ctx);
extern void Md5_Update(MD5_CTX *ctx, unsigned char *buf, unsigned len);
extern void Md5_Final(unsigned char digest[16], MD5_CTX *ctx);
extern void hash_passwd(u_int8_t *result, char *passwd);

#ifdef __cplusplus
}
#endif

#endif

