////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_eth.h
/// @author   LiuYQ (liuyunqing@mindmotion.com.cn)
/// @version  v2.0.0
/// @date     2019-07-30
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE HAL_eth.h EXAMPLES.
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

#ifndef __HAL_ETH_H
#define __HAL_ETH_H

// Files includes
#include "mm32_types.h"
#include "MM32.h"

#include "HAL_eth_conf.h"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ETH     | Header | Extra | VLAN tag | Payload   | CRC |
// Size    | 14     | 2     | 4        | 46 ~ 1500 | 4   |
#define ETH_MAX_PACKET_SIZE     1524
#define ETH_HEADER              14      ///< MAC Dest Addr 6 byte + MAC Src Addr 6 byte + Lenth/Type 2 byte
#define ETH_EXTRA               2
#define VLAN_TAG                4
#define ETH_PAYLOAD_MIN         46
#define ETH_PAYLOAD_MAX         1500
#define JUMBO_FRAME_PAYLOAD     9000

#ifndef ETH_RX_BUF_SIZE
#define ETH_RX_BUF_SIZE     ETH_MAX_PACKET_SIZE
#endif

#ifndef ETH_RX_BUF_NUM
#define ETH_RX_BUF_NUM      4
#endif

#ifndef ETH_TX_BUF_SIZE
#define ETH_TX_BUF_SIZE     ETH_MAX_PACKET_SIZE
#endif

#ifndef ETH_TX_BUF_NUM
#define ETH_TX_BUF_NUM      4
#endif

#define ETH_DMA_RDES_FL_Pos     16                                              ///< Ethernet DMA Received Frame Length Position

#define ETH_WAKEUP_REGISTER_LENGTH                          8                   ///< ETHERNET Remote Wake-up frame register length

#define ETH_DMA_RX_OVERFLOW_MISSEDFRAMES_COUNTERSHIFT       17                  ///< ETHERNET Missed frames counter Shift

#define ETH_DMA_TDES_COLLISION_COUNTSHIFT                   3                   ///< ETHERNET DMA Tx descriptors Collision Count Shift
#define ETH_DMA_TDES_BUFFER2_SIZESHIFT                      11                  ///< ETHERNET DMA Tx descriptors Buffer2 Size Shift

#define ETH_DMA_RDES_FRAME_LENGTHSHIFT                      16                  ///< ETHERNET DMA Rx descriptors Frame Length Shift
#define ETH_DMA_RDES_BUFFER2_SIZESHIFT                      11                  ///< ETHERNET DMA Rx descriptors Buffer2 Size Shift

///< ETHERNET errors
#define  ETH_ERROR                                          ((uint32_t)0)
#define  ETH_SUCCESS                                        ((uint32_t)1)


#ifdef _HAL_ETH_C_
#define GLOBAL

#else
#define GLOBAL extern
#endif


////////////////////////////////////////////////////////////////////////////////
/// @brief ETH Init Structure Definition
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    u32    ETH_AutoNegotiation;
    u32    ETH_Watchdog;
    u32    ETH_Jabber;
    u32    ETH_InterFrameGap;
    u32    ETH_CarrierSense;
    u32    ETH_Speed;
    u32    ETH_ReceiveOwn;
    u32    ETH_LoopbackMode;
    u32    ETH_Mode;
    u32    ETH_ChecksumOffload;
    u32    ETH_RetryTransmission;
    u32    ETH_AutomaticPadCRCStrip;
    u32    ETH_BackOffLimit;
    u32    ETH_DeferralCheck;
    u32    ETH_ReceiveAll;
    u32    ETH_SourceAddrFilter;
    u32    ETH_PassControlFrames;
    u32    ETH_BroadcastFramesReception;
    u32    ETH_DestinationAddrFilter;
    u32    ETH_PromiscuousMode;
    u32    ETH_MulticastFramesFilter;
    u32    ETH_UnicastFramesFilter;
    u32    ETH_HashTableHigh;
    u32    ETH_HashTableLow;
    u32    ETH_PauseTime;
    u32    ETH_ZeroQuantaPause;
    u32    ETH_PauseLowThreshold;
    u32    ETH_UnicastPauseFrameDetect;
    u32    ETH_ReceiveFlowControl;
    u32    ETH_TransmitFlowControl;
    u32    ETH_VLANTagComparison;
    u32    ETH_VLANTagIdentifier;
    u32    ETH_DropTCPIPChecksumErrorFrame;
    u32    ETH_ReceiveStoreForward;
    u32    ETH_FlushReceivedFrame;
    u32    ETH_TransmitStoreForward;
    u32    ETH_TransmitThresholdControl;
    u32    ETH_ForwardErrorFrames;
    u32    ETH_ForwardUndersizedGoodFrames;
    u32    ETH_ReceiveThresholdControl;
    u32    ETH_SecondFrameOperate;
    u32    ETH_AddressAlignedBeats;
    u32    ETH_FixedBurst;
    u32    ETH_RxDMABurstLength;
    u32    ETH_TxDMABurstLength;
    u32    ETH_DescriptorSkipLength;
    u32    ETH_DMAArbitration;
} ETH_InitTypeDef;

typedef struct {
    __IO u32   CS;                                                              ///< Control and Status
    __IO u32   BL;                                                              ///< Buffer1, Buffer2 lengths
    __IO u32   BUF1ADDR;                                                        ///< Buffer1 address pointer
    __IO u32   BUF2NDADDR;                                                      ///< Buffer2 or next descriptor address pointer

#ifdef USE_ENHANCED_DMA_DESCRIPTORS                                             ///< Enhanced ETHERNET DMA PTP Descriptors
    __IO u32        ExtendedStatus;                                             ///< Extended status for PTP receive descriptor
    __IO u32        Reserved1;                                                  ///< Reserved
    __IO u32        TimeStampLow;                                               ///< Time Stamp Low value for transmit and receive
    __IO u32        TimeStampHigh;                                              ///< Time Stamp High value for transmit and receive
#endif
} ETH_DMADESCTypeDef;

typedef struct {
    __IO u32                         len;
    __IO u32                         buf;
    __IO ETH_DMADESCTypeDef*    ptrDesc;
} FrameTypeDef;

typedef struct  {
    __IO ETH_DMADESCTypeDef*    ptrFS_Rx_Desc;                                  ///< First Segment Rx Desc
    __IO ETH_DMADESCTypeDef*    ptrLS_Rx_Desc;                                  ///< Last Segment Rx Desc
    __IO uint32_t               cnt;                                            ///< Segment count
} ETH_DMA_Rx_Frame_infos;




///???xhm:以下注释的内容不只从何得知
#define ETH_DMA_TDES_OWN                     ((uint32_t)0x80000000)             ///< OWN bit: descriptor is owned by DMA engine
#define ETH_DMA_TDES_ES                      ((uint32_t)0x00008000)             ///< Error summary: OR of the following bits: UE || ED || EC || LCO || NC || LCA || FF || JT
#define ETH_DMA_TDES_JT                      ((uint32_t)0x00004000)             ///< Jabber Timeout
#define ETH_DMA_TDES_FF                      ((uint32_t)0x00002000)             ///< Frame Flushed: DMA/MTL flushed the frame due to SW flush
#define ETH_DMA_TDES_LCA                     ((uint32_t)0x00000800)             ///< Loss of Carrier: carrier lost during transmission
#define ETH_DMA_TDES_NC                      ((uint32_t)0x00000400)             ///< No Carrier: no carrier signal from the transceiver
#define ETH_DMA_TDES_LCO                     ((uint32_t)0x00000200)             ///< Late Collision: transmission aborted due to collision
#define ETH_DMA_TDES_EC                      ((uint32_t)0x00000100)             ///< Excessive Collision: transmission aborted after 16 collisions
#define ETH_DMA_TDES_VF                      ((uint32_t)0x00000080)             ///< VLAN Frame
#define ETH_DMA_TDES_CC                      ((uint32_t)0x00000078)             ///< Collision Count
#define ETH_DMA_TDES_ED                      ((uint32_t)0x00000004)             ///< Excessive Deferral
#define ETH_DMA_TDES_UF                      ((uint32_t)0x00000002)             ///< Underflow Error: late data arrival from the memory
#define ETH_DMA_TDES_DB                      ((uint32_t)0x00000001)             ///< Deferred Bit

#define ETH_DMA_TDES_IC                      ((uint32_t)0x80000000)             ///< Interrupt on Completion
#define ETH_DMA_TDES_LS                      ((uint32_t)0x40000000)             ///< Last Segment
#define ETH_DMA_TDES_FS                      ((uint32_t)0x20000000)             ///< First Segment
#define ETH_DMA_TDES_DC                      ((uint32_t)0x04000000)             ///< Disable CRC
#define ETH_DMA_TDES_TER                     ((uint32_t)0x02000000)             ///< Transmit end of ring
#define ETH_DMA_TDES_TCH                     ((uint32_t)0x01000000)             ///< Second Address Chained
#define ETH_DMA_TDES_DP                      ((uint32_t)0x00800000)             ///< Disable Padding
#define ETH_DMA_TDES_TBS2                    ((uint32_t)0x003FF800)             ///< Transmit Buffer 2 Size
#define ETH_DMA_TDES_TBS1                    ((uint32_t)0x000007FF)             ///< Transmit Buffer 1 Size

#define ETH_DMA_TDES_B1AP                    ((uint32_t)0xFFFFFFFF)             ///< Buffer 1 Address Pointer

#define ETH_DMA_TDES_B2AP                    ((uint32_t)0xFFFFFFFF)             ///< Buffer 2 Address Pointer

#if defined(USE_ENHANCED_DMA_DESCRIPTORS)
#define ETH_DMA_PTP_TDES_TTSL                ((uint32_t)0xFFFFFFFF)              ///< Transmit Time Stamp Low
#define ETH_DMA_PTP_TDES_TTSH                ((uint32_t)0xFFFFFFFF)              ///< Transmit Time Stamp High
#endif

#define ETH_DMA_RDES_OWN                     ((uint32_t)0x80000000)             ///< OWN bit: descriptor is owned by DMA engine
#define ETH_DMA_RDES_AFM                     ((uint32_t)0x40000000)             ///< DA Filter Fail for the rx frame
#define ETH_DMA_RDES_FL                      ((uint32_t)0x3FFF0000)             ///< Receive descriptor frame length
#define ETH_DMA_RDES_ES                      ((uint32_t)0x00008000)             ///< Error summary: OR of the following bits: DE || OE || IPC || LC || RWT || RE || CE
#define ETH_DMA_RDES_DE                      ((uint32_t)0x00004000)             ///< Descriptor error: no more descriptors for receive frame
#define ETH_DMA_RDES_SAF                     ((uint32_t)0x00002000)             ///< SA Filter Fail for the received frame
#define ETH_DMA_RDES_LE                      ((uint32_t)0x00001000)             ///< Frame size not matching with length field
#define ETH_DMA_RDES_OE                      ((uint32_t)0x00000800)             ///< Overflow Error: Frame was damaged due to buffer overflow
#define ETH_DMA_RDES_VLAN                    ((uint32_t)0x00000400)             ///< VLAN Tag: received frame is a VLAN frame
#define ETH_DMA_RDES_FS                      ((uint32_t)0x00000200)             ///< First descriptor of the frame
#define ETH_DMA_RDES_LS                      ((uint32_t)0x00000100)             ///< Last descriptor of the frame
#define ETH_DMA_RDES_IPV4HCE                 ((uint32_t)0x00000080)             ///< IPC Checksum Error: Rx Ipv4 header checksum error
#define ETH_DMA_RDES_LC                      ((uint32_t)0x00000040)             ///< Late collision occurred during reception
#define ETH_DMA_RDES_FT                      ((uint32_t)0x00000020)             ///< Frame type - Ethernet, otherwise 802.3
#define ETH_DMA_RDES_RWT                     ((uint32_t)0x00000010)             ///< Receive Watchdog Timeout: watchdog timer expired during reception
#define ETH_DMA_RDES_RE                      ((uint32_t)0x00000008)             ///< Receive error: error reported by MII interface
#define ETH_DMA_RDES_DBE                     ((uint32_t)0x00000004)             ///< Dribble bit error: frame contains non int multiple of 8 bits
#define ETH_DMA_RDES_CE                      ((uint32_t)0x00000002)             ///< CRC error
#define ETH_DMA_RDES_MAMPCE                  ((uint32_t)0x00000001)             ///< Rx MAC Address/Payload Checksum Error: Rx MAC address matched/ Rx Payload Checksum Error

#define ETH_DMA_RDES_DIC                     ((uint32_t)0x80000000)             ///< Disable Interrupt on Completion
#define ETH_DMA_RDES_RER                     ((uint32_t)0x02000000)             ///< Receive End of Ring
#define ETH_DMA_RDES_RCH                     ((uint32_t)0x01000000)             ///< Second Address Chained
#define ETH_DMA_RDES_RBS2                    ((uint32_t)0x003FF800)             ///< Receive Buffer2 Size
#define ETH_DMA_RDES_RBS1                    ((uint32_t)0x000007FF)             ///< Receive Buffer1 Size

#define ETH_DMA_RDES_B1AP                    ((uint32_t)0xFFFFFFFF)             ///< Buffer 1 Address Pointer

#define ETH_DMA_RDES_B2AP                    ((uint32_t)0xFFFFFFFF)             ///< Buffer 2 Address Pointer


#if defined(USE_ENHANCED_DMA_DESCRIPTORS)
#define ETH_DMA_PTP_RDES_PTPV                            ((uint32_t)0x00002000)  ///< PTP Version
#define ETH_DMA_PTP_RDES_PTPFT                           ((uint32_t)0x00001000)  ///< PTP Frame Type
#define ETH_DMA_PTP_RDES_PTPMT                           ((uint32_t)0x00000F00)  ///< PTP Message Type
#define ETH_DMA_PTP_RDES_PTPMT_Sync                      ((uint32_t)0x00000100)  ///< SYNC message (all clock types)
#define ETH_DMA_PTP_RDES_PTPMT_FollowUp                  ((uint32_t)0x00000200)  ///< FollowUp message (all clock types)
#define ETH_DMA_PTP_RDES_PTPMT_DelayReq                  ((uint32_t)0x00000300)  ///< DelayReq message (all clock types)
#define ETH_DMA_PTP_RDES_PTPMT_DelayResp                 ((uint32_t)0x00000400)  ///< DelayResp message (all clock types)
#define ETH_DMA_PTP_RDES_PTPMT_PdelayReq_Announce        ((uint32_t)0x00000500)  ///< PdelayReq message (peer-to-peer transparent clock) or Announce message (Ordinary or Boundary clock)
#define ETH_DMA_PTP_RDES_PTPMT_PdelayResp_Manag          ((uint32_t)0x00000600)  ///< PdelayResp message (peer-to-peer transparent clock) or Management message (Ordinary or Boundary clock)
#define ETH_DMA_PTP_RDES_PTPMT_PdelayRespFollowUp_Signal ((uint32_t)0x00000700)  ///< PdelayRespFollowUp message (peer-to-peer transparent clock) or Signaling message (Ordinary or Boundary clock)
#define ETH_DMA_PTP_RDES_IPV6PR                          ((uint32_t)0x00000080)  ///< IPv6 Packet Received
#define ETH_DMA_PTP_RDES_IPV4PR                          ((uint32_t)0x00000040)  ///< IPv4 Packet Received
#define ETH_DMA_PTP_RDES_IPCB                            ((uint32_t)0x00000020)  ///< IP Checksum Bypassed
#define ETH_DMA_PTP_RDES_IPPE                            ((uint32_t)0x00000010)  ///< IP Payload Error
#define ETH_DMA_PTP_RDES_IPHE                            ((uint32_t)0x00000008)  ///< IP Header Error
#define ETH_DMA_PTP_RDES_IPPT                            ((uint32_t)0x00000007)  ///< IP Payload Type
#define ETH_DMA_PTP_RDES_IPPT_UDP                        ((uint32_t)0x00000001)  ///< UDP payload encapsulated in the IP datagram
#define ETH_DMA_PTP_RDES_IPPT_TCP                        ((uint32_t)0x00000002)  ///< TCP payload encapsulated in the IP datagram
#define ETH_DMA_PTP_RDES_IPPT_ICMP                       ((uint32_t)0x00000003)  ///< ICMP payload encapsulated in the IP datagram



#define ETH_DMA_PTP_RDES_TTSL               ((uint32_t)0xFFFFFFFF)              ///< Receive Time Stamp Low
#define ETH_DMA_PTP_RDES_TTSH               ((uint32_t)0xFFFFFFFF)              ///< Receive Time Stamp High
#endif


////////////////////////////////////////////////////////////////////////////////
#define ETH_AutoNegotiation_Enable          ((uint32_t)0x00000001)
#define ETH_AutoNegotiation_Disable         ((uint32_t)0x00000000)

#define ETH_Watchdog_Enable                 ((uint32_t)0x00000000)
#define ETH_Watchdog_Disable                ((uint32_t)0x00800000)

#define ETH_Jabber_Enable                   ((uint32_t)0x00000000)
#define ETH_Jabber_Disable                  ((uint32_t)0x00400000)

#define ETH_InterFrameGap_96Bit             ((uint32_t)0x00000000)              ///< minimum IFG between frames during transmission is 96Bit
#define ETH_InterFrameGap_88Bit             ((uint32_t)0x00020000)              ///< minimum IFG between frames during transmission is 88Bit
#define ETH_InterFrameGap_80Bit             ((uint32_t)0x00040000)              ///< minimum IFG between frames during transmission is 80Bit
#define ETH_InterFrameGap_72Bit             ((uint32_t)0x00060000)              ///< minimum IFG between frames during transmission is 72Bit
#define ETH_InterFrameGap_64Bit             ((uint32_t)0x00080000)              ///< minimum IFG between frames during transmission is 64Bit
#define ETH_InterFrameGap_56Bit             ((uint32_t)0x000A0000)              ///< minimum IFG between frames during transmission is 56Bit
#define ETH_InterFrameGap_48Bit             ((uint32_t)0x000C0000)              ///< minimum IFG between frames during transmission is 48Bit
#define ETH_InterFrameGap_40Bit             ((uint32_t)0x000E0000)              ///< minimum IFG between frames during transmission is 40Bit

#define ETH_CarrierSense_Enable             ((uint32_t)0x00000000)
#define ETH_CarrierSense_Disable            ((uint32_t)0x00010000)

#define ETH_Speed_10M                       ((uint32_t)0x00000000)
#define ETH_Speed_100M                      ((uint32_t)0x00004000)

#define ETH_ReceiveOwn_Enable               ((uint32_t)0x00000000)
#define ETH_ReceiveOwn_Disable              ((uint32_t)0x00002000)

#define ETH_LoopbackMode_Enable             ((uint32_t)0x00001000)
#define ETH_LoopbackMode_Disable            ((uint32_t)0x00000000)

#define ETH_Mode_FullDuplex                 ((uint32_t)0x00000800)
#define ETH_Mode_HalfDuplex                 ((uint32_t)0x00000000)

#define ETH_ChecksumOffload_Enable          ((uint32_t)0x00000400)
#define ETH_ChecksumOffload_Disable         ((uint32_t)0x00000000)

#define ETH_RetryTransmission_Enable        ((uint32_t)0x00000000)
#define ETH_RetryTransmission_Disable       ((uint32_t)0x00000200)

#define ETH_AutomaticPadCRCStrip_Enable     ((uint32_t)0x00000080)
#define ETH_AutomaticPadCRCStrip_Disable    ((uint32_t)0x00000000)

#define ETH_BackOffLimit_10                 ((uint32_t)0x00000000)
#define ETH_BackOffLimit_8                  ((uint32_t)0x00000020)
#define ETH_BackOffLimit_4                  ((uint32_t)0x00000040)
#define ETH_BackOffLimit_1                  ((uint32_t)0x00000060)

#define ETH_DeferralCheck_Enable            ((uint32_t)0x00000010)
#define ETH_DeferralCheck_Disable           ((uint32_t)0x00000000)

#define ETH_ReceiveAll_Enable               ((uint32_t)0x80000000)
#define ETH_ReceiveAll_Disable              ((uint32_t)0x00000000)

#define ETH_SourceAddrFilter_Normal_Enable  ((uint32_t)0x00000200)
#define ETH_SourceAddrFilter_Inverse_Enable ((uint32_t)0x00000300)
#define ETH_SourceAddrFilter_Disable        ((uint32_t)0x00000000)

#define ETH_PassControlFrames_BlockAll                  ((uint32_t)0x00000040)  ///< MAC filters all control frames from reaching the application
#define ETH_PassControlFrames_ForwardAll                ((uint32_t)0x00000080)  ///< MAC forwards all control frames to application even if they fail the Address Filter
#define ETH_PassControlFrames_ForwardPassedAddrFilter   ((uint32_t)0x000000C0)  ///< MAC forwards control frames that pass the Address Filter.

#define ETH_BroadcastFramesReception_Enable             ((uint32_t)0x00000000)
#define ETH_BroadcastFramesReception_Disable            ((uint32_t)0x00000020)

#define ETH_DestinationAddrFilter_Normal                ((uint32_t)0x00000000)
#define ETH_DestinationAddrFilter_Inverse               ((uint32_t)0x00000008)

#define ETH_PromiscuousMode_Enable                      ((uint32_t)0x00000001)
#define ETH_PromiscuousMode_Disable                     ((uint32_t)0x00000000)

#define ETH_MulticastFramesFilter_PerfectHashTable      ((uint32_t)0x00000404)
#define ETH_MulticastFramesFilter_HashTable             ((uint32_t)0x00000004)
#define ETH_MulticastFramesFilter_Perfect               ((uint32_t)0x00000000)
#define ETH_MulticastFramesFilter_None                  ((uint32_t)0x00000010)

#define ETH_UnicastFramesFilter_PerfectHashTable        ((uint32_t)0x00000402)
#define ETH_UnicastFramesFilter_HashTable               ((uint32_t)0x00000002)
#define ETH_UnicastFramesFilter_Perfect                 ((uint32_t)0x00000000)

#define ETH_ZeroQuantaPause_Enable                      ((uint32_t)0x00000000)
#define ETH_ZeroQuantaPause_Disable                     ((uint32_t)0x00000080)

#define ETH_PauseLowThreshold_Minus4                    ((uint32_t)0x00000000)              ///< Pause time minus 4 slot times
#define ETH_PauseLowThreshold_Minus28                   ((uint32_t)0x00000010)              ///< Pause time minus 28 slot times
#define ETH_PauseLowThreshold_Minus144                  ((uint32_t)0x00000020)              ///< Pause time minus 144 slot times
#define ETH_PauseLowThreshold_Minus256                  ((uint32_t)0x00000030)              ///< Pause time minus 256 slot times

#define ETH_UnicastPauseFrameDetect_Enable              ((uint32_t)0x00000008)
#define ETH_UnicastPauseFrameDetect_Disable             ((uint32_t)0x00000000)

#define ETH_ReceiveFlowControl_Enable                   ((uint32_t)0x00000004)
#define ETH_ReceiveFlowControl_Disable                  ((uint32_t)0x00000000)

#define ETH_TransmitFlowControl_Enable                  ((uint32_t)0x00000002)
#define ETH_TransmitFlowControl_Disable                 ((uint32_t)0x00000000)

#define ETH_VLANTagComparison_12Bit                     ((uint32_t)0x00010000)
#define ETH_VLANTagComparison_16Bit                     ((uint32_t)0x00000000)

#define ETH_MAC_FLAG_TST                                ((uint32_t)0x00000200)              ///< Time stamp trigger flag (on MAC)
#define ETH_MAC_FLAG_MMCT                               ((uint32_t)0x00000040)              ///< MMC transmit flag
#define ETH_MAC_FLAG_MMCR                               ((uint32_t)0x00000020)              ///< MMC receive flag
#define ETH_MAC_FLAG_MMC                                ((uint32_t)0x00000010)              ///< MMC flag (on MAC)
#define ETH_MAC_FLAG_PMT                                ((uint32_t)0x00000008)              ///< PMT flag (on MAC)

#define ETH_MAC_IT_TST                                  ((uint32_t)0x00000200)              ///< Time stamp trigger interrupt (on MAC)
#define ETH_MAC_IT_MMCT                                 ((uint32_t)0x00000040)              ///< MMC transmit interrupt
#define ETH_MAC_IT_MMCR                                 ((uint32_t)0x00000020)              ///< MMC receive interrupt
#define ETH_MAC_IT_MMC                                  ((uint32_t)0x00000010)              ///< MMC interrupt (on MAC)
#define ETH_MAC_IT_PMT                                  ((uint32_t)0x00000008)              ///< PMT interrupt (on MAC)

#define ETH_MAC_Address0                                ((uint32_t)0x00000000)
#define ETH_MAC_Address1                                ((uint32_t)0x00000008)
#define ETH_MAC_Address2                                ((uint32_t)0x00000010)
#define ETH_MAC_Address3                                ((uint32_t)0x00000018)

#define ETH_MAC_AddressFilter_SA                        ((uint32_t)0x00000000)
#define ETH_MAC_AddressFilter_DA                        ((uint32_t)0x00000008)

#define ETH_MAC_AddressMask_Byte6                       ((uint32_t)0x20000000)              ///< Mask MAC Address high reg bits [15:8]
#define ETH_MAC_AddressMask_Byte5                       ((uint32_t)0x10000000)              ///< Mask MAC Address high reg bits [7:0]
#define ETH_MAC_AddressMask_Byte4                       ((uint32_t)0x08000000)              ///< Mask MAC Address low reg bits [31:24]
#define ETH_MAC_AddressMask_Byte3                       ((uint32_t)0x04000000)              ///< Mask MAC Address low reg bits [23:16]
#define ETH_MAC_AddressMask_Byte2                       ((uint32_t)0x02000000)              ///< Mask MAC Address low reg bits [15:8]
#define ETH_MAC_AddressMask_Byte1                       ((uint32_t)0x01000000)              ///< Mask MAC Address low reg bits [70]

////////////////////////////////////////////////////////////////////////////////
#define ETH_DMA_TDES_LastSegment                        ((uint32_t)0x40000000)              ///< Last Segment
#define ETH_DMA_TDES_FirstSegment                       ((uint32_t)0x20000000)              ///< First Segment

#define ETH_DMA_TDES_ChecksumByPass                     ((uint32_t)0x00000000)      ///< Checksum engine bypass
#define ETH_DMA_TDES_ChecksumIPV4Header                 ((uint32_t)0x00400000)      ///< IPv4 header checksum insertion
#define ETH_DMA_TDES_ChecksumTCPUDPICMPSegment          ((uint32_t)0x00800000)      ///< TCP/UDP/ICMP checksum insertion. Pseudo header checksum is assumed to be present
#define ETH_DMA_TDES_ChecksumTCPUDPICMPFull             ((uint32_t)0x00C00000)      ///< TCP/UDP/ICMP checksum fully in hardware including pseudo header

#define ETH_DMA_RDES_Buffer1                            ((uint32_t)0x00000000)              ///< DMA Rx Desc Buffer1
#define ETH_DMA_RDES_Buffer2                            ((uint32_t)0x00000001)              ///< DMA Rx Desc Buffer2

#define ETH_DropTCPIPChecksumErrorFrame_Enable          ((uint32_t)0x00000000)
#define ETH_DropTCPIPChecksumErrorFrame_Disable         ((uint32_t)0x04000000)

#define ETH_ReceiveStoreForward_Enable                  ((uint32_t)0x02000000)
#define ETH_ReceiveStoreForward_Disable                 ((uint32_t)0x00000000)

#define ETH_FlushReceivedFrame_Enable                   ((uint32_t)0x00000000)
#define ETH_FlushReceivedFrame_Disable                  ((uint32_t)0x01000000)

#define ETH_TransmitStoreForward_Enable                 ((uint32_t)0x00200000)
#define ETH_TransmitStoreForward_Disable                ((uint32_t)0x00000000)

#define ETH_TransmitThresholdControl_64Bytes            ((uint32_t)0x00000000)      ///< threshold level of the MTL Transmit FIFO is 64 Bytes
#define ETH_TransmitThresholdControl_128Bytes           ((uint32_t)0x00004000)      ///< threshold level of the MTL Transmit FIFO is 128 Bytes
#define ETH_TransmitThresholdControl_192Bytes           ((uint32_t)0x00008000)      ///< threshold level of the MTL Transmit FIFO is 192 Bytes
#define ETH_TransmitThresholdControl_256Bytes           ((uint32_t)0x0000C000)      ///< threshold level of the MTL Transmit FIFO is 256 Bytes
#define ETH_TransmitThresholdControl_40Bytes            ((uint32_t)0x00010000)      ///< threshold level of the MTL Transmit FIFO is 40 Bytes
#define ETH_TransmitThresholdControl_32Bytes            ((uint32_t)0x00014000)      ///< threshold level of the MTL Transmit FIFO is 32 Bytes
#define ETH_TransmitThresholdControl_24Bytes            ((uint32_t)0x00018000)      ///< threshold level of the MTL Transmit FIFO is 24 Bytes
#define ETH_TransmitThresholdControl_16Bytes            ((uint32_t)0x0001C000)      ///< threshold level of the MTL Transmit FIFO is 16 Bytes

#define ETH_ForwardErrorFrames_Enable                   ((uint32_t)0x00000080)
#define ETH_ForwardErrorFrames_Disable                  ((uint32_t)0x00000000)

#define ETH_ForwardUndersizedGoodFrames_Enable          ((uint32_t)0x00000040)
#define ETH_ForwardUndersizedGoodFrames_Disable         ((uint32_t)0x00000000)

#define ETH_ReceiveThresholdControl_64Bytes             ((uint32_t)0x00000000)      ///< threshold level of the MTL Receive FIFO is 64 Bytes
#define ETH_ReceiveThresholdControl_32Bytes             ((uint32_t)0x00000008)      ///< threshold level of the MTL Receive FIFO is 32 Bytes
#define ETH_ReceiveThresholdControl_96Bytes             ((uint32_t)0x00000010)      ///< threshold level of the MTL Receive FIFO is 96 Bytes
#define ETH_ReceiveThresholdControl_128Bytes            ((uint32_t)0x00000018)      ///< threshold level of the MTL Receive FIFO is 128 Bytes

#define ETH_SecondFrameOperate_Enable       ((uint32_t)0x00000004)
#define ETH_SecondFrameOperate_Disable      ((uint32_t)0x00000000)

#define ETH_AddressAlignedBeats_Enable      ((uint32_t)0x02000000)
#define ETH_AddressAlignedBeats_Disable     ((uint32_t)0x00000000)

#define ETH_FixedBurst_Enable               ((uint32_t)0x00010000)
#define ETH_FixedBurst_Disable              ((uint32_t)0x00000000)

#define ETH_RxDMABurstLength_1Beat          ((uint32_t)0x00020000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 1
#define ETH_RxDMABurstLength_2Beat          ((uint32_t)0x00040000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 2
#define ETH_RxDMABurstLength_4Beat          ((uint32_t)0x00080000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 4
#define ETH_RxDMABurstLength_8Beat          ((uint32_t)0x00100000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 8
#define ETH_RxDMABurstLength_16Beat         ((uint32_t)0x00200000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 16
#define ETH_RxDMABurstLength_32Beat         ((uint32_t)0x00400000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 32
#define ETH_RxDMABurstLength_4xPBL_4Beat    ((uint32_t)0x01020000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 4
#define ETH_RxDMABurstLength_4xPBL_8Beat    ((uint32_t)0x01040000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 8
#define ETH_RxDMABurstLength_4xPBL_16Beat   ((uint32_t)0x01080000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 16
#define ETH_RxDMABurstLength_4xPBL_32Beat   ((uint32_t)0x01100000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 32
#define ETH_RxDMABurstLength_4xPBL_64Beat   ((uint32_t)0x01200000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 64
#define ETH_RxDMABurstLength_4xPBL_128Beat  ((uint32_t)0x01400000)              ///< maximum number of beats to be transferred in one RxDMA transaction is 128

#define ETH_TxDMABurstLength_1Beat          ((uint32_t)0x00000100)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 1
#define ETH_TxDMABurstLength_2Beat          ((uint32_t)0x00000200)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 2
#define ETH_TxDMABurstLength_4Beat          ((uint32_t)0x00000400)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 4
#define ETH_TxDMABurstLength_8Beat          ((uint32_t)0x00000800)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 8
#define ETH_TxDMABurstLength_16Beat         ((uint32_t)0x00001000)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 16
#define ETH_TxDMABurstLength_32Beat         ((uint32_t)0x00002000)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 32
#define ETH_TxDMABurstLength_4xPBL_4Beat    ((uint32_t)0x01000100)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 4
#define ETH_TxDMABurstLength_4xPBL_8Beat    ((uint32_t)0x01000200)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 8
#define ETH_TxDMABurstLength_4xPBL_16Beat   ((uint32_t)0x01000400)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 16
#define ETH_TxDMABurstLength_4xPBL_32Beat   ((uint32_t)0x01000800)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 32
#define ETH_TxDMABurstLength_4xPBL_64Beat   ((uint32_t)0x01001000)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 64
#define ETH_TxDMABurstLength_4xPBL_128Beat  ((uint32_t)0x01002000)              ///< maximum number of beats to be transferred in one TxDMA (or both) transaction is 128

#define ETH_DMAArbitration_RoundRobin_RxTx_1_1      ((uint32_t)0x00000000)
#define ETH_DMAArbitration_RoundRobin_RxTx_2_1      ((uint32_t)0x00004000)
#define ETH_DMAArbitration_RoundRobin_RxTx_3_1      ((uint32_t)0x00008000)
#define ETH_DMAArbitration_RoundRobin_RxTx_4_1      ((uint32_t)0x0000C000)
#define ETH_DMAArbitration_RxPriorTx                ((uint32_t)0x00000002)

#define ETH_DMA_FLAG_TST                    ((uint32_t)0x20000000)              ///< Time-stamp trigger interrupt (on DMA)
#define ETH_DMA_FLAG_PMT                    ((uint32_t)0x10000000)              ///< PMT interrupt (on DMA)
#define ETH_DMA_FLAG_MMC                    ((uint32_t)0x08000000)              ///< MMC interrupt (on DMA)
#define ETH_DMA_FLAG_DataTransferError      ((uint32_t)0x00800000)              ///< Error bits 0-Rx DMA, 1-Tx DMA
#define ETH_DMA_FLAG_ReadWriteError         ((uint32_t)0x01000000)              ///< Error bits 0-write trnsf, 1-read transfr
#define ETH_DMA_FLAG_AccessError            ((uint32_t)0x02000000)              ///< Error bits 0-data buffer, 1-desc. access
#define ETH_DMA_FLAG_NIS                    ((uint32_t)0x00010000)              ///< Normal interrupt summary flag
#define ETH_DMA_FLAG_AIS                    ((uint32_t)0x00008000)              ///< Abnormal interrupt summary flag
#define ETH_DMA_FLAG_ER                     ((uint32_t)0x00004000)              ///< Early receive flag
#define ETH_DMA_FLAG_FBE                    ((uint32_t)0x00002000)              ///< Fatal bus error flag
#define ETH_DMA_FLAG_ET                     ((uint32_t)0x00000400)              ///< Early transmit flag
#define ETH_DMA_FLAG_RWT                    ((uint32_t)0x00000200)              ///< Receive watchdog timeout flag
#define ETH_DMA_FLAG_RPS                    ((uint32_t)0x00000100)              ///< Receive process stopped flag
#define ETH_DMA_FLAG_RBU                    ((uint32_t)0x00000080)              ///< Receive buffer unavailable flag
#define ETH_DMA_FLAG_R                      ((uint32_t)0x00000040)              ///< Receive flag
#define ETH_DMA_FLAG_TU                     ((uint32_t)0x00000020)              ///< Underflow flag
#define ETH_DMA_FLAG_RO                     ((uint32_t)0x00000010)              ///< Overflow flag
#define ETH_DMA_FLAG_TJT                    ((uint32_t)0x00000008)              ///< Transmit jabber timeout flag
#define ETH_DMA_FLAG_TBU                    ((uint32_t)0x00000004)              ///< Transmit buffer unavailable flag
#define ETH_DMA_FLAG_TPS                    ((uint32_t)0x00000002)              ///< Transmit process stopped flag
#define ETH_DMA_FLAG_T                      ((uint32_t)0x00000001)              ///< Transmit flag

#define ETH_DMA_IT_TST                      ((uint32_t)0x20000000)              ///< Time-stamp trigger interrupt (on DMA)
#define ETH_DMA_IT_PMT                      ((uint32_t)0x10000000)              ///< PMT interrupt (on DMA)
#define ETH_DMA_IT_MMC                      ((uint32_t)0x08000000)              ///< MMC interrupt (on DMA)
#define ETH_DMA_IT_NIS                      ((uint32_t)0x00010000)              ///< Normal interrupt summary
#define ETH_DMA_IT_AIS                      ((uint32_t)0x00008000)              ///< Abnormal interrupt summary
#define ETH_DMA_IT_ER                       ((uint32_t)0x00004000)              ///< Early receive interrupt
#define ETH_DMA_IT_FBE                      ((uint32_t)0x00002000)              ///< Fatal bus error interrupt
#define ETH_DMA_IT_ET                       ((uint32_t)0x00000400)              ///< Early transmit interrupt
#define ETH_DMA_IT_RWT                      ((uint32_t)0x00000200)              ///< Receive watchdog timeout interrupt
#define ETH_DMA_IT_RPS                      ((uint32_t)0x00000100)              ///< Receive process stopped interrupt
#define ETH_DMA_IT_RBU                      ((uint32_t)0x00000080)              ///< Receive buffer unavailable interrupt
#define ETH_DMA_IT_R                        ((uint32_t)0x00000040)              ///< Receive interrupt
#define ETH_DMA_IT_TU                       ((uint32_t)0x00000020)              ///< Underflow interrupt
#define ETH_DMA_IT_RO                       ((uint32_t)0x00000010)              ///< Overflow interrupt
#define ETH_DMA_IT_TJT                      ((uint32_t)0x00000008)              ///< Transmit jabber timeout interrupt
#define ETH_DMA_IT_TBU                      ((uint32_t)0x00000004)              ///< Transmit buffer unavailable interrupt
#define ETH_DMA_IT_TPS                      ((uint32_t)0x00000002)              ///< Transmit process stopped interrupt
#define ETH_DMA_IT_T                        ((uint32_t)0x00000001)              ///< Transmit interrupt

#define ETH_DMA_TransmitProcess_Stopped     ((uint32_t)0x00000000)              ///< Stopped - Reset or Stop Tx Command issued
#define ETH_DMA_TransmitProcess_Fetching    ((uint32_t)0x00100000)              ///< Running - fetching the Tx descriptor
#define ETH_DMA_TransmitProcess_Waiting     ((uint32_t)0x00200000)              ///< Running - waiting for status
#define ETH_DMA_TransmitProcess_Reading     ((uint32_t)0x00300000)              ///< Running - reading the data from host memory
#define ETH_DMA_TransmitProcess_Suspended   ((uint32_t)0x00600000)              ///< Suspended - Tx Descriptor unavailable
#define ETH_DMA_TransmitProcess_Closing     ((uint32_t)0x00700000)              ///< Running - closing Rx descriptor

#define ETH_DMA_ReceiveProcess_Stopped      ((uint32_t)0x00000000)              ///< Stopped - Reset or Stop Rx Command issued
#define ETH_DMA_ReceiveProcess_Fetching     ((uint32_t)0x00020000)              ///< Running - fetching the Rx descriptor
#define ETH_DMA_ReceiveProcess_Waiting      ((uint32_t)0x00060000)              ///< Running - waiting for packet
#define ETH_DMA_ReceiveProcess_Suspended    ((uint32_t)0x00080000)              ///< Suspended - Rx Descriptor unavailable
#define ETH_DMA_ReceiveProcess_Closing      ((uint32_t)0x000A0000)              ///< Running - closing descriptor
#define ETH_DMA_ReceiveProcess_Queuing      ((uint32_t)0x000E0000)              ///< Running - queuing the receive frame into host memory

#define ETH_DMA_Overflow_RxFIFOCounter      ((uint32_t)0x10000000)              ///< Overflow bit for FIFO overflow counter
#define ETH_DMA_Overflow_MissedFrameCounter ((uint32_t)0x00010000)              ///< Overflow bit for missed frame counter

////////////////////////////////////////////////////////////////////////////////
#define ETH_PMT_FLAG_WUFFRPR                ((uint32_t)0x80000000)              ///< Wake-Up Frame Filter Register Pointer Reset
#define ETH_PMT_FLAG_WUFR                   ((uint32_t)0x00000040)              ///< Wake-Up Frame Received
#define ETH_PMT_FLAG_MPR                    ((uint32_t)0x00000020)              ///< Magic Packet Received

////////////////////////////////////////////////////////////////////////////////
#define ETH_MMC_IT_TGF                      ((uint32_t)0x00200000)              ///< When Tx good frame counter reaches half the maximum value
#define ETH_MMC_IT_TGFMSC                   ((uint32_t)0x00008000)              ///< When Tx good multi col counter reaches half the maximum value
#define ETH_MMC_IT_TGFSC                    ((uint32_t)0x00004000)              ///< When Tx good single col counter reaches half the maximum value

#define ETH_MMC_IT_RGUF                     ((uint32_t)0x10020000)              ///< When Rx good unicast frames counter reaches half the maximum value
#define ETH_MMC_IT_RFAE                     ((uint32_t)0x10000040)              ///< When Rx alignment error counter reaches half the maximum value
#define ETH_MMC_IT_RFCE                     ((uint32_t)0x10000020)              ///< When Rx crc error counter reaches half the maximum value

#define ETH_MMCCR                           ((uint32_t)0x00000100)              ///< MMC CR register
#define ETH_MMCRIR                          ((uint32_t)0x00000104)              ///< MMC RIR register
#define ETH_MMCTIR                          ((uint32_t)0x00000108)              ///< MMC TIR register
#define ETH_MMCRIMR                         ((uint32_t)0x0000010C)              ///< MMC RIMR register
#define ETH_MMCTIMR                         ((uint32_t)0x00000110)              ///< MMC TIMR register
#define ETH_MMCTGFSCCR                      ((uint32_t)0x0000014C)              ///< MMC TGFSCCR register
#define ETH_MMCTGFMSCCR                     ((uint32_t)0x00000150)              ///< MMC TGFMSCCR register
#define ETH_MMCTGFCR                        ((uint32_t)0x00000168)              ///< MMC TGFCR register
#define ETH_MMCRFCECR                       ((uint32_t)0x00000194)              ///< MMC RFCECR register
#define ETH_MMCRFAECR                       ((uint32_t)0x00000198)              ///< MMC RFAECR register
#define ETH_MMCRGUFCR                       ((uint32_t)0x000001C4)              ///< MMC RGUFCR register

////////////////////////////////////////////////////////////////////////////////
#define ETH_PTP_FineUpdate                  ((uint32_t)0x00000001)              ///< Fine Update method
#define ETH_PTP_CoarseUpdate                ((uint32_t)0x00000000)              ///< Coarse Update method

#define ETH_PTP_FLAG_TSARU                  ((uint32_t)0x00000020)              ///< Addend Register Update
#define ETH_PTP_FLAG_TSITE                  ((uint32_t)0x00000010)              ///< Time Stamp Interrupt Trigger
#define ETH_PTP_FLAG_TSSTU                  ((uint32_t)0x00000008)              ///< Time Stamp Update
#define ETH_PTP_FLAG_TSSTI                  ((uint32_t)0x00000004)              ///< Time Stamp Initialize

#define ETH_PTP_FLAG_TSTTR                  ((uint32_t)0x10000002)              ///< Time stamp target time reached
#define ETH_PTP_FLAG_TSSO                   ((uint32_t)0x10000001)              ///< Time stamp seconds overflow

#define ETH_PTP_PositiveTime                ((uint32_t)0x00000000)              ///< Positive time value
#define ETH_PTP_NegativeTime                ((uint32_t)0x80000000)              ///< Negative time value

#define ETH_PTPTSCR                         ((uint32_t)0x00000700)              ///< PTP TSCR register
#define ETH_PTPSSIR                         ((uint32_t)0x00000704)              ///< PTP SSIR register
#define ETH_PTPTSHR                         ((uint32_t)0x00000708)              ///< PTP TSHR register
#define ETH_PTPTSLR                         ((uint32_t)0x0000070C)              ///< PTP TSLR register
#define ETH_PTPTSHUR                        ((uint32_t)0x00000710)              ///< PTP TSHUR register
#define ETH_PTPTSLUR                        ((uint32_t)0x00000714)              ///< PTP TSLUR register
#define ETH_PTPTSAR                         ((uint32_t)0x00000718)              ///< PTP TSAR register
#define ETH_PTPTTHR                         ((uint32_t)0x0000071C)              ///< PTP TTHR register
#define ETH_PTPTTLR                         ((uint32_t)0x00000720)              ///< PTP TTLR register

#define ETH_PTPTSSR                         ((uint32_t)0x00000728)              ///< PTP TSSR register

#define ETH_PTP_OrdinaryClock               ((uint32_t)0x00000000)              ///< Ordinary Clock
#define ETH_PTP_BoundaryClock               ((uint32_t)0x00010000)              ///< Boundary Clock
#define ETH_PTP_EndToEndTransparentClock    ((uint32_t)0x00020000)              ///< End To End Transparent Clock
#define ETH_PTP_PeerToPeerTransparentClock  ((uint32_t)0x00030000)              ///< Peer To Peer Transparent Clock

#define ETH_PTP_SnapshotMasterMessage               ((uint32_t)0x00008000)      ///< Time stamp snapshot for message relevant to master enable
#define ETH_PTP_SnapshotEventMessage                ((uint32_t)0x00004000)      ///< Time stamp snapshot for event message enable
#define ETH_PTP_SnapshotIPV4Frames                  ((uint32_t)0x00002000)      ///< Time stamp snapshot for IPv4 frames enable
#define ETH_PTP_SnapshotIPV6Frames                  ((uint32_t)0x00001000)      ///< Time stamp snapshot for IPv6 frames enable
#define ETH_PTP_SnapshotPTPOverEthernetFrames       ((uint32_t)0x00000800)      ///< Time stamp snapshot for PTP over ethernet frames enable
#define ETH_PTP_SnapshotAllReceivedFrames           ((uint32_t)0x00000100)      ///< Time stamp snapshot for all received frames enable

#define ETH_MAC_ADDR_HBASE      (ETH_BASE + 0x40)                               ///< ETHERNET MAC address high offset
#define ETH_MAC_ADDR_LBASE      (ETH_BASE + 0x44)                               ///< ETHERNET MAC address low offset

#define MACMIIAR_CR_MASK    ((uint32_t)0xFFFFFFE3)

#define MACCR_CLEAR_MASK    ((uint32_t)0xFF20810F)
#define MACFCR_CLEAR_MASK   ((uint32_t)0x0000FF41)
#define DMAOMR_CLEAR_MASK   ((uint32_t)0xF8DE3F23)



GLOBAL __IO ETH_DMADESCTypeDef*  DMATxDescToSet;
GLOBAL __IO ETH_DMADESCTypeDef*  DMARxDescToGet;

GLOBAL ETH_DMA_Rx_Frame_infos RX_Frame_Descriptor;
GLOBAL __IO ETH_DMA_Rx_Frame_infos* DMA_RX_FRAME_infos;
GLOBAL __IO uint32_t Frame_Rx_index;

#undef GLOBAL

void ETH_DeInit(void);
void ETH_StructInit(ETH_InitTypeDef* ptr);
//uint32_t ETH_Init(ETH_InitTypeDef* ptr, uint16_t phy_addr);
void ETH_Start(void);
void ETH_Stop(void);
void ETH_MACTransmissionCmd(FunctionalState sta);
void ETH_MACReceptionCmd(FunctionalState sta);
FlagStatus ETH_GetFlowControlBusyStatus(void);
void ETH_InitiatePauseControlFrame(void);
void ETH_BackPressureActivationCmd(FunctionalState sta);
void ETH_MACAddressConfig(uint32_t reg_addr, uint8_t* mac_addr);
void ETH_GetMACAddress(uint32_t reg_addr, uint8_t* mac_addr);
void ETH_MACAddressPerfectFilterCmd(uint32_t reg_addr, FunctionalState sta);
void ETH_MACAddressFilterConfig(uint32_t reg_addr, uint32_t sta);
void ETH_MACAddressMaskBytesFilterConfig(uint32_t reg_addr, uint32_t mask_byte);
FrameTypeDef ETH_Get_Received_Frame(void);
FrameTypeDef ETH_Get_Received_Frame_interrupt(void);
uint32_t ETH_Prepare_Transmit_Descriptors(u16 len);
void ETH_DMARxDescChainInit(ETH_DMADESCTypeDef* ptr_desc, uint8_t* buf, uint32_t cnt);
uint32_t ETH_CheckFrameReceived(void);
void ETH_DMATxDescChainInit(ETH_DMADESCTypeDef* ptr_desc, uint8_t* buf, uint32_t cnt);
FlagStatus ETH_GetDMATxDescFlagStatus(ETH_DMADESCTypeDef* ptr_desc, uint32_t flag);
uint32_t ETH_GetDMATxDescCollisionCount(ETH_DMADESCTypeDef* ptr_desc);
void ETH_SetDMATxDescOwnBit(ETH_DMADESCTypeDef* ptr_desc);
void ETH_DMATxDescTransmitITConfig(ETH_DMADESCTypeDef* ptr_desc, FunctionalState sta);
void ETH_DMATxDescFrameSegmentConfig(ETH_DMADESCTypeDef* ptr_desc, uint32_t val);
void ETH_DMATxDescChecksumInsertionConfig(ETH_DMADESCTypeDef* ptr_desc, uint32_t val);
void ETH_DMATxDescCRCCmd(ETH_DMADESCTypeDef* ptr_desc, FunctionalState sta);
void ETH_DMATxDescSecondAddressChainedCmd(ETH_DMADESCTypeDef* ptr_desc, FunctionalState sta);
void ETH_DMATxDescShortFramePaddingCmd(ETH_DMADESCTypeDef* ptr_desc, FunctionalState sta);
void ETH_DMATxDescBufferSizeConfig(ETH_DMADESCTypeDef* ptr_desc, uint32_t buf1_size, uint32_t buf2_size);
FlagStatus ETH_GetDMARxDescFlagStatus(ETH_DMADESCTypeDef* ptr_desc, uint32_t flag);
void ETH_SetDMARxDescOwnBit(ETH_DMADESCTypeDef* ptr_desc);
uint32_t ETH_GetDMARxDescFrameLength(ETH_DMADESCTypeDef* ptr_desc);
void ETH_DMARxDescReceiveITConfig(ETH_DMADESCTypeDef* ptr_desc, FunctionalState sta);
uint32_t ETH_GetDMARxDescBufferSize(ETH_DMADESCTypeDef* ptr_desc, uint32_t buf);
uint32_t ETH_GetRxPktSize(ETH_DMADESCTypeDef* ptr_desc);
void ETH_SoftwareReset(void);
FlagStatus ETH_GetSoftwareResetStatus(void);
FlagStatus ETH_GetDMAFlagStatus(uint32_t flag);
void ETH_DMAClearFlag(uint32_t flag);
void ETH_DMAITConfig(uint32_t it, FunctionalState sta);
ITStatus ETH_GetDMAITStatus(uint32_t it);
void ETH_DMAClearITPendingBit(uint32_t it);
uint32_t ETH_GetTransmitProcessState(void);
uint32_t ETH_GetReceiveProcessState(void);
void ETH_FlushTransmitFIFO(void);
FlagStatus ETH_GetFlushTransmitFIFOStatus(void);
void ETH_DMATransmissionCmd(FunctionalState sta);
void ETH_DMAReceptionCmd(FunctionalState sta);
FlagStatus ETH_GetDMAOverflowStatus(uint32_t val);
uint32_t ETH_GetRxOverflowMissedFrameCounter(void);
uint32_t ETH_GetBufferUnavailableMissedFrameCounter(void);
uint32_t ETH_GetCurrentTxDescStartAddress(void);
uint32_t ETH_GetCurrentRxDescStartAddress(void);
uint32_t ETH_GetCurrentTxBufferAddress(void);
uint32_t ETH_GetCurrentRxBufferAddress(void);
void ETH_ResumeDMATransmission(void);
void ETH_ResumeDMAReception(void);
void ETH_SetReceiveWatchdogTimer(uint8_t val);
uint16_t ETH_ReadPHYRegister(uint16_t addr, uint16_t reg);
uint16_t ETH_WritePHYRegister(uint16_t addr, uint16_t reg, uint16_t val);
void ETH_ResetWakeUpFrameFilterRegisterPointer(void);
void ETH_SetWakeUpFrameFilterRegister(uint32_t* buf);
void ETH_GlobalUnicastWakeUpCmd(FunctionalState sta);
FlagStatus ETH_GetPMTFlagStatus(uint32_t flag);
void ETH_WakeUpFrameDetectionCmd(FunctionalState sta);
void ETH_MagicPacketDetectionCmd(FunctionalState sta);
void ETH_PowerDownCmd(FunctionalState sta);
void ETH_MMCCounterFullPreset(void);
void ETH_MMCCounterHalfPreset(void);
void ETH_MMCCounterFreezeCmd(FunctionalState sta);
void ETH_MMCResetOnReadCmd(FunctionalState sta);
void ETH_MMCCounterRolloverCmd(FunctionalState sta);
void ETH_MMCCountersReset(void);
void ETH_MMCITConfig(uint32_t it, FunctionalState sta);
ITStatus ETH_GetMMCITStatus(uint32_t it);
uint32_t ETH_GetMMCRegister(uint32_t reg);


uint32_t ETH_Init(ETH_InitTypeDef* ptr);
void exETH_MDCClock();

#endif
