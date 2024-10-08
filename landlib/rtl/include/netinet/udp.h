/*	$NetBSD: udp.h,v 1.16 2012/06/22 14:54:35 christos Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)udp.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_UDP_H_
#define _NETINET_UDP_H_

#include <sys/types.h>


/*
 * How much room to leave for headers in UDP packets:
 *  14: struct ether_header
 *  20: struct ip
 *   8: struct udphdr
 * That's 42 but let's pad it out to 48 bytes.
 */
 
/*
#define ETHERNET_HEADER_SIZE 14
#define IP_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8
*/
//#define	UDP_TOTAL_HEADER_SIZE (ETHERNET_HEADER_SIZE + IP_HEADER_SIZE + UDP_HEADER_SIZE)


/*
 * Udp protocol header.
 * Per RFC 768, September, 1981.
 */

struct udphdr 
{
    uint16_t uh_sport;		/* source port */
    uint16_t uh_dport;		/* destination port */
    uint16_t uh_ulen;		/* udp length */
    uint16_t uh_sum;		/* udp checksum */

} __attribute__((packed));



/* socket options for UDP */
#define	UDP_ENCAP	100

/* Encapsulation types */
#define UDP_ENCAP_ESPINUDP_NON_IKE 	1 /* draft-ietf-ipsec-nat-t-ike-00/01 */
#define UDP_ENCAP_ESPINUDP		2 /* draft-ietf-ipsec-udp-encaps-06 */

/* Default encapsulation port */
#define UDP_ENCAP_ESPINUDP_PORT		500

/* Maximum UDP fragment size for ESP over UDP */
#define UDP_ENCAP_ESPINUDP_MAXFRAGLEN	552

#endif /* !_NETINET_UDP_H_ */
