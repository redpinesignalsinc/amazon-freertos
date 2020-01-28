 /*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "I2C.h"
#if RTE_I2C0 
#define ARM_I2C_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* driver version */
#if ((defined(RTE_Drivers_I2C0) || \
      defined(RTE_Drivers_I2C1) || \
      defined(RTE_Drivers_I2C2))   \
     && !RTE_I2C0                  \
     && !RTE_I2C1                  \
     && !RTE_I2C2)

#error "I2C not configured in RTE_Device.h!"
#endif
/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_I2C_API_VERSION,
    ARM_I2C_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_I2C_CAPABILITIES DriverCapabilities = {
    1  /* supports 10-bit addressing */
};

#if (RTE_I2C0)
/* I2C0 Control Information */
static I2C_CTRL I2C0_Ctrl = { 0 };

/* I2C0 Resources */
static I2C_RESOURCES I2C0_Resources = {
  (I2C0_Type *)I2C0,
  I2C0_IRQn,  
  &I2C0_Ctrl
};
#endif /* RTE_I2C0 */


#if (RTE_I2C1)
/* I2C1 Control Information */
I2C_CTRL I2C1_Ctrl = { 0 };

/* I2C1 Resources */
static I2C_RESOURCES I2C1_Resources = {
  (I2C0_Type *)I2C1,
  I2C1_IRQn,
  &I2C1_Ctrl
};
#endif /* RTE_I2C1 */

#if (RTE_I2C2)
/* I2C2 Control Information */
static I2C_CTRL I2C2_Ctrl = { 0 };

/* I2C2 Resources */
static I2C_RESOURCES I2C2_Resources = {
  (I2C0_Type *)I2C2,
  I2C2_IRQn,
  &I2C2_Ctrl
};
#endif /* RTE_I2C2 */


///////////////////////////////////////////////////////////////
//                         Functions                         //
///////////////////////////////////////////////////////////////

static ARM_DRIVER_VERSION I2C_GetVersion(void)
{
  return DriverVersion;
}

static ARM_I2C_CAPABILITIES I2C_GetCapabilities(void)
{
  return DriverCapabilities;
}

static void I2C_EnableDisableInterrupt(I2C_RESOURCES *i2c, uint32_t flag, uint32_t intFlags)
{
	uint32_t maskReg =0;
  
  if(flag == 1)
  {
    i2c->reg->IC_INTR_MASK |= intFlags;
  }
  else
  {
		maskReg = i2c->reg->IC_INTR_MASK;
    i2c->reg->IC_INTR_MASK = (maskReg &(~intFlags));	
  }    
}

static int32_t I2Cx_Initialize (ARM_I2C_SignalEvent_t cb_event, I2C_RESOURCES *i2c) 
{
  /*Pin muxing */
#if(RTE_I2C0)
  RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI1);
  if(i2c->reg == I2C0)
	{
    if(RTE_I2C0_SCL_PIN > 63)
    {
		  RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2C0_SCL_PIN-64));
		  RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2C0_SCL_PORT,(uint8_t)(RTE_I2C0_SCL_PIN-64),6);
		  RSI_EGPIO_PadSelectionEnable(RTE_I2C0_SCL_PAD);
	    //configure DIN0 pin
		  RSI_EGPIO_SetPinMux(EGPIO,RTE_I2C0_SCL_PORT,RTE_I2C0_SCL_PIN,RTE_I2C0_SCL_MUX);
	  }
    else
    {
      /* SCL */
      if(RTE_I2C0_SCL_PIN == 25)
      {
        egpio_host_pads_gpio_mode_enable(RTE_I2C0_SCL_PIN);
        RSI_EGPIO_PadReceiverEnable(RTE_I2C0_SCL_PIN);
        RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C0_SCL_PORT , RTE_I2C0_SCL_PIN ,RTE_I2C0_SCL_MUX);        
      }
      else
      {
        RSI_EGPIO_PadSelectionEnable(RTE_I2C0_SCL_PAD);
        RSI_EGPIO_PadReceiverEnable(RTE_I2C0_SCL_PIN);
        RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C0_SCL_PORT , RTE_I2C0_SCL_PIN ,RTE_I2C0_SCL_MUX);
      }           
    }
    if(RTE_I2C0_SDA_PIN > 63)
    {
		  RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2C0_SDA_PIN-64));
		  RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2C0_SDA_PORT,(uint8_t)(RTE_I2C0_SDA_PIN-64),6);
		  RSI_EGPIO_PadSelectionEnable(RTE_I2C0_SDA_PAD);
	    //configure DIN0 pin
		  RSI_EGPIO_SetPinMux(EGPIO,RTE_I2C0_SDA_PORT,RTE_I2C0_SDA_PIN,RTE_I2C0_SDA_MUX);
	  }
    else
    {
      /* SDA */
      if(RTE_I2C0_SDA_PIN == 26)
      {
        egpio_host_pads_gpio_mode_enable(RTE_I2C0_SDA_PIN); 
        RSI_EGPIO_PadReceiverEnable(RTE_I2C0_SDA_PIN);
        RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C0_SDA_PORT , RTE_I2C0_SDA_PIN ,RTE_I2C0_SDA_MUX); 
      }
      else
      {
        RSI_EGPIO_PadSelectionEnable(RTE_I2C0_SDA_PAD);
        RSI_EGPIO_PadReceiverEnable(RTE_I2C0_SDA_PIN);
        RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C0_SDA_PORT , RTE_I2C0_SDA_PIN ,RTE_I2C0_SDA_MUX);   
      }              
    }		
	}
#endif
#if(RTE_I2C1) 
  RSI_PS_M4ssPeriPowerUp(M4SS_PWRGATE_ULP_PERI3);
  if(i2c->reg == I2C1)
	{
    if(RTE_I2C1_SCL_PIN > 63)
    {
		  RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2C1_SCL_PIN-64));
		  RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2C1_SCL_PORT,(uint8_t)(RTE_I2C1_SCL_PIN-64),6);
		  RSI_EGPIO_PadSelectionEnable(RTE_I2C1_SCL_PAD);
	    //configure DIN0 pin
		  RSI_EGPIO_SetPinMux(EGPIO,RTE_I2C1_SCL_PORT,RTE_I2C1_SCL_PIN,RTE_I2C1_SCL_MUX);
	  }
    else
    {
      if(RTE_I2C1_SCL_PIN == 28)
      {
        egpio_host_pads_gpio_mode_enable(RTE_I2C1_SCL_PIN); 
        RSI_EGPIO_PadReceiverEnable(RTE_I2C1_SCL_PIN);
	      RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C1_SCL_PORT , RTE_I2C1_SCL_PIN ,RTE_I2C1_SCL_MUX); 
      } 
      else
      {
        /* SCL */
        RSI_EGPIO_PadSelectionEnable(RTE_I2C1_SCL_PAD);
        RSI_EGPIO_PadReceiverEnable(RTE_I2C1_SCL_PIN);
	      RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C1_SCL_PORT , RTE_I2C1_SCL_PIN ,RTE_I2C1_SCL_MUX); 
      }         
    }
		if(RTE_I2C1_SDA_PIN > 63)
    {
		  RSI_EGPIO_UlpPadReceiverEnable((uint8_t)(RTE_I2C1_SDA_PIN-64));
		  RSI_EGPIO_SetPinMux(EGPIO1,RTE_I2C1_SDA_PORT,(uint8_t)(RTE_I2C1_SDA_PIN-64),6);
		  RSI_EGPIO_PadSelectionEnable(RTE_I2C1_SDA_PAD);
	    //configure DIN0 pin
		  RSI_EGPIO_SetPinMux(EGPIO,RTE_I2C1_SDA_PORT,RTE_I2C1_SDA_PIN,RTE_I2C1_SDA_MUX);
	  }
    else
    {
      if(RTE_I2C1_SDA_PIN == 29)
      {
        egpio_host_pads_gpio_mode_enable(RTE_I2C1_SDA_PIN); 
        RSI_EGPIO_PadReceiverEnable(RTE_I2C1_SDA_PIN);
		    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C1_SDA_PORT , RTE_I2C1_SDA_PIN ,RTE_I2C1_SDA_MUX); 
      } 
      else
      {
         /* SDA */
        RSI_EGPIO_PadSelectionEnable(RTE_I2C1_SDA_PAD);
	      RSI_EGPIO_PadReceiverEnable(RTE_I2C1_SDA_PIN);
		    RSI_EGPIO_SetPinMux(EGPIO ,RTE_I2C1_SDA_PORT , RTE_I2C1_SDA_PIN ,RTE_I2C1_SDA_MUX);
      }        
    }		 
	}
#endif
#if(RTE_I2C2)
  RSI_PS_UlpssPeriPowerUp(ULPSS_PWRGATE_ULP_I2C);  
  if(i2c->reg == I2C2)
	{
		/* SCL */
	  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2C2_SCL_PIN);
	  RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2C2_SCL_PORT , RTE_I2C2_SCL_PIN ,RTE_I2C2_SCL_MUX);
		/* SDA */
	  RSI_EGPIO_UlpPadReceiverEnable(RTE_I2C2_SDA_PIN);
		RSI_EGPIO_SetPinMux(EGPIO1 ,RTE_I2C2_SDA_PORT , RTE_I2C2_SDA_PIN ,RTE_I2C2_SDA_MUX);   
	}
#endif
   
  if (i2c->ctrl->flags & I2C_FLAG_INIT) 
  { 
    return ARM_DRIVER_OK; 
  }
  /* Reset Run-Time information structure */
  memset (i2c->ctrl, 0x00, sizeof (I2C_CTRL));
  
  i2c->ctrl->cb_event = cb_event;
  i2c->ctrl->flags    = I2C_FLAG_INIT;
  
  if(i2c->reg == I2C0)
  {
    RSI_CLK_I2CClkConfig(M4CLK ,1, I2C1_INSTAN);
  }
  if(i2c->reg == I2C1)
  {
    RSI_CLK_I2CClkConfig(M4CLK ,1, I2C2_INSTAN);
  }  
  if(i2c->reg == I2C2)
  {
    RSI_ULPSS_PeripheralEnable(ULPCLK ,ULP_I2C_CLK, ENABLE_STATIC_CLK);
  } 
  /* Disable TX empty interrupt */
  I2C_EnableDisableInterrupt(i2c, 0, RSI_I2C_EVENT_TRANSMIT_EMPTY);
  
  return ARM_DRIVER_OK;
}


static int32_t SetClockRate(I2C_RESOURCES *i2c, uint8_t speed,uint32_t icClk)
{
	uint16_t ss_scl_high =0, ss_scl_low =0;
	uint16_t fs_scl_high =0, fs_scl_low =0;
	uint16_t hs_scl_high =0, hs_scl_low =0;
  volatile uint32_t time_p =0;
    // ic_clk is the clock speed (in MHz) that is being supplied to the
    // DW_apb_i2c device.  The correct clock count values are determined
    // by using this inconjunction with the minimum high and low signal
    // hold times as per the I2C bus specification.
  
    time_p = ((1/(float)icClk)* 1000);
      
    ss_scl_high = ((SS_MIN_SCL_HIGH/time_p)-8);
    ss_scl_low  = ((SS_MIN_SCL_LOW/time_p)-1);
    fs_scl_high = ((FS_MIN_SCL_HIGH/time_p)-8);
    fs_scl_low  = ((FS_MIN_SCL_LOW/time_p)-1);
    hs_scl_high = ((HS_MIN_SCL_HIGH_400PF/time_p)-8);
    hs_scl_low  = ((HS_MIN_SCL_LOW_400PF/time_p)-1);

	switch(speed)
	{
		case ARM_I2C_BUS_SPEED_STANDARD:
		  i2c->reg->IC_SS_SCL_LCNT_b.IC_SS_SCL_LCNT = ss_scl_low;
			i2c->reg->IC_SS_SCL_HCNT_b.IC_SS_SCL_HCNT = ss_scl_high;
		 break;
		case ARM_I2C_BUS_SPEED_FAST:			
			i2c->reg->IC_FS_SCL_LCNT_b.IC_FS_SCL_LCNT = fs_scl_low;
			i2c->reg->IC_FS_SCL_HCNT_b.IC_FS_SCL_HCNT = fs_scl_high;        
		 break;
		case ARM_I2C_BUS_SPEED_HIGH:
			i2c->reg->IC_HS_SCL_LCNT_b.IC_HS_SCL_LCNT = hs_scl_low;
			i2c->reg->IC_HS_SCL_HCNT_b.IC_HS_SCL_HCNT = hs_scl_high;    
		 break;
		default:
			return ARM_DRIVER_ERROR_PARAMETER;
	}
	return ARM_DRIVER_OK;
}


static int32_t I2Cx_Uninitialize (I2C_RESOURCES *i2c)
{
  if(i2c->reg == I2C0)
  {
    RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI1);
    RSI_CLK_I2CClkConfig(M4CLK ,0, I2C1_INSTAN);
  }
  if(i2c->reg == I2C1)
  {
    RSI_PS_M4ssPeriPowerDown(M4SS_PWRGATE_ULP_PERI3);
    RSI_CLK_I2CClkConfig(M4CLK ,0, I2C2_INSTAN);
  }  
  if(i2c->reg == I2C2)
  {
    RSI_PS_UlpssPeriPowerDown(ULPSS_PWRGATE_ULP_I2C);
    RSI_ULPSS_PeripheralDisable(ULPCLK, ULP_I2C_CLK);
  } 
  i2c->ctrl->flags = 0;
  return ARM_DRIVER_OK;
}

static int32_t I2Cx_PowerControl(ARM_POWER_STATE state,I2C_RESOURCES *i2c)
{
  volatile uint32_t conset = 0;

  conset = conset;
  switch (state)
  {
    case ARM_POWER_OFF:
      /* Abort Master/Slave transfer */
      NVIC_DisableIRQ (i2c->i2c_ev_irq);
    
      if (i2c->ctrl->status.busy) 
      {
        /* Master: send STOP to I2C bus  */
        /* Clear all errors and interrupts */
        i2c->reg->IC_DATA_CMD = (1 << 9); 
        conset = i2c->reg->IC_CLR_INTR;	;
      }

      i2c->ctrl->status.busy             = 0U;
      i2c->ctrl->status.mode             = 0U;
      i2c->ctrl->status.direction        = 0U;
      i2c->ctrl->status.general_call     = 0U;
      i2c->ctrl->status.arbitration_lost = 0U;
      i2c->ctrl->status.bus_error        = 0U;

      i2c->ctrl->stalled = 0U;
      i2c->ctrl->snum    = 0U;

      i2c->ctrl->flags  &= ~I2C_FLAG_POWER;

      /* Disable I2C Operation */
      i2c->reg->IC_ENABLE_b.EN = 0;

      /* Disable power to I2C block */
      if(i2c->reg == I2C2)
      {
        RSI_PS_UlpssPeriPowerDown( ULPSS_PWRGATE_ULP_I2C); 
      }
      if(i2c->reg == I2C0)
      {
        RSI_CLK_I2CClkConfig(M4CLK ,0, I2C1_INSTAN);
      }
      if(i2c->reg == I2C1)
      {
        RSI_CLK_I2CClkConfig(M4CLK ,0, I2C2_INSTAN);
      }
      break;

    case ARM_POWER_FULL:      
      if ((i2c->ctrl->flags & I2C_FLAG_INIT)  == 0U) 
      { 
        return ARM_DRIVER_ERROR; 
      }
      if ((i2c->ctrl->flags & I2C_FLAG_POWER) != 0U) 
      { 
        return ARM_DRIVER_OK; 
      }
      /* Enable clock to I2C block */
      if(i2c->reg == I2C0)
      {
        RSI_CLK_I2CClkConfig(M4CLK ,1, I2C1_INSTAN);
      }
      if(i2c->reg == I2C1)
      {
        RSI_CLK_I2CClkConfig(M4CLK ,1, I2C2_INSTAN);
      }  
      if(i2c->reg == I2C2)
      {
        RSI_ULPSS_PeripheralEnable(ULPCLK ,ULP_I2C_CLK, ENABLE_STATIC_CLK);
      } 
      i2c->ctrl->stalled = 0;
      i2c->ctrl->con_aa  = 0;
    
      NVIC_ClearPendingIRQ (i2c->i2c_ev_irq);
      NVIC_EnableIRQ (i2c->i2c_ev_irq);
      i2c->ctrl->flags |= I2C_FLAG_POWER;
      break;
    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}




/**
  \fn          int32_t I2Cx_MasterTransmit (uint32_t       addr,
                                            const uint8_t *data,
                                            uint32_t       num,
                                            bool           xfer_pending,
                                            I2C_RESOURCES *i2c)
  \brief       Start transmitting data as I2C Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to transmit to I2C Slave
  \param[in]   num           Number of data bytes to transmit
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \param[in]   i2c           Pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_MasterTransmit( uint32_t addr, const uint8_t *data, uint32_t num, 
                                    bool xfer_pending, I2C_RESOURCES *i2c )
{
  if (!data || !num) 
  {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (!(i2c->ctrl->flags & I2C_FLAG_SETUP)) 
  {
    /* Driver not yet configured */
    return ARM_DRIVER_ERROR;
  }

  if (i2c->ctrl->status.busy || (i2c->ctrl->stalled & I2C_SLAVE)) 
  {
    /* Transfer operation in progress, or Slave stalled */
    return ARM_DRIVER_ERROR_BUSY;
  }

  NVIC_DisableIRQ (i2c->i2c_ev_irq);

  /* Set control variables */
  i2c->ctrl->sla_rw  = addr;
  i2c->ctrl->pending = xfer_pending;
  i2c->ctrl->data    = (uint8_t *)data;
  i2c->ctrl->num     = num;
  i2c->ctrl->cnt     = 0;

  /* Update driver status */
  i2c->ctrl->status.busy             = 1;
  i2c->ctrl->status.mode             = 1;
  i2c->ctrl->status.direction        = 0;
  i2c->ctrl->status.arbitration_lost = 0;
  i2c->ctrl->status.bus_error        = 0;
        
  if (!i2c->ctrl->stalled) 
  {
    i2c->reg->IC_ENABLE_b.EN = DISABLE;
    i2c->reg->IC_CON_b.IC_SLAVE_DISABLE  = 1;
    i2c->reg->IC_CON_b.IC_RESTART_EN = 1;
		/* TODO: Check this address range */
    if((addr > 0x7F) && (addr < 0x3FF))
    {
      i2c->reg->IC_TAR_b.IC_10BITADDR_MASTER = 0x1; 
    }
		else
    {
      i2c->reg->IC_TAR_b.IC_10BITADDR_MASTER = 0x0;
    }
    i2c->reg->IC_CON_b.MASTER_MODE = 1;
    
#if (I2C_DMA)
    /* Enable DMA tx control */
    i2c->reg->IC_DMA_CR_b.TDMAE = 0x1;
    /* DMA TX thresholds */
    i2c->reg->IC_DMA_TDLR_b.DMATDL = DMA_TX_TL;  
#endif
  }
  /* IO TX thresholds */
  i2c->reg->IC_TX_TL_b.TX_TL = 0x0;
  
	i2c->reg->IC_TAR_b.IC_TAR = addr;
	i2c->reg->IC_ENABLE_b.EN = ENABLE;	
	I2C_EnableDisableInterrupt(i2c, 1, I2C_EVENT_TRANSMIT_EMPTY);
  NVIC_EnableIRQ(i2c->i2c_ev_irq);
	
  return ARM_DRIVER_OK;
}

static int32_t I2Cx_MasterReceive( uint32_t addr, uint8_t *data, uint32_t num, 
                                   bool xfer_pending,I2C_RESOURCES *i2c )
{
   if (!data || !num) 
   {
    /* Invalid parameters */ 
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (!(i2c->ctrl->flags & I2C_FLAG_SETUP)) 
  {
    /* Driver not yet configured */
    return ARM_DRIVER_ERROR;
  }

  if (i2c->ctrl->status.busy || (i2c->ctrl->stalled & I2C_SLAVE)) 
  {
    /* Transfer operation in progress, or Slave stalled */
    return ARM_DRIVER_ERROR_BUSY;
  }

  NVIC_DisableIRQ (i2c->i2c_ev_irq);

  /* Set control variables */
  i2c->ctrl->sla_rw  = (addr & 0x3ff);
  i2c->ctrl->pending = xfer_pending;
  i2c->ctrl->data    = data;
  i2c->ctrl->num     = num;
  i2c->ctrl->cnt     = 0;

  /* Update driver status */
  i2c->ctrl->status.busy             = 1;
  i2c->ctrl->status.mode             = 1;
  i2c->ctrl->status.direction        = 0;
  i2c->ctrl->status.arbitration_lost = 0;
  i2c->ctrl->status.bus_error        = 0;
  
  if (!i2c->ctrl->stalled) 
  {
    i2c->reg->IC_ENABLE_b.EN = DISABLE;
    i2c->reg->IC_CON_b.IC_SLAVE_DISABLE  = 1;
    i2c->reg->IC_CON_b.IC_RESTART_EN = 1;
		if((addr > 0x7F) && (addr < 0x3FF))
    {
      i2c->reg->IC_TAR_b.IC_10BITADDR_MASTER = 0x1; 
    }
		else
    {
      i2c->reg->IC_TAR_b.IC_10BITADDR_MASTER = 0x0;
    }
    i2c->reg->IC_CON_b.MASTER_MODE = 1;
    i2c->reg->IC_TAR_b.IC_TAR = addr;
#if (I2C_DMA)
    /* Enable DMA RX control */
    i2c->reg->IC_DMA_CR_b.RDMAE = 0x1;
    /* DMA RX thresholds */
    i2c->reg->IC_DMA_RDLR_b.DMARDL = DMA_RX_TL;  
#endif
  }
  /* IO RX thresholds */
  i2c->reg->IC_RX_TL_b.RX_TL = 0x0;
  
	i2c->reg->IC_ENABLE_b.EN = ENABLE;
	if((num == 1)&&(!i2c->ctrl->pending))
  {
    i2c->reg->IC_DATA_CMD = (uint16_t)((1 << 8)|(1 << 9));
  }
	else
  {
		i2c->reg->IC_DATA_CMD_b.CMD = 0x01;	
	}

  NVIC_EnableIRQ (i2c->i2c_ev_irq);
  
  return ARM_DRIVER_OK;
}

static int32_t I2Cx_SlaveTransmit(const uint8_t *data, uint32_t num, I2C_RESOURCES *i2c)
{
  if (!data || !num) 
  {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (i2c->ctrl->status.busy || (i2c->ctrl->stalled & (I2C_MASTER | I2C_SLAVE_RX))) 
  {
    /* Transfer operation in progress, Master stalled or Slave receive stalled */
    return ARM_DRIVER_ERROR_BUSY;
  }

  NVIC_DisableIRQ (i2c->i2c_ev_irq);

  /* Set control variables */
  i2c->ctrl->flags &= ~I2C_FLAG_SLAVE_RX;
  i2c->ctrl->sdata  = (uint8_t *)data;
  i2c->ctrl->snum   = num;
  i2c->ctrl->cnt    = 0;

  /* Update driver status */
  i2c->ctrl->status.general_call = 0;
  i2c->ctrl->status.bus_error    = 0;
  
#if (I2C_DMA)
    /* Enable DMA tx control */
    i2c->reg->IC_DMA_CR_b.TDMAE = 0x1;
    /* DMA TX thresholds */
    i2c->reg->IC_DMA_TDLR_b.DMATDL = DMA_TX_TL; 
#endif
    i2c->reg->IC_TX_TL_b.TX_TL = 0x0;

  i2c->reg->IC_INTR_MASK = I2C_EVENT_TRANSMIT_EMPTY;
  NVIC_EnableIRQ (i2c->i2c_ev_irq);

  return ARM_DRIVER_OK;
}

static int32_t I2Cx_SlaveReceive(uint8_t *data, uint32_t num,I2C_RESOURCES *i2c)
{
  if(!data || !num) 
  {
    /* Invalid parameters */ 
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if(i2c->ctrl->status.busy || (i2c->ctrl->stalled & (I2C_MASTER | I2C_SLAVE_TX))) 
  {
    /* Transfer operation in progress, Master stalled or Slave transmit stalled */
    return ARM_DRIVER_ERROR_BUSY;
  }

  NVIC_DisableIRQ (i2c->i2c_ev_irq);

  /* Set control variables */
  i2c->ctrl->flags |= I2C_FLAG_SLAVE_RX;
  i2c->ctrl->sdata  = data;
  i2c->ctrl->snum   = num;
  i2c->ctrl->cnt    = 0;

  /* Update driver status */
  i2c->ctrl->status.general_call = 0;
  i2c->ctrl->status.bus_error    = 0;
	
#if (I2C_DMA)
  /* Enable DMA RX control */
  i2c->reg->IC_DMA_CR_b.RDMAE = 0x1;
  /* DMA RX thresholds */
  i2c->reg->IC_DMA_RDLR_b.DMARDL = DMA_RX_TL; 
#endif
  
  i2c->reg->IC_RX_TL_b.RX_TL = 0x0;
  NVIC_EnableIRQ (i2c->i2c_ev_irq);

  return ARM_DRIVER_OK;  
}

static int32_t I2Cx_GetDataCount(I2C_RESOURCES *i2c)
{
  return (i2c->ctrl->cnt);
}


static uint32_t GetI2CClockFreq (I2C_RESOURCES *i2c) 
{
  return(SystemCoreClock);
}




/**
  \fn          int32_t I2Cx_Control (uint32_t       control,
                                     uint32_t       arg,
                                     I2C_RESOURCES *i2c)
  \brief       Control I2C Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \param[in]   i2c      pointer to I2C resources
  \return      \ref execution_status
*/
static int32_t I2Cx_Control(uint32_t control, uint32_t arg,I2C_RESOURCES *i2c)
{
   uint32_t val =0,clk =0;
  
   if (!(i2c->ctrl->flags & I2C_FLAG_POWER)) 
   {
     /* Driver not powered */
     return ARM_DRIVER_ERROR;
   }  
   switch (control)
   {
		 /* Slave mode only */
     case ARM_I2C_OWN_ADDRESS:
       /* Set Own Slave Address */
       val = (arg);
		   val = (val & 0x3FF);
       if(arg & ARM_I2C_ADDRESS_GC) 
       {
         /* General call enable */
				 i2c->reg->IC_ACK_GENERAL_CALL_b.ACK_GEN_CALL  = 1;
				 i2c->reg->IC_TAR_b.SPECIAL = 0x1;
       }
       i2c->reg->IC_ENABLE_b.EN = DISABLE;			 
       i2c->reg->IC_SAR_b.IC_SAR = val;
			 /* Slave 10bit mode */
       if(arg & ARM_I2C_ADDRESS_10BIT) 
       {
         i2c->reg->IC_CON_b.IC_10BITADDR_SLAVE = 0x1;
       } 
       else 
       {
         i2c->reg->IC_CON_b.IC_10BITADDR_SLAVE = 0x0;
       }         
       i2c->ctrl->con_aa  = val;
			 i2c->reg->IC_CON_b.MASTER_MODE = 0;
       i2c->reg->IC_CON_b.IC_SLAVE_DISABLE  = 0;	
			 i2c->reg->IC_ENABLE_b.EN = ENABLE;
       break;
       
     case ARM_I2C_BUS_SPEED:
       /* Set Bus speed */
       clk = GetI2CClockFreq(i2c);
		   clk = clk/1000000;
       switch (arg)
       {         
         case ARM_I2C_BUS_SPEED_STANDARD:
           /* Standard Speed (100kHz) */
           i2c->reg->IC_CON_b.SPEED = ARM_I2C_BUS_SPEED_STANDARD;
           SetClockRate(i2c , ARM_I2C_BUS_SPEED_STANDARD,clk);             
           break;
         case ARM_I2C_BUS_SPEED_FAST:
           /* Fast Speed     (400kHz) */
           i2c->reg->IC_CON_b.SPEED = ARM_I2C_BUS_SPEED_FAST;
           SetClockRate(i2c , ARM_I2C_BUS_SPEED_FAST,clk);         
           break;
         case ARM_I2C_BUS_SPEED_HIGH:
           /* High Speed    (3.4MHz) */
           i2c->reg->IC_CON_b.SPEED = RSI_I2C_HIGH_SPEED;
           SetClockRate(i2c , ARM_I2C_BUS_SPEED_HIGH,clk);
           break;   
         default:
           return ARM_DRIVER_ERROR_UNSUPPORTED;
       }
			 /* Speed configured, I2C Master active */
       i2c->ctrl->flags |= I2C_FLAG_SETUP;
       break;
       
     case ARM_I2C_BUS_CLEAR:
       NVIC_DisableIRQ (i2c->i2c_ev_irq); 
       NVIC_ClearPendingIRQ (i2c->i2c_ev_irq);
       NVIC_EnableIRQ (i2c->i2c_ev_irq);
     
       /* Bus Clear Sequence */
       i2c->reg->IC_ENABLE_b.EN = 0;
      // i2c->reg->IC_CON_b.BUS_CLEAR_FEATURE_CTRL = 0x1;
       i2c->reg->IC_SCL_STUCK_AT_LOW_TIMEOUT = IC_SCL_STUCK_TIMEOUT;
       i2c->reg->IC_SDA_STUCK_AT_LOW_TIMEOUT = IC_SDA_STUCK_TIMEOUT;
      // i2c->reg->IC_ENABLE_b.EN = 1;     
       return ARM_DRIVER_OK;
       
     case ARM_I2C_ABORT_TRANSFER:
       /* Abort Master/Slave transfer */
       NVIC_DisableIRQ (i2c->i2c_ev_irq);

       i2c->ctrl->status.busy = 0;
       i2c->ctrl->stalled = 0;
       i2c->ctrl->snum    = 0;
     
       /* Master mode only: Aborts transfer in progress */
       i2c->reg->IC_ENABLE_b.ABORT = 0x1;

       NVIC_ClearPendingIRQ (i2c->i2c_ev_irq);
       NVIC_EnableIRQ (i2c->i2c_ev_irq);
       break; 

     default:
       return ARM_DRIVER_ERROR_UNSUPPORTED;     
   }
   return ARM_DRIVER_OK;
}

static ARM_I2C_STATUS I2Cx_GetStatus(I2C_RESOURCES *i2c)
{
  return (i2c->ctrl->status);
}




/**
  \fn          void I2Cx_MasterHandler (I2C_RESOURCES *i2c)
  \brief       I2C Master state event handler.
  \param[in]   i2c  Pointer to I2C resources
  \return      I2C event notification flags
*/
static uint32_t I2Cx_MasterHandler (I2C_RESOURCES *i2c) 
{
  uint32_t status  = 0; 
  uint32_t event   = 0;
  volatile uint32_t clear = 0;
  uint32_t tx_abrt = 0;
	
  clear = clear;
  status = i2c->reg->IC_INTR_STAT;
  
  if(i2c->ctrl->stalled) 
  {
    /* Master resumes with repeated START here */
    i2c->ctrl->stalled = 0;
    i2c->reg->IC_CON_b.IC_RESTART_EN = 0x1;
  }

  if(status & RSI_I2C_EVENT_TRANSMIT_ABRT) 
  {
    i2c->ctrl->status.busy = 0;
    i2c->ctrl->status.mode = 0;
    tx_abrt = i2c->reg->IC_TX_ABRT_SOURCE;
    if(tx_abrt & TX_ABRT_7B_ADDR_NOACK)
    {
      event = ARM_I2C_EVENT_ADDRESS_NACK |
              ARM_I2C_EVENT_TRANSFER_DONE|
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->reg->IC_DATA_CMD_b.STOP = 0x1;     
    }
    if(tx_abrt & TX_ABRT_10ADDR1_NOACK)
    {
       event = ARM_I2C_EVENT_ADDRESS_NACK |
               ARM_I2C_EVENT_TRANSFER_DONE|
               ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
       i2c->reg->IC_DATA_CMD_b.STOP = 0x1;
    }
    if(tx_abrt & TX_ABRT_10ADDR2_NOACK)
    {
      event = ARM_I2C_EVENT_ADDRESS_NACK |
              ARM_I2C_EVENT_TRANSFER_DONE|
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->reg->IC_DATA_CMD_b.STOP = 0x1;
    }
    if(tx_abrt & TX_ABRT_TXDATA_NOACK)
    {
      event = ARM_I2C_EVENT_TRANSFER_DONE|
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->reg->IC_DATA_CMD_b.STOP = 0x1; 
    }
    if(tx_abrt & TX_ABRT_GCALL_NOACK)
    {
      event = ARM_I2C_EVENT_GENERAL_CALL;
      i2c->ctrl->status.general_call = 1;
    }
    if(tx_abrt & TX_ABRT_GCALL_READ)
    {
      event = ARM_I2C_EVENT_GENERAL_CALL;
      i2c->ctrl->status.general_call = 1;
    }
    if(tx_abrt & TX_ABRT_HS_ACKDET)
    {}
    if(tx_abrt & TX_ABRT_SBYTE_ACKDET)
    {}
    if(tx_abrt & TX_ABRT_HS_NORSTRT)
    {}
    if(tx_abrt & TX_ABRT_SBYTE_NORSTRT)
    {}
    if(tx_abrt & TX_ABRT_10B_RD_NORSTRT)
    {} 
    if(tx_abrt & TX_ABRT_MASTER_DIS)
    {}
    if(tx_abrt & TX_ARB_LOST)
    {
      event = ARM_I2C_EVENT_ARBITRATION_LOST|
              ARM_I2C_EVENT_TRANSFER_DONE   |
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->ctrl->status.arbitration_lost = 1;     
    }
    if(tx_abrt & TX_ABRT_SLV_ARBLOST)
    {
      event = ARM_I2C_EVENT_ARBITRATION_LOST|
              ARM_I2C_EVENT_TRANSFER_DONE   |
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->ctrl->status.arbitration_lost = 1;      
    }
    if(tx_abrt & TX_ABRT_USER_ABRT)
    {}
    if(tx_abrt & TX_ABRT_SDA_STUCK_AT_LOW)
    {
      i2c->reg->IC_ENABLE_b.SDA_STUCK_RECOVERY_ENABLE = 0x1;
      event = ARM_I2C_EVENT_BUS_CLEAR; 
    }
    clear = i2c->reg->IC_CLR_INTR;
		I2C_EnableDisableInterrupt(i2c, 0, I2C_EVENT_TRANSMIT_EMPTY);
	}
  if(status & RSI_I2C_EVENT_SCL_STUCK_AT_LOW)
  {
    clear = i2c->reg->IC_CLR_INTR;
    i2c->reg->IC_ENABLE_b.EN = 0;
  }
  if(status & RSI_I2C_EVENT_MST_ON_HOLD)
  {
    clear = i2c->reg->IC_CLR_INTR;
  }
  if(status & RSI_I2C_EVENT_START_DETECT)
  {
    clear = i2c->reg->IC_CLR_START_DET_b.CLR_START_DET;		
  }
  if(status & RSI_I2C_EVENT_STOP_DETECT)
  {
    clear = i2c->reg->IC_CLR_STOP_DET_b.CLR_STOP_DET; 
		i2c->ctrl->status.busy = 0;
		event = ARM_I2C_EVENT_TRANSFER_DONE;
		I2C_EnableDisableInterrupt(i2c, 0, I2C_EVENT_RECEIVE_FULL);
		
		if (i2c->ctrl->pending) 
		{
			/* Stall I2C transaction */
			NVIC_DisableIRQ (i2c->i2c_ev_irq);
			i2c->ctrl->stalled = I2C_MASTER;
			return (event);
		}
  }
  if(status & RSI_I2C_EVENT_ACTIVITY_ON_BUS)
  {
    event = ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
    clear = i2c->reg->IC_CLR_ACTIVITY_b.CLR_ACTIVITY;
  }
  if(status & RSI_I2C_EVENT_RECEIVE_FULL)
  {
    if(i2c->ctrl->num > 0)
    {    
      i2c->ctrl->data[ i2c->ctrl->cnt] = i2c->reg->IC_DATA_CMD_b.DAT;
		  i2c->ctrl->cnt++;				
		  i2c->ctrl->num--; 			
			if(i2c->ctrl->num == 1)
			{
				i2c->reg->IC_DATA_CMD = (1 << 8)|(1 << 9);
			}
			if(i2c->ctrl->num > 1)
		  {
				i2c->reg->IC_DATA_CMD = (1 << 8);
			}
			if(i2c->ctrl->num == 0)
			{
				i2c->ctrl->status.busy = 0;
				event = ARM_I2C_EVENT_TRANSFER_DONE;
			
				if (i2c->ctrl->pending) 
				{
					/* Stall I2C transaction */
					NVIC_DisableIRQ (i2c->i2c_ev_irq);
					i2c->ctrl->stalled = I2C_MASTER;
					return (event);
				}
			}
    }
	}
  if(status & RSI_I2C_EVENT_TRANSMIT_EMPTY)
  {
    if(i2c->ctrl->num > 0)
    {
      if(i2c->ctrl->num == 1)
      {
        if(i2c->ctrl->pending)
        {
				  i2c->reg->IC_DATA_CMD = i2c->ctrl->data[i2c->ctrl->cnt];
				}
        else
        {
          i2c->reg->IC_DATA_CMD = i2c->ctrl->data[i2c->ctrl->cnt]|(1 << 9); 
        }   
      }
      else
      {				
        i2c->reg->IC_DATA_CMD_b.DAT = i2c->ctrl->data[i2c->ctrl->cnt];
      }      
      i2c->ctrl->cnt++;
      i2c->ctrl->num--;             
    }
    else
    {
      i2c->ctrl->status.busy = 0;
      event = ARM_I2C_EVENT_TRANSFER_DONE;
      I2C_EnableDisableInterrupt(i2c, 0, I2C_EVENT_TRANSMIT_EMPTY);
      
      if (i2c->ctrl->pending) 
      {
        /* Stall I2C transaction */
        NVIC_DisableIRQ (i2c->i2c_ev_irq);
        i2c->ctrl->stalled = I2C_MASTER;
        return (event);
      }
    }
  }
  if(status & RSI_I2C_EVENT_RECEIVE_UNDER)
  {
    clear = i2c->reg->IC_CLR_RX_UNDER_b.CLR_RX_UNDER;
  }
  if(status & RSI_I2C_EVENT_RECEIVE_OVER)
  {
    clear = i2c->reg->IC_CLR_RX_OVER_b.CLR_RX_OVER;
  } 
  return (event);
}




/**
  \fn          void I2Cx_SlaveHandler (I2C_RESOURCES *i2c)
  \brief       I2C Slave state event handler.
  \param[in]   i2c  Pointer to I2C resources
  \return      I2C event notification flags
*/
static uint32_t I2Cx_SlaveHandler (I2C_RESOURCES *i2c) 
{
  uint32_t status  = 0; 
  uint32_t event   = 0;
  volatile uint32_t clear   = 0;
  uint32_t tx_abrt = 0;

  clear = clear;
  status = i2c->reg->IC_INTR_STAT;
  
  if (i2c->ctrl->stalled) 
  {
    /* Master resumes with repeated START here */
    i2c->ctrl->stalled = 0;
    i2c->reg->IC_CON_b.IC_RESTART_EN = 0x1;
  }

  if(status & RSI_I2C_EVENT_TRANSMIT_ABRT)
  {
    i2c->ctrl->status.busy = 0;
    i2c->ctrl->status.mode = 0;
    tx_abrt = i2c->reg->IC_TX_ABRT_SOURCE; 
    if(tx_abrt & TX_ARB_LOST)
    {
      event = ARM_I2C_EVENT_ARBITRATION_LOST|
              ARM_I2C_EVENT_TRANSFER_DONE   |
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->ctrl->status.arbitration_lost = 1;
    }
    if(tx_abrt & TX_ABRT_SLVFLUSH_TXFIFO)
    {}
    if(tx_abrt & TX_ABRT_SLV_ARBLOST)
    {
      event = ARM_I2C_EVENT_ARBITRATION_LOST|
              ARM_I2C_EVENT_TRANSFER_DONE   |
              ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      i2c->ctrl->status.arbitration_lost = 1;
    }
    if(tx_abrt & TX_ABRT_SLVRD_INTX)
    {}
    if(tx_abrt & TX_TX_FLUSH_CNT)
    {}
    /* Clear all interrupt and error flags */
    clear = i2c->reg->IC_CLR_INTR;         
  }
  if(status & RSI_I2C_EVENT_GENERAL_CALL)
  {
    /* General address recvd, ACK returned */
    i2c->ctrl->status.general_call = 1;
    clear = i2c->reg->IC_CLR_GEN_CALL_b.CLR_GEN_CALL;
		i2c->reg->IC_TAR_b.SPECIAL = 0x0;
    /* Stalled Slave receiver also resumes here */
    if (!i2c->ctrl->snum || !(i2c->ctrl->flags & I2C_FLAG_SLAVE_RX)) 
    {
      /* Receive buffer unavailable */
      if (i2c->ctrl->stalled) 
      {
        /* Already stalled, abort transaction to prevent dead-loops */
        event = ARM_I2C_EVENT_TRANSFER_DONE |ARM_I2C_EVENT_GENERAL_CALL|
                ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      }
      else
      {
        /* Stall I2C transaction */
        NVIC_DisableIRQ (i2c->i2c_ev_irq);
        i2c->ctrl->stalled = I2C_SLAVE_RX;
        return (ARM_I2C_EVENT_SLAVE_RECEIVE);        
      }      
    }
    i2c->ctrl->status.direction = 1;
    i2c->ctrl->status.busy      = 1;
    i2c->ctrl->cnt     = 0;
    i2c->ctrl->stalled = 0; 
    return (ARM_I2C_EVENT_GENERAL_CALL);		
   }
   if(status & RSI_I2C_EVENT_SCL_STUCK_AT_LOW)
   {
     clear = i2c->reg->IC_CLR_INTR;
     i2c->reg->IC_ENABLE_b.EN = 0;
     event = ARM_I2C_EVENT_BUS_CLEAR;      
   }
   if(status & RSI_I2C_EVENT_START_DETECT)
   {
     clear = i2c->reg->IC_CLR_START_DET_b.CLR_START_DET;
   }     
   if(status & RSI_I2C_EVENT_RESTART_DET)
   {
     clear = i2c->reg->IC_CLR_RESTART_DET_b.CLR_RESTART_DET; 
   }
   if(status & RSI_I2C_EVENT_STOP_DETECT)
   {
      clear = i2c->reg->IC_CLR_STOP_DET_b.CLR_STOP_DET;   
      /* STOP received while addressed */
      i2c->ctrl->status.busy = 0;
      /* Slave operation completed, generate events */
      event = ARM_I2C_EVENT_TRANSFER_DONE;
      if (i2c->ctrl->status.arbitration_lost) 
      {
        event |= ARM_I2C_EVENT_ARBITRATION_LOST;
      }
      if (i2c->ctrl->status.general_call) 
      {
        event |= ARM_I2C_EVENT_GENERAL_CALL;
      }
      if (i2c->ctrl->snum) 
      {
        event |= ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
      }
   }
   if(status & RSI_I2C_EVENT_ACTIVITY_ON_BUS)
   {
     event = ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
     clear = i2c->reg->IC_CLR_ACTIVITY_b.CLR_ACTIVITY;
   }
   if(status & RSI_I2C_EVENT_RECEIVE_FULL)
   {
     if(i2c->ctrl->snum > 0)
     {        
       i2c->ctrl->sdata[i2c->ctrl->cnt++] = i2c->reg->IC_DATA_CMD_b.DAT;
       i2c->ctrl->snum--;        
     }
     if(i2c->ctrl->snum == 0)
		 {
			 i2c->ctrl->status.busy = 0;
			 event = ARM_I2C_EVENT_TRANSFER_DONE;
			 //I2C_EnableDisableInterrupt(i2c, 0, RSI_I2C_EVENT_RECEIVE_FULL);
		 }
		 if(i2c->ctrl->pending) 
		 {
			 /* Stall I2C transaction */
			 NVIC_DisableIRQ (i2c->i2c_ev_irq);
			 i2c->ctrl->stalled = I2C_SLAVE;
			 return (event);
		 }		 
   }
   if(status & RSI_I2C_EVENT_TRANSMIT_EMPTY)
   {
     if(i2c->ctrl->snum > 0)
     {
       i2c->reg->IC_DATA_CMD = i2c->ctrl->sdata[i2c->ctrl->cnt];      
       i2c->ctrl->cnt++;
       i2c->ctrl->snum--;                
     } 
		 else
		 {
			 if(i2c->ctrl->snum == 0)
			 {
				 i2c->ctrl->status.busy = 0;
			   event = ARM_I2C_EVENT_TRANSFER_DONE;
			   I2C_EnableDisableInterrupt(i2c, 0, I2C_EVENT_TRANSMIT_EMPTY);
			 }
			 if (i2c->ctrl->pending) 
       {
         /* Stall I2C transaction */
         NVIC_DisableIRQ (i2c->i2c_ev_irq);
         i2c->ctrl->stalled = I2C_SLAVE;
         return (event);
       }
		 }
   }
   if(status & RSI_I2C_EVENT_RECEIVE_UNDER)
   {
     clear = i2c->reg->IC_CLR_RX_UNDER_b.CLR_RX_UNDER;     
   }
   if(status & RSI_I2C_EVENT_RECEIVE_OVER)
   {
     clear = i2c->reg->IC_CLR_RX_OVER_b.CLR_RX_OVER;
   }
   if(status & RSI_I2C_EVENT_TRANSMIT_OVER)
   {
     clear = i2c->reg->IC_CLR_TX_OVER_b.CLR_TX_OVER;
   }     
   if(status & RSI_I2C_EVENT_READ_REQ)
   {
     /* Stalled Slave transmitter also resumes here */
     if (!i2c->ctrl->snum || (i2c->ctrl->flags & I2C_FLAG_SLAVE_RX)) 
     {
       /* Transmit buffer unavailable */
       if (i2c->ctrl->stalled) 
       {
         /* Already stalled, abort transaction to prevent dead-loops */
         event = ARM_I2C_EVENT_TRANSFER_DONE |
                 ARM_I2C_EVENT_TRANSFER_INCOMPLETE;
       }
       else
       {
         NVIC_DisableIRQ (i2c->i2c_ev_irq);
         i2c->ctrl->stalled = I2C_SLAVE_TX;
         return (ARM_I2C_EVENT_SLAVE_TRANSMIT);
       }     
     }
     i2c->ctrl->status.direction = 0;
     i2c->ctrl->status.busy      = 1;
     i2c->ctrl->cnt     = 0;
     i2c->ctrl->stalled = 0;         
   }     
  return (event);
}

/**
  \fn          void I2Cx_IRQHandler (I2C_RESOURCES *i2c)
  \brief       I2C Event Interrupt handler.
  \param[in]   i2c  Pointer to I2C resources
*/
static void I2Cx_IRQHandler (I2C_RESOURCES *i2c) 
{
  uint32_t event;

  if(i2c->ctrl->status.mode) 
  {
    event = I2Cx_MasterHandler (i2c);
  }
  else 
  {
    event = I2Cx_SlaveHandler (i2c);
  }
  /* Callback event notification */
  if (event && i2c->ctrl->cb_event) 
  {
    i2c->ctrl->cb_event (event);
  }
}

// End I2C Interface
#if (RTE_I2C0)
/* I2C0 Driver wrapper functions */
static int32_t I2C0_Initialize (ARM_I2C_SignalEvent_t cb_event) 
{
  return (I2Cx_Initialize (cb_event, &I2C0_Resources));
}
static int32_t I2C0_Uninitialize (void) 
{
  return (I2Cx_Uninitialize (&I2C0_Resources));
}
static int32_t I2C0_PowerControl (ARM_POWER_STATE state) 
{
  return (I2Cx_PowerControl (state, &I2C0_Resources));
}
static int32_t I2C0_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C0_Resources));
}
static int32_t I2C0_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C0_Resources));
}
static int32_t I2C0_SlaveTransmit (const uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveTransmit (data, num, &I2C0_Resources));
}
static int32_t I2C0_SlaveReceive (uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveReceive (data, num, &I2C0_Resources));
}
static int32_t I2C0_GetDataCount (void) 
{
  return (I2Cx_GetDataCount (&I2C0_Resources));
}
static int32_t I2C0_Control (uint32_t control, uint32_t arg) 
{
  return (I2Cx_Control (control, arg, &I2C0_Resources));
}
static ARM_I2C_STATUS I2C0_GetStatus (void) 
{
  return (I2Cx_GetStatus (&I2C0_Resources));
}
void I2C0_IRQHandler (void) 
{
  I2Cx_IRQHandler (&I2C0_Resources);
}

/* I2C0 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C0 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C0_Initialize,
  I2C0_Uninitialize,
  I2C0_PowerControl,
  I2C0_MasterTransmit,
  I2C0_MasterReceive,
  I2C0_SlaveTransmit,
  I2C0_SlaveReceive,
  I2C0_GetDataCount,
  I2C0_Control,
  I2C0_GetStatus
};
#endif

#if (RTE_I2C1)
/* I2C1 Driver wrapper functions */
static int32_t I2C1_Initialize (ARM_I2C_SignalEvent_t cb_event) 
{
  return (I2Cx_Initialize (cb_event, &I2C1_Resources));
}
static int32_t I2C1_Uninitialize (void) 
{
  return (I2Cx_Uninitialize (&I2C1_Resources));
}
static int32_t I2C1_PowerControl (ARM_POWER_STATE state)
{
  return (I2Cx_PowerControl (state, &I2C1_Resources));
}
static int32_t I2C1_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C1_Resources));
}
static int32_t I2C1_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C1_Resources));
}
static int32_t I2C1_SlaveTransmit (const uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveTransmit (data, num, &I2C1_Resources));
}
static int32_t I2C1_SlaveReceive (uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveReceive (data, num, &I2C1_Resources));
}
static int32_t I2C1_GetDataCount (void) 
{
  return (I2Cx_GetDataCount (&I2C1_Resources));
}
static int32_t I2C1_Control (uint32_t control, uint32_t arg) 
{
  return (I2Cx_Control (control, arg, &I2C1_Resources));
}
static ARM_I2C_STATUS I2C1_GetStatus (void) 
{
  return (I2Cx_GetStatus (&I2C1_Resources));
}
void I2C1_IRQHandler (void) 
{
  I2Cx_IRQHandler (&I2C1_Resources);
}

/* I2C1 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C1 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C1_Initialize,
  I2C1_Uninitialize,
  I2C1_PowerControl,
  I2C1_MasterTransmit,
  I2C1_MasterReceive,
  I2C1_SlaveTransmit,
  I2C1_SlaveReceive,
  I2C1_GetDataCount,
  I2C1_Control,
  I2C1_GetStatus
};
#endif

#if (RTE_I2C2)
/* I2C2 Driver wrapper functions */
static int32_t I2C2_Initialize (ARM_I2C_SignalEvent_t cb_event) 
{
  return (I2Cx_Initialize (cb_event, &I2C2_Resources));
}
static int32_t I2C2_Uninitialize (void) 
{
  return (I2Cx_Uninitialize (&I2C2_Resources));
}
static int32_t I2C2_PowerControl (ARM_POWER_STATE state) 
{
  return (I2Cx_PowerControl (state, &I2C2_Resources));
}
static int32_t I2C2_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterTransmit (addr, data, num, xfer_pending, &I2C2_Resources));
}
static int32_t I2C2_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending) 
{
  return (I2Cx_MasterReceive (addr, data, num, xfer_pending, &I2C2_Resources));
}
static int32_t I2C2_SlaveTransmit (const uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveTransmit (data, num, &I2C2_Resources));
}
static int32_t I2C2_SlaveReceive (uint8_t *data, uint32_t num) 
{
  return (I2Cx_SlaveReceive (data, num, &I2C2_Resources));
}
static int32_t I2C2_GetDataCount (void) 
{
  return (I2Cx_GetDataCount (&I2C2_Resources));
}
static int32_t I2C2_Control (uint32_t control, uint32_t arg) 
{
  return (I2Cx_Control (control, arg, &I2C2_Resources));
}
static ARM_I2C_STATUS I2C2_GetStatus (void) 
{
  return (I2Cx_GetStatus (&I2C2_Resources));
}
void I2C2_IRQHandler (void) 
{
  I2Cx_IRQHandler (&I2C2_Resources);
}

/* I2C1 Driver Control Block */
ARM_DRIVER_I2C Driver_I2C2 = {
  I2C_GetVersion,
  I2C_GetCapabilities,
  I2C2_Initialize,
  I2C2_Uninitialize,
  I2C2_PowerControl,
  I2C2_MasterTransmit,
  I2C2_MasterReceive,
  I2C2_SlaveTransmit,
  I2C2_SlaveReceive,
  I2C2_GetDataCount,
  I2C2_Control,
  I2C2_GetStatus
};
#endif

#endif //RTE_I2C

