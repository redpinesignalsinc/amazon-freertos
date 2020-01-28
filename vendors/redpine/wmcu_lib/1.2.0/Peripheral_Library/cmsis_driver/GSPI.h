#include "rsi_chip.h"
#include "Driver_SPI.h"
#include "RTE_Device.h"
#include "RTE_Components.h"
#include "UDMA.h"     
#include "clock_update.h"


/* Current driver status flag definition */
#define SPI_INITIALIZED                   (1    << 0)       // SPI initialized
#define SPI_POWERED                       (1    << 1)       // SPI powered on
#define SPI_CONFIGURED                    (1    << 2)       // SPI configured
#define SPI_DATA_LOST                     (1    << 3)       // SPI data lost occurred
#define SPI_MODE_FAULT                    (1    << 4)       // SPI mode fault occurred

/**
 * Macro defines for GSPI MANUAL STATUS Register
 */

#define GSPI_MANUAL_STATUS_BITMASK   0x07AF   /*!< GSPI Manual Status reg bitmask*/
#define GSPI_BUSY_F                  0x01     /*!< A read,write or dummy cycle operation is in process in manual mode*/
#define GSPI_FIFO_FULL_WFIFO_S       0x02     /*!< Full status indication for Wfifo in manual mode*/
#define GSPI_FIFO_AFULL_WFIFO_S      0x04     /*!< AFull status indication for Wfifo in manual mode*/
#define GSPI_FIFO_EMPTY_WFIFO_S      0x08     /*!< Empty status indication for Wfifo in manual mode*/
#define GSPI_FIFO_EMPTY_RFIFO_S      0x80     /*!< Empty status indication for Rfifo in manual mode*/
#define GSPI_FIFO_AEMPTY_WFIFO_S     0x100    /*!< AEmpty status indication for Rfifo in manual mode*/
#define GSPI_MAN_CSN                 0x400    /*!< Status of chip select signal*/

/**
 * Macro defines for GSPI INTR MASK Register
 */
#define GSPI_INTR_MASK_BITMASK         0x7F 	/*!< GSPI Interrupt mask reg bitmask*/
#define GSPI_INTR_MASK_BIT             0x01 	/*!< Mask the GSPI intr*/
#define GSPI_FIFO_AEMPTY_RFIFO_MASK    0x02 	/*!< read fifo almost empty intr mask */
#define GSPI_FIFO_AFULL_RFIFO_MASK     0x04		/*!< read fifo almost full intr mask*/
#define GSPI_FIFO_AEMPTY_WFIFO_MASK    0x08	  /*!< write fifo almost empty intr mask*/
#define GSPI_FIFO_AFULL_WFIFO_MASK     0x10 	/*!< Write Fifo almost full intr mask*/
#define GSPI_FIFO_FULL_WFIFO_MASK      0x20   /*!< write fifo is full intr mask*/
#define GSPI_FIFO_EMPTY_RFIFO_MASK     0x40   /*!< read fifo is empty intr mask*/
/**
 * Macro defines for GSPI INTR UNMASK Register
 */
#define GSPI_INTR_UNMASK_BITMASK         0x7F   /*!< GSPI Interrupt Unmask reg bitmask*/
#define GSPI_INTR_UNMASK_BIT             0x01   /*!< Unmask the GSPI intr*/
#define GSPI_FIFO_AEMPTY_RFIFO_UNMASK    0x02   /*!< read fifo almost empty intr unmask */
#define GSPI_FIFO_AFULL_RFIFO_UNMASK     0x04   /*!< read fifo almost full intr unmask*/
#define GSPI_FIFO_AEMPTY_WFIFO_UNMASK    0x08   /*!< write fifo almost empty intr unmask*/
#define GSPI_FIFO_AFULL_WFIFO_UNMASK     0x10   /*!< Write Fifo almost full intr unmask*/
#define GSPI_FIFO_FULL_WFIFO_UNMASK      0x20   /*!< write fifo is full intr unmask*/
#define GSPI_FIFO_EMPTY_RFIFO_UNMASK     0x40   /*!< read fifo is empty intr unmask*/

// SPI DMA
typedef  struct _SPI_DMA
{
	RSI_UDMA_CHA_CFG_T         chnl_cfg;
	uint8_t                    channel;       // DMA Channel number
	UDMA_SignalEvent_t         cb_event;      // DMA Event callback
} GSPI_DMA;
/* SPI status */
typedef struct _SPI_STATUS {
	uint8_t busy;                         // Transmitter/Receiver busy flag
	uint8_t data_lost;                    // Data lost: Receive overflow / Transmit underflow (cleared on start of transfer operation)
	uint8_t mode_fault;                   // Mode fault detected; optional (cleared on start of transfer operation)
} GSPI_STATUS;

/* SPI Information (Run-time) */
typedef struct _SPI_INFO {
	ARM_SPI_SignalEvent_t cb_event;       // Event Callback
	GSPI_STATUS            status;        // Status flags
	uint8_t               state;          // Current SPI state
	uint32_t              mode;           // Current SPI mode
  uint8_t               receive_function;
} GSPI_INFO;

/* SPI Transfer Information (Run-Time) */
typedef struct _SPI_TRANSFER_INFO {
	uint32_t              num;             // Total number of transfers
	uint8_t               *rx_buf;         // Pointer to in data buffer
	uint8_t               *tx_buf;         // Pointer to out data buffer
	uint32_t              rx_cnt;          // Number of data received
	uint32_t              tx_cnt;          // Number of data sent
	uint32_t              dump_val;        // Variable for dumping DMA data
	uint16_t              def_val;         // Default transfer value
} GSPI_TRANSFER_INFO;

/* SPI Resources */
typedef struct {
	GSPI0_Type            *reg;                                   // SPI peripheral register interface
	uint32_t             (*periph_clock)(PERI_CLKS_T peri_src);   // Peripheral bus clock
	IRQn_Type            irq_num;                                 // SPI IRQ number
	GSPI_DMA              *rx_dma;                                // Receive stream register interface
	GSPI_DMA              *tx_dma;                                // Transmit stream register interface
	GSPI_INFO             *info;                                  // SPI Run-time information
	GSPI_TRANSFER_INFO    *xfer;                                  // SPI transfer information
  
} const GSPI_RESOURCES;
