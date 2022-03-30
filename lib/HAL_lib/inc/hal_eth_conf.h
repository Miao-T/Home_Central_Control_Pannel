////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_eth_conf.h
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  v2.0.0
/// @date     2019-07-30
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE HAL_eth_conf.h EXAMPLES.
/// ////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

#ifndef __HAL_ETH_CONF_H
#define __HAL_ETH_CONF_H

// #define USE_ENHANCED_DMA_DESCRIPTORS
// #define CUSTOM_DRIVER_BUFFERS_CONFIG
//#define DP83848
#define LAN8720

#ifdef CUSTOM_DRIVER_BUFFERS_CONFIG
#define ETH_RX_BUF_SIZE     ETH_MAX_PACKET_SIZE
#define ETH_TX_BUF_SIZE     ETH_MAX_PACKET_SIZE
#define ETH_RX_BUF_NUM      4
#define ETH_TX_BUF_NUM      4
#endif

////////////////////////////////////////////////////////////////////////////////
#if defined(DP83848)
#define PHY_SR                  ((uint16_t)0x10)
#define PHY_SR_LINKSTATUS       ((uint16_t)0x0001)
#define PHY_SPEED_STATUS        ((uint16_t)0x0002)
#define PHY_DUPLEX_STATUS       ((uint16_t)0x0004)

#define PHY_MICR                ((uint16_t)0x11)
#define PHY_MICR_INT_EN         ((uint16_t)0x0002)
#define PHY_MICR_INT_OE         ((uint16_t)0x0001)

#define PHY_MISR                ((uint16_t)0x12)
#define PHY_MISR_LINK_INT_EN    ((uint16_t)0x0020)
#define PHY_LINK_STATUS         ((uint16_t)0x2000)
#endif


#if defined(LAN8720)

#define PHY_SCSR                ((uint16_t)0x1F)
#define PHY_SCSR_AutoDone       ((uint16_t)0x1 << 12)
#define PHY_SCSR_10xHalfDup     ((uint16_t)0x001 << 2)
#define PHY_SCSR_10xFullDup     ((uint16_t)0x101 << 2)
#define PHY_SCSR_100xHalfDup    ((uint16_t)0x010 << 2)
#define PHY_SCSR_100xFullDup    ((uint16_t)0x110 << 2)

#define PHY_DUPLEX_STATUS       (0x1 << 4)
#define PHY_SPEED_STATUS        (0x1 << 3)

#endif



#endif
