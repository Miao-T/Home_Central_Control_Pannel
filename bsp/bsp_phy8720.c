////////////////////////////////////////////////////////////////////////////////
/// @file     BSP_ETH.C
/// @author   AE Team
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE LED BSP LAYER FUNCTIONS.
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
#define _BSP_PHY8720_C_

// Files includes  -------------------------------------------------------------
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_eth.h"
#include "hal_nvic.h"

#include "bsp.h"
#include "common.h"

#include "bsp_eth.h"
#include "bsp_phy8720.h"

/* -----------------------------------------------------------------------------
----------  P e r i p h e r a l s    i n i t i a l i z a t i o n   -------------
----------------------------------------------------------------------------- */
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Board_Support_Package
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_BSP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BEEP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
void closePhy_8720(u16 phyAddr)
{
    ETH_WritePHYRegister(phyAddr, PHY_BCR, PHY_Powerdown);
}

////////////////////////////////////////////////////////////////////////////////
void changeAutoMDIX(u16 phyAddr, u8 mode)
{
    u16 temp = ETH_ReadPHYRegister(phyAddr, 27);

    temp |= 1 << 15;                                    // Disable AutoMDIX
    (mode) ? (temp &= ~(1 << 13)) : (temp |= 1 << 13);  // 0:Tx=Tx, 1:Tx=Rx
    ETH_WritePHYRegister(phyAddr, 27, temp);
}

////////////////////////////////////////////////////////////////////////////////
void initPhy_8720(u16 phy_addr)
{
    if (ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
        ETH_WritePHYRegister(phy_addr, PHY_BCR, PHY_AutoNegotiation);
    else
        ETH_WritePHYRegister(phy_addr, PHY_BCR, ((u16)(ETH_InitStructure.ETH_Mode >> 3) | (u16)(ETH_InitStructure.ETH_Speed >> 1) ));
    
    (ETH_Speed_10M == ETH_InitStructure.ETH_Speed) ? (SYSCFG->CFGR2 &= ~(1 << 21)) : (SYSCFG->CFGR2 |= (1 << 21));
}

////////////////////////////////////////////////////////////////////////////////
void changePhy(u8 mode)
{
    switch(mode){
        case 0:
        initPhy_8720(PHY_ADDRESS_LAN8720_A);
        closePhy_8720(PHY_ADDRESS_LAN8720_B);
        break;
        case 1:
        initPhy_8720(PHY_ADDRESS_LAN8720_B);
        closePhy_8720(PHY_ADDRESS_LAN8720_A);
        break;
        default:
        closePhy_8720(PHY_ADDRESS_LAN8720_A);
        closePhy_8720(PHY_ADDRESS_LAN8720_B);
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
void BSP_PHY8720_Configure(u8 mode)
{
    changePhy(mode);
}


/// @}

/// @}

/// @}

