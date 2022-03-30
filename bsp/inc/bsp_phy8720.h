////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_BEEP.H
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE CONTAINS ALL THE FUNCTIONS PROTOTYPES FOR THE LED
///           BSP LAYER.
////////////////////////////////////////////////////////////////////////////////
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

// Define to prevent recursive inclusion  --------------------------------------
#ifndef __BSP_PHY8720_H
#define __BSP_PHY8720_H

#include "hal_eth.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_BSP
/// @brief LED BSP modules
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Types
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Constants
/// @{

#define PHY_ADDRESS_LAN8720_A       ((u16) 0x0)
#define PHY_ADDRESS_LAN8720_B       ((u16) 0x1)


////////////////////////////////////////////////////////////////////////////////
#define PHY_READ_TIMEOUT                    ((uint32_t)0x0004FFFF)
#define PHY_WRITE_TIMEOUT                   ((uint32_t)0x0004FFFF)

#define PHY_BCR                             0                                   ///< Transceiver Basic Control Register
#define PHY_BSR                             1                                   ///< Transceiver Basic Status Register
#define PHY_MCSR                            17                                   ///< Transceiver Basic Status Register

#define PHY_Reset                           ((uint16_t)0x8000)                  ///< PHY Reset
#define PHY_Loopback                        ((uint16_t)0x4000)                  ///< Select loop-back mode
#define PHY_FULLDUPLEX_100M                 ((uint16_t)0x2100)                  ///< Set the full-duplex mode at 100 Mb/s
#define PHY_HALFDUPLEX_100M                 ((uint16_t)0x2000)                  ///< Set the half-duplex mode at 100 Mb/s
#define PHY_FULLDUPLEX_10M                  ((uint16_t)0x0100)                  ///< Set the full-duplex mode at 10 Mb/s
#define PHY_HALFDUPLEX_10M                  ((uint16_t)0x0000)                  ///< Set the half-duplex mode at 10 Mb/s
#define PHY_AutoNegotiation                 ((uint16_t)0x1000)                  ///< Enable auto-negotiation function
#define PHY_Restart_AutoNegotiation         ((uint16_t)0x0200)                  ///< Restart auto-negotiation function
#define PHY_Powerdown                       ((uint16_t)0x0800)                  ///< Select the power down mode
#define PHY_Isolate                         ((uint16_t)0x0400)                  ///< Isolate PHY from MII

#define PHY_AutoNego_Complete               ((uint16_t)0x0020)                  ///< Auto-Negotiation process completed
#define PHY_Linked_Status                   ((uint16_t)0x0004)                  ///< Valid link established
#define PHY_Jabber_detection                ((uint16_t)0x0002)                  ///< Jabber condition detected
#define PHY_EnergyDetectPD                  ((u16)1 << 13)                      ///< 

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Variables
/// @{
#ifdef _BSP_PHY8720_C_

#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL bool phyANEG_Flag;
GLOBAL bool phyFull_Flag;
GLOBAL bool phy100M_Flag;
GLOBAL bool phyLink_Flag;

#undef GLOBAL

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @defgroup LED_Exported_Functions
/// @{

void changePhy(u8 mode);
void changeAutoMDIX(u16 phyAddr, u8 mode);
void PHY8720_GPIO_Config();
void PHY_LoopBackCmd(uint16_t addr, FunctionalState sta);
void writePhyConfig(u16 phy_addr);
void checkPhyStatus(u16 phy_addr);
void BSP_PHY8720_Configure(u8 mode);


/// @}



/// @}

/// @}

////////////////////////////////////////////////////////////////////////////////
#endif /*__BSP_BEEP_H */
////////////////////////////////////////////////////////////////////////////////
