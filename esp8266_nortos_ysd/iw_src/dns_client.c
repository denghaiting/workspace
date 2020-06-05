/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 魔灯
**  File:    dns_client.c
**  Author:  fangfuhan
**  Date:    06/12/2016
**
**  Purpose:
**    Dns解析.
**************************************************************************/


/* Include files. */
#include "iw_priv.h"
#include "ice_domain.h"
#include "dns_client.h"
/* Macro constant definitions. */

#define DNS_CLI_SERVER_PORT          (53)

#define AF_INET         2

#define DNS_MSG_SIZE                    512
#define DNS_MAX_SERVERS                 2

#define DNS_MAX_NAME_LENGTH             256


/* Type definitions. */
/* DNS field TYPE used for "Resource Records". */
#define DNS_CLI_RRTYPE_A              1     /* a host address. */
#define DNS_CLI_RRTYPE_NS             2     /* an authoritative name server. */
#define DNS_CLI_RRTYPE_MD             3     /* a mail destination (Obsolete - use MX). */
#define DNS_CLI_RRTYPE_MF             4     /* a mail forwarder (Obsolete - use MX) */
#define DNS_CLI_RRTYPE_CNAME          5     /* the canonical name for an alias. */
#define DNS_CLI_RRTYPE_SOA            6     /* marks the start of a zone of authority. */
#define DNS_CLI_RRTYPE_MB             7     /* a mailbox domain name (EXPERIMENTAL). */
#define DNS_CLI_RRTYPE_MG             8     /* a mail group member (EXPERIMENTAL). */
#define DNS_CLI_RRTYPE_MR             9     /* a mail rename domain name (EXPERIMENTAL). */
#define DNS_CLI_RRTYPE_NULL           10    /* a null RR (EXPERIMENTAL). */
#define DNS_CLI_RRTYPE_WKS            11    /* a well known service description. */
#define DNS_CLI_RRTYPE_PTR            12    /* a domain name pointer. */
#define DNS_CLI_RRTYPE_HINFO          13    /* host information. */
#define DNS_CLI_RRTYPE_MINFO          14    /* mailbox or mail list information. */
#define DNS_CLI_RRTYPE_MX             15    /* mail exchange. */
#define DNS_CLI_RRTYPE_TXT            16    /* text strings. */

/* DNS field CLASS used for "Resource Records". */
#define DNS_CLI_RRCLASS_IN            1     /* the Internet */
#define DNS_CLI_RRCLASS_CS            2     /* the CSNET class (Obsolete - used only for examples in some obsolete RFCs).*/
#define DNS_CLI_RRCLASS_CH            3     /* the CHAOS class. */
#define DNS_CLI_RRCLASS_HS            4     /* Hesiod [Dyer 87].*/
#define DNS_CLI_RRCLASS_FLUSH         0x800 /* Flush bit. */

/* DNS protocol flags */
#define DNS_CLI_FLAG1_RESPONSE        0x80
#define DNS_CLI_FLAG1_OPCODE_STATUS   0x10
#define DNS_CLI_FLAG1_OPCODE_INVERSE  0x08
#define DNS_CLI_FLAG1_OPCODE_STANDARD 0x00
#define DNS_CLI_FLAG1_AUTHORATIVE     0x04
#define DNS_CLI_FLAG1_TRUNC           0x02
#define DNS_CLI_FLAG1_RD              0x01
#define DNS_CLI_FLAG2_RA              0x80
#define DNS_CLI_FLAG2_ERR_MASK        0x0f
#define DNS_CLI_FLAG2_ERR_NONE        0x00
#define DNS_CLI_FLAG2_ERR_NAME        0x03

/* Type definitions. */

typedef struct dns_pkt {
	// 总长度
	int total;
	// dns_hdr_t *
	u_int8_t *data;
} dns_pkt_t;


/* Local function declarations. */


/* Macro API definitions. */
//#define DNS_CLI_DEBUG_TEST
#ifdef DNS_CLI_DEBUG_TEST
	#define DNSC_DEBUG(fmt ...) DBGPRINT_MGMT(DEBUG_ERROR, fmt)
	#define DNSC_INFO(fmt ...)  DBGPRINT_MGMT(DEBUG_ERROR, fmt)
	#define DNSC_ERROR(fmt ...) DBGPRINT_MGMT(DEBUG_ERROR, fmt)
#else
	#define DNSC_DEBUG(fmt ...)  DEBUG(fmt) 
	#define DNSC_INFO(fmt ...)   INFO(fmt)
	#define DNSC_ERROR(fmt ...)  ERROR(fmt)
#endif

/* Global variable declarations. */

extern ip_addr_t dns_getserver(u_int8_t numdns);



/**
 * Compare the "dotted" name "query" with the encoded name "response"
 * to make sure an answer from the DNS server matches the current dns_table
 * entry (otherwise, answers might arrive late for hostname not on the list
 * any more).
 *
 * @param query hostname (not encoded) from the dns_table
 * @param response encoded hostname in the DNS response
 * @return 0: names equal; 1: names differ
 */
static u_int8_t ICACHE_FLASH_ATTR dns_cli_compare_name(u_int8_t *query, u_int8_t *response)
{
  u_int8_t n;

  do {
    n = *response++;
    /** @see RFC 1035 - 4.1.4. Message compression */
    if ((n & 0xc0) == 0xc0) {
      /* Compressed name */
      break;
    } else {
      /* Not compressed name */
      while (n > 0) {
        if ((*query) != (*response)) {
          return 1;
        }
        ++response;
        ++query;
        --n;
      };
      ++query;
    }
  } while (*response != 0);

  return 0;
}


/**
 * Walk through a compact encoded DNS name and return the end of the name.
 *
 * @param query encoded DNS name in the DNS server response
 * @return end of the name
 */
static u_int8_t * ICACHE_FLASH_ATTR dns_cli_parse_name(u_int8_t *query)
{
  u_int8_t n;

  do {
    n = *query++;
    /** @see RFC 1035 - 4.1.4. Message compression */
    if ((n & 0xc0) == 0xc0) {
      /* Compressed name */
      break;
    } else {
      /* Not compressed name */
      while (n > 0) {
        ++query;
        --n;
      };
    }
  } while (*query != 0);

  return query + 1;
}

void show_bin(u_int8_t *data, u_int8_t len)
{
	int i = 0;

	for(i = 0; i < len; i++){
		os_printf("0x%02x, ", data[i]);
	}
	os_printf("\r\n");
}

int ICACHE_FLASH_ATTR dns_cli_parse_packet(char *name, u_int8_t* data, int data_len, u_int32_t addr[NDS_CLI_MAX_IP], int addr_count)
{
	dns_hdr_t *hdr;
	u_int16_t nquestions;
	u_int16_t nanswers;
  	char *pHostname;
	dns_answer_t ans;
	u_int32_t ip;
	int index = 0;
	u_int16_t id;

	
    /* The ID in the DNS header should be our entry into the name table. */
    hdr = (dns_hdr_t*)data;
    id = htons(hdr->id);
	id = id;
    nquestions = htons(hdr->num_questions);
    nanswers   = htons(hdr->num_answers);

	DNSC_DEBUG("nquestions:%d, nanswers:%d\r\n", nquestions, nanswers);

	if ((hdr->flags1 & DNS_CLI_FLAG1_RESPONSE) == 0 || nquestions != 1){
		DNSC_ERROR("dns flag error 1\r\n");
		return 0;
	}
	
	if (dns_cli_compare_name((u_int8_t *)name, data + SIZEOF_DNS_HDR) != 0){
		DNSC_ERROR("dns doname error 2\r\n");
		return 0;
	}

   /* Skip the name in the "question" part */
    pHostname = (char *) dns_cli_parse_name((unsigned char *)data + SIZEOF_DNS_HDR) + SIZEOF_DNS_QUERY;

    while (nanswers > 0) {
	      /* skip answer resource record's host name */
	      pHostname = (char *) dns_cli_parse_name((unsigned char *)pHostname);
			
	      /* Check for IP address type and Internet class. Others are discarded. */
	      os_memcpy(&ans, (u_int8_t *)pHostname, SIZEOF_DNS_ANSWER);
	      if((ans.type == htons(DNS_CLI_RRTYPE_A)) 
		  	  && (ans.cls == htons(DNS_CLI_RRCLASS_IN))
		  	  &&(ans.len == htons(sizeof(ip))) ){
		  	
	        /* read the IP address after answer resource record's header */			
	        os_memcpy(&ip, (u_int8_t*)(pHostname + SIZEOF_DNS_ANSWER), sizeof(ip));
			addr[index] = ip;			
			index++;

			DNSC_DEBUG("[%s] %d.%d.%d.%d\r\n", name, IP_SHOW(ntohl(ip)));
	     }

		 /* 最多保存 addr_count个数ip*/
		 if (index >= addr_count){
			break;
		 }
		 
		 pHostname = pHostname + SIZEOF_DNS_ANSWER + htons(ans.len);
		 --nanswers;
    }

	return index;
}



static dns_pkt_t * ICACHE_FLASH_ATTR dns_cli_pkt_new(const char* name, u_int8_t id)
{
	dns_pkt_t *pkt = NULL;
	u_int32_t total_len = 0;
	dns_hdr_t *hdr = NULL;
	dns_query_t qry;
	char *query, *nptr;
	const char *pHostname;
	u_int8_t n;

	
	total_len = SIZEOF_DNS_HDR + DNS_MAX_NAME_LENGTH + SIZEOF_DNS_QUERY;

	pkt =(dns_pkt_t *) os_malloc(total_len);
	if (NULL == pkt){
		return NULL;
	}

	pkt->total = total_len;
	pkt->data = (u_int8_t *)&pkt[1];

	/* fill dns header */
	hdr = (dns_hdr_t*)pkt->data;
	os_memset(hdr, 0, SIZEOF_DNS_HDR);
	hdr->id = htons(id);
	hdr->flags1 = DNS_CLI_FLAG1_RD;
	hdr->num_questions = htons(1);
	query = (char*)hdr + SIZEOF_DNS_HDR;
	
	pHostname = name;
	--pHostname;
	/* convert hostname into suitable query format. */
	do {
	  ++pHostname;
	  nptr = query;
	  ++query;
	  for(n = 0; *pHostname != '.' && *pHostname != 0; ++pHostname) {
	    *query = *pHostname;
	    ++query;
	    ++n;
	  }
	  *nptr = n;
	} while(*pHostname != 0);
	*query++='\0';

	/* fill dns query */
	qry.type = htons(DNS_CLI_RRTYPE_A);
	qry.cls = htons(DNS_CLI_RRCLASS_IN);
	os_memcpy((u_int8_t*)query, &qry, SIZEOF_DNS_QUERY);

	pkt->total = ((query+SIZEOF_DNS_QUERY) - ((char *)(&pkt->data[0])));
	return pkt;
}


static int ICACHE_FLASH_ATTR dns_cli_gethostbyname_from_dnsserver(	
	u_int32_t dns_ser_ip, 	
	u_int8_t id, 	
	char *name,
	dn_t *dn)
{	
	dns_pkt_t *pkt = NULL;	
	int n = 0;	
	int ret = 1;
		
	pkt = dns_cli_pkt_new(name, id);
	if (NULL == pkt){		
		DNSC_ERROR("Malloc dns cli pkt failed\r\n");	
		ret = -1;
		goto end;	
	}	

	dns_ser_ip = htonl(dns_ser_ip);
	//DNSC_ERROR("dns ip = %u.%u.%u.%u:%u\r\n", IP_SHOW(dns_ser_ip), DNS_CLI_SERVER_PORT);	
	n = iw_udp_send(dns_ser_ip, DNS_CLI_SERVER_PORT, (char *)pkt->data, pkt->total, dn->reso_conn);	
	if (n <= 0) {		
		DNSC_ERROR("Send bytes failed(want:%d != real:%d)\r\n", pkt->total, n);		
		ret = n;	
		goto end;	
	} 

end:
	if(pkt != NULL){
		iw_free(pkt);
	}	
	
	return ret;
}

int ICACHE_FLASH_ATTR dns_cli_gethostbyname(char *name, dn_t *dn)
{
	static u_int8_t seq_no = 0;
	ip_addr_t dns_server_ip;
	int rt = 0;
	int i;

	if (NULL == name){
		return -1;
	}	
	rt = rt;

	//处理dns server ip无效的情况, 在接收处判断，主要是排除没有解析成功
	for(i = dn->dns_send_index; i < DNS_MAX_SERVERS; i++){
		dns_server_ip = dns_getserver(i);
		dn->dns_send_index = i;
		if (dns_server_ip.addr != 0){
			break;
		}
	}

	if(i == DNS_MAX_SERVERS){
		DNSC_ERROR("dns server index = [%d],  error\r\n", dn->dns_send_index);
		dn->dns_send_index = i;
		return 1; //返回1， 出去等待超时切换下一个域名
	}
	DNSC_ERROR("dns server index = [%d]\r\n", dn->dns_send_index);
	
	rt = dns_cli_gethostbyname_from_dnsserver(dns_server_ip.addr, seq_no++, name, dn);
	if (rt < 0){
		return -1;
	}
	
	return 1;
}

