#ifndef __SPI_DRV_HAL_H__
/* !ignore me! */
#define __SPI_DRV_HAL_H__

#include "spi_internal.h"

#define CMMB_SPI_INTERFACE_MAX_PKT_LENGTH_PER_TIMES (0x400)
#define CMMB_SPI_INTERFACE_MAX_PKT_COUNT_PER_TIMES  (0x100)

typedef int SPI_HANDLE;
typedef void (*SPI_CALLBACK)(void);
enum SPI_INT_TYPE {
  SPI_INT_PAUSE,
  /**<
   * \ingroup spi
   * Pause interrupt.
   */
  SPI_INT_FINISH
  /**<
   * \ingroup spi
   * Finish interrupt.
   */
};
typedef enum SPI_INT_TYPE SPI_INT_TYPE;
enum SPI_BIT_STATUS
{
  SPI_DISABLE,
  SPI_ENABLE
};
typedef enum SPI_BIT_STATUS SPI_BIT_STATUS;
/*********************************
  * SPI get tick delay mode enums.
  *********************************/
typedef enum
{
    /* Delay SCK for one clock. */
    SPI_GET_TICK_DELAY1 = 1,
    /* Delay SCK for two clocks. */
    SPI_GET_TICK_DELAY2 = 2
} SPI_GET_TICK_E;

/* SPI transimssion bit order definition. */
typedef enum
{
    /* Transfer LSB first. */
    SPI_MSBF_LSB = 0,
    /* Transfer MSB first. */
    SPI_MSBF_MSB = 1
} SPI_MSBF_E;

/* SPI clock polarity definition. */
typedef enum
{
    /* CPOL = 0. */
    SPI_CPOL_B0 = 0,
    /* CPOL = 1. */
    SPI_CPOL_B1 = 1
} SPI_CPOL_E;

/* SPI clock format definition. */
typedef enum
{
    /* CPHA = 0. */
    SPI_CPHA_B0 = 0,
    /* CPHA = 1. */
    SPI_CPHA_B1 = 1
} SPI_CPHA_E;

/* SPI data transfer byte order definition. */
typedef enum
{
    /* Use little endian. */
    SPI_ENDIAN_LITTLE = 0,
    /* Use big endian. */
    SPI_ENDIAN_BIG = 1
} SPI_ENDIAN_E;
typedef struct
{
    /* CS setup time. Unit in count of SPI base clock. Range(0-255).
       The chip select setup time = (cs_setup_time+1)*CLK_PERIOD, where CLK_PERIOD is the cycle time of the clock the SPI engine adopts. */
    int cs_setup_time;
    /* CS hold time. Unit in count of SPI base clock. Range(0-255).
       The chip select hold time = (cs_hold_time+1)*CLK_PERIOD. */
    int cs_hold_time;
    /* CS idle time. Unit in count of SPI base clock. Range(0-255).
       The chip select idle time between consecutive transaction = (cs_idle_time+1)*CLK_PERIOD. */
    int cs_idle_time;
    /* SCK clock low time. Unit in count of SPI base clock. Range(0-255).
       The SCK clock low time = (clk_low_time+1)*CLK_PERIOD. */
    int clk_low_time;
    /* SCK clock high time. Unit in count of SPI base clock. Range(0-255).
       The SCK clock high time = (clk_high_time+1)*CLK_PERIOD. */
    int clk_high_time;
    /* Bit order setting for SPI output. */
    SPI_MSBF_E tx_msbf;
    /* Bit order setting for SPI input. */
    SPI_MSBF_E rx_msbf;
    /* Byte order setting for SPI output. */
    SPI_ENDIAN_E tx_endian;
    /* Byte order setting for SPI input. */
    SPI_ENDIAN_E rx_endian;
    /* SPI clock polarity. */
    SPI_CPOL_E clk_polarity;
    /* SPI clock format. */
    SPI_CPHA_E clk_fmt;
} SPI_CONFIG_PARAM_T;

typedef enum
{
    /* Indicates no error occurs in the last operation. */
    SPI_RESULT_OK,
    
    /* Indicates some errors have occured in the last operation. */
    SPI_RESULT_ERROR,
    
    /* Indicates the function is not supported on the current platform. */
    SPI_RESULT_NOT_SUPPORTED
} SPI_RESULT;

typedef enum
{
    /* Deassert mode. SPI CS pin will be pulled low and high for each byte during transmission. */
    SPI_MODE_DEASSERT,
    /* Pause mode. SPI CS pin is pulled low and keeps until specific amount of transfers have been finished. */
    SPI_MODE_PAUSE,
    /* Ultra high mode. Raise DMA priority during DMA transmission. */
    SPI_MODE_ULTRA_HIGH,
    /* Slow down mode. Slow down SPI DMA speed during DMA transmission. */
    SPI_MODE_SLOW_DOWN,
    /* Get tick delay mode. This mode is used to tuning SPI timing. */
    SPI_MODE_GET_TICK
} SPI_HAL_MODE;

typedef struct
{
    /* [IN] Specify a SPI mode. */
    SPI_HAL_MODE mode;
    /* [IN/OUT] Specify whether the mode is enabled.
       For SPI_IOCTL_SET_MODE, it is an input parameter.
       For SPI_IOCTL_GET_MODE, it is an output parameter.
    */
    bool bEnable;
    /* [IN/OUT] The parameter for the specific mode.
       The meaning of this parameter depends on the mode to be set/get.
    */
    int Param;
} SPI_MODE_T;

typedef struct
{
    int id;
    int port;
    int owner;
    SPI_CONFIG_PARAM_T config;
} SPI_INTERNAL_HANDLE_T;

enum SPI_MODE {
  SPI_MODE_FIFO = 0,
  /**<
   * \ingroup spi
   * SPI FIFO mode.
   */
  SPI_MODE_DMA
  /**<
   * \ingroup spi
   * SPI DMA mode.
   */
};
typedef enum SPI_MODE SPI_MODE;
struct spi_attr_t {
	/*
  unsigned char setup_time;
  unsigned char hold_time;
  unsigned char clk_low;
  unsigned char clk_high;
  unsigned char idle_time;
  bool enable_pause_int;
  bool enable_finish_int;
  bool enable_pause_mode;
  bool enable_deassert_mode;
  SPI_MLSB tx_mlsb;
  SPI_MLSB rx_mlsb;
  SPI_MODE tx_mode;
  SPI_MODE rx_mode;
  SPI_CPOL clk_polarity;
  SPI_CPHA clk_fmt;
  SPI_GET_TICK_MODE get_tick;
  bool enable_ultra_high;
  int ultra_high_thresh;
  bool enable_gmc_slow_down;
  bool enable_gmc_split_burst;
  int gmc_slow_down_thresh;
  */
  int tt;
};
typedef struct spi_attr_t spi_attr_t;

#define CLEAR_SPI_COMM_REG(bit_pos)             \
  do {                                          \
    *SPI_COMM_REG &= (~(1 << bit_pos));          \
  } while (0)

#define SET_SPI_COMM_REG(bit_pos, status)       \
  do {                                          \
    *SPI_COMM_REG |= (status << bit_pos);        \
  } while(0)

  bool mtk_spi_open();
/********************************************************
  * Function
  * spi_close
  * Description
  * Release a SPI handle.
  * Parameters
  * handle :  [IN] SPI handle. Retrieved from spi_open().
  * Returns
  * Return a SPI result code.
  * Example
  * <code>
  * SPI_HANDLE handle;
  * handle = spi_open(0);
  * // SPI operations.
  * spi_close(handle);
  * </code>                                              
  ********************************************************/
bool mtk_spi_close();

bool mtk_spi_config(spi_attr_t * const attr);

/***********************************************************************
  * Function
  * spi_configure
  * Description
  * Configure SPI parameters including timing parameters and waveform parameters.
  * SPI parameters must be set before any SPI transmissions.
  * Parameters
  * handle :        [IN] SPI handle. Retrieved from spi_open().
  * pConfigParam :  [IN] SPI parameters. Refer to SPI_CONFIG_PARAM_T for details.
  * Returns
  * Return a SPI result code.
  * Example
  * <code>
  * SPI_HANDLE handle;
  * SPI_CONFIG_PARAM_T param;
  * handle = spi_open(0);
  * param.cs_setup_time = 5;
  * param.cs_hold_time = 5;
  * param.cs_idle_time = 5;
  * param.clk_low_time = 5;
  * param.clk_high_time = 5;
  * param.tx_mlsb = SPI_LSB;
  * param.rx_mlsb = SPI_LSB;
  * param.clk_polarity = SPI_CPOL_0;
  * param.clk_fmt = SPI_CPHA_0;
  * spi_configure(handle, &amp;param);
  * // ...
  * spi_close(handle);
  * </code>                                                             
  ***********************************************************************/
bool mtk_spi_configure(SPI_HANDLE handle, SPI_CONFIG_PARAM_T* pConfigParam);

/*******************************************************************************
  * Function
  * spi_write
  * Description
  * Fire a SPI transmission for output.
  * Parameters
  * handle :   [IN] SPI handle. Retrieved from spi_open().
  * pBuffer :  [IN] Data buffer for output.
  * length :   [IN] Data length for one package. Unit in bytes. <color Red><b>If count &gt; 1, length must be 4X.</b></color>
  * count :    [IN] Package count.<b> <color Red>If count &gt; 1, length must be 4X.</b></color>
  * fCB :      [IN] Specifies the callback function when the transmission completes.
  * If fCB is specified, SPI driver uses interrupt mode. This API is asynchronous.
  * If fCB is NULL, SPI drvier uses polling mode. This API becomes synchronous.
  * Returns
  * Return a SPI result code.
  * Example
  * <code>
  * SPI_HANDLE handle;
  * SPI_CONFIG_PARAM_T param;
  * handle = spi_open(0);
  * // ...
  * spi_configure(handle, &amp;param);
  * 
  * // Synchronous call.
  * spi_write(handle, spi_test_buffer1, 1024, 1, NULL);
  * 
  * // Asynchronous call.
  * spi_write(handle, spi_test_buffer1, 1024, 1, spi_test_cb);
  * // Wait for a event which is set in callback function.
  * // ...
  * spi_close(handle);
  * </code>                                                                     
  *******************************************************************************/
bool mtk_spi_write(SPI_INTERNAL_HANDLE_T* handle, void* pBuffer, int length, int count, SPI_CALLBACK fCB);

/*******************************************************************************
  * Function
  * spi_read
  * Description
  * Fire a SPI transmission for input.
  * Parameters
  * handle :   [IN] SPI handle. Retrieved from spi_open().
  * pBuffer :  [OUT] Data buffer for input.
  * length :   [IN] Data length for one package. Unit in bytes. <color Red><b>If count &gt; 1, length must be 4X.</b></color>
  * count :    [IN] Package count. <color Red><b>If count &gt; 1, length must be 4X.</b></color>
  * fCB :      [IN] Specifies the callback function when the transmission completes.
  * If fCB is specified, SPI driver uses interrupt mode. This API is asynchronous.
  * If fCB is NULL, SPI drvier uses polling mode. This API becomes synchronous.
  * Returns
  * Return a SPI result code.
  * Example
  * <code>
  * SPI_HANDLE handle;
  * SPI_CONFIG_PARAM_T param;
  * handle = spi_open(0);
  * // ...
  * spi_configure(handle, &amp;param);
  * 
  * // Synchronous call.
  * spi_read(handle, spi_test_buffer2, 1024, 1, NULL);
  * 
  * // Asynchronous call.
  * spi_read(handle, spi_test_buffer2, 1024, 1, spi_test_cb);
  * // Wait for a event which is set in callback function.
  * // ...
  * spi_close(handle);
  * </code>                                                                     
  *******************************************************************************/
bool mtk_spi_read(SPI_INTERNAL_HANDLE_T* handle, void* pBuffer, int length, int count, SPI_CALLBACK fCB);

/***********************************************************************************
  * Function
  * spi_readwrite
  * Description
  * Fire a SPI transmission for input and output. This transaction uses
  * duplex mode of SPI.
  * Parameters
  * handle :      [IN] SPI handle. Retrieved from spi_open().
  * pOutBuffer :  [IN] Data buffer for output.
  * pInBuffer :   [OUT] Data buffer for input.
  * length :      [IN] Data length for one package. Unit in bytes. <color Red><b>If count &gt; 1, length must be 4X.</b></color>
  * count :       [IN] Package count. <color Red><b>If count &gt; 1, length must be 4X.</b></color>
  * fCB :         [IN] Specifies the callback function when the transmission completes.
  * If fCB is specified, SPI driver uses interrupt mode. This API is asynchronous.
  * If fCB is NULL, SPI drvier uses polling mode. This API becomes synchronous.
  * Returns
  * Return a SPI result code.
  * Example
  * <code>
  * SPI_HANDLE handle;
  * SPI_CONFIG_PARAM_T param;
  * handle = spi_open(0);
  * // ...
  * spi_configure(handle, &amp;param);
  * 
  * // Synchronous call.
  * spi_readwrite(handle, spi_test_buffer1, spi_test_buffer2, 1024, 1, NULL);
  * 
  * // Asynchronous call.
  * spi_readwrite(handle, spi_test_buffer1, spi_test_buffer2, 1024, 1, spi_test_cb);
  * // Wait for a event which is set in callback function.
  * // ...
  * spi_close(handle);
  * </code>
  ***********************************************************************************/
bool mtk_spi_readwrite(SPI_INTERNAL_HANDLE_T* handle, void* pOutBuffer, void* pInBuffer, int length, int count, SPI_CALLBACK fCB);
//bool mtk_spi_readwrite_fifo( SPI_INTERNAL_HANDLE_T* handle,void* txdata, void* rxdata, int length, int count, SPI_CALLBACK fCB);
bool mtk_spi_mode_setting(SPI_MODE_T pSetMode);
bool mtk_spi_test();
#endif            //__SPI_DRV_HAL_H__
