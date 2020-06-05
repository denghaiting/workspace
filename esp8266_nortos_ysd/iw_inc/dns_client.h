/**************************************************************************
**  Copyright (c) 2011 GalaxyWind, Ltd.
**
**  Project: Ä§µÆ
**  File:    dns_client.h
**  Author:  fangfuhan
**  Date:    06/12/2016
**
**  Purpose:
**    Dns½âÎö.
**************************************************************************/


#ifndef DNS_CLIENT_H
#define DNS_CLIENT_H


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files. */
#include "c_types.h"

/* Macro constant definitions. */

/* Type definitions. */
#pragma pack(push, 1)

typedef struct dns_hdr {
	u_int16_t id;
	u_int8_t  flags1;
	u_int8_t  flags2;
	u_int16_t num_questions;
	u_int16_t num_answers;
	u_int16_t num_auth_rr;
	u_int16_t num_extra_rr;
}dns_hdr_t;

/* DNS query message structure.
 */
typedef struct dns_query {
  /* DNS query record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  u_int16_t type;
  u_int16_t cls;
}dns_query_t;


/** DNS answer message structure.
 */
typedef struct dns_answer {
  /* DNS answer record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  u_int16_t type;
  u_int16_t cls;
  u_int32_t ttl;
  u_int16_t len;
}dns_answer_t;

#pragma pack(pop)



/* External function declarations. */


/* Macro API definitions. */
#define SIZEOF_DNS_HDR	(sizeof(dns_hdr_t))
#define SIZEOF_DNS_QUERY (sizeof(dns_query_t))
#define SIZEOF_DNS_ANSWER (sizeof(dns_answer_t))


/* Global variable declarations. */

int dns_cli_gethostbyname(char *name, dn_t *dn);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* DNS_CLIENT_H */

