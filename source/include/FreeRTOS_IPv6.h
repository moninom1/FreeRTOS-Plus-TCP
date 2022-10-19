/*
 * FreeRTOS+TCP V2.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef FREERTOS_IPV6_H
#define FREERTOS_IPV6_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"

/* Some constants defining the sizes of several parts of a packet.
 * These defines come before including the configuration header files. */
#define ipSIZE_OF_IPv6_HEADER                    40U
#define ipSIZE_OF_ICMPv6_HEADER                  24U

#define ipSIZE_OF_IPv6_ADDRESS                   16U

#define ipPROTOCOL_ICMP_IPv6                     ( 58U )
#define ipTYPE_IPv6                              ( 0x60U )

/* Some IPv6 ICMP requests. */
#define ipICMP_DEST_UNREACHABLE_IPv6             ( ( uint8_t ) 1U )
#define ipICMP_PACKET_TOO_BIG_IPv6               ( ( uint8_t ) 2U )
#define ipICMP_TIME_EXEEDED_IPv6                 ( ( uint8_t ) 3U )
#define ipICMP_PARAMETER_PROBLEM_IPv6            ( ( uint8_t ) 4U )
#define ipICMP_PING_REQUEST_IPv6                 ( ( uint8_t ) 128U )
#define ipICMP_PING_REPLY_IPv6                   ( ( uint8_t ) 129U )
#define ipICMP_ROUTER_SOLICITATION_IPv6          ( ( uint8_t ) 133U )
#define ipICMP_ROUTER_ADVERTISEMENT_IPv6         ( ( uint8_t ) 134U )
#define ipICMP_NEIGHBOR_SOLICITATION_IPv6        ( ( uint8_t ) 135U )
#define ipICMP_NEIGHBOR_ADVERTISEMENT_IPv6       ( ( uint8_t ) 136U )


#define ipIPv6_EXT_HEADER_HOP_BY_HOP             0U
#define ipIPv6_EXT_HEADER_DESTINATION_OPTIONS    60U
#define ipIPv6_EXT_HEADER_ROUTING_HEADER         43U
#define ipIPv6_EXT_HEADER_FRAGMENT_HEADER        44U
#define ipIPv6_EXT_HEADER_AUTHEN_HEADER          51U
#define ipIPv6_EXT_HEADER_SECURE_PAYLOAD         50U
/* Destination options may follow here in case there are no routing options. */
#define ipIPv6_EXT_HEADER_MOBILITY_HEADER        135U

#ifndef _MSC_VER
    extern const struct xIPv6_Address in6addr_any;
    extern const struct xIPv6_Address in6addr_loopback;
#else

/* Microsoft visual C already has these objects defined.
 * Name them slightly different. */
    extern const struct xIPv6_Address FreeRTOS_in6addr_any;
    extern const struct xIPv6_Address FreeRTOS_in6addr_loopback;
    #define in6addr_any         FreeRTOS_in6addr_any
    #define in6addr_loopback    FreeRTOS_in6addr_loopback

#endif

/* A forward declaration of 'struct xNetworkEndPoint' and 'xNetworkInterface'.
 * The actual declaration can be found in FreeRTOS_Routing.h which is included
 * as the last +TCP header file. */
struct xNetworkEndPoint;
struct xNetworkInterface;

/* The last parameter is either ipTYPE_IPv4 or ipTYPE_IPv6. */
void * FreeRTOS_GetUDPPayloadBufferv6( size_t uxRequestedSizeBytes,
                                       TickType_t uxBlockTimeTicks,
                                       uint8_t ucIPType );

/*
 * Calculates the starting offset of the UDP payload.
 * If IPv6 enabled, checks for ( usFrameType == ipIPv6_FRAME_TYPE )
 */
uint8_t * pcNetworkBuffer_to_UDPPayloadBuffer( NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Returns the addresses stored in an end-point structure.
 * This function already existed in the release with the single-interface.
 * Only the first parameters is new: an end-point
 */
void FreeRTOS_GetEndPointConfiguration( uint32_t * pulIPAddress,
                                        uint32_t * pulNetMask,
                                        uint32_t * pulGatewayAddress,
                                        uint32_t * pulDNSServerAddress,
                                        struct xNetworkEndPoint * pxEndPoint );

void FreeRTOS_SetEndPointConfiguration( const uint32_t * pulIPAddress,
                                        const uint32_t * pulNetMask,
                                        const uint32_t * pulGatewayAddress,
                                        const uint32_t * pulDNSServerAddress,
                                        struct xNetworkEndPoint * pxEndPoint );

/* Return true if a given end-point is up and running.
* When FreeRTOS_IsNetworkUp() is called with NULL as a parameter,
* it will return pdTRUE when all end-points are up. */
BaseType_t FreeRTOS_IsEndPointUp( const struct xNetworkEndPoint * pxEndPoint );

/* Return pdTRUE if all end-points are up.
 * When pxInterface is null, all end-points will be checked. */
BaseType_t FreeRTOS_AllEndPointsUp( const struct xNetworkInterface * pxInterface );

extern void FreeRTOS_ClearND( void );

/* Return pdTRUE if the IPv6 address is a multicast address. */
BaseType_t xIsIPv6Multicast( const IPv6_Address_t * pxIPAddress );

/* Note that 'xCompareIPv6_Address' will also check if 'pxRight' is
 * the special unicast address: ff02::1:ffnn:nnnn, where nn:nnnn are
 * the last 3 bytes of the IPv6 address. */
BaseType_t xCompareIPv6_Address( const IPv6_Address_t * pxLeft,
                                 const IPv6_Address_t * pxRight,
                                 size_t uxPrefixLength );

/* FreeRTOS_dnslookup6() returns pdTRUE when a host has been found. */
uint32_t FreeRTOS_dnslookup6( const char * pcHostName,
                              IPv6_Address_t * pxAddress_IPv6 );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }         /* extern "C" */
#endif
/* *INDENT-ON* */

#endif /* FREERTOS_IP_H */
