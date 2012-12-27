#ifndef __spi_internal_h__
#define __spi_internal_h__

enum SPI_COMM_REG_BIT_POS
{
  SPI_COMM_BIT_ACT            =  0,
  /**<
   * \ingroup spi
   * The activate bit.
   */
  SPI_COMM_BIT_RESUME         =  1,
  /**<
   * \ingroup spi
   * The resume bit.
   */
  SPI_COMM_BIT_RESET          =  2,
  /**<
   * \ingroup spi
   * The reset bit.
   */
  SPI_COMM_BIT_PAUSE_EN       =  4,
  /**<
   * \ingroup spi
   * The pause enable bit.
   */
  SPI_COMM_BIT_CS_DEASSERT_EN =  5,
  /**<
   * \ingroup spi
   * The deassert enable bit.
   */
  SPI_COMM_BIT_CPHA           =  8,
  /**<
   * \ingroup spi
   * The clock format bit.
   */
  SPI_COMM_BIT_CPOL           =  9,
  /**<
   * \ingroup spi
   * The clock polarity bit.
   */
  SPI_COMM_BIT_RX_DMA_EN      = 10,
  /**<
   * \ingroup spi
   * The RX DMA enable/disable bit.
   */
  SPI_COMM_BIT_TX_DMA_EN      = 11,
  /**<
   * \ingroup spi
   * The TX DMA enable/disable bit.
   */
  SPI_COMM_BIT_TX_MSBF        = 12,
  /**<
   * \ingroup spi
   * The TX MSB/LSB select bit.
   */
  SPI_COMM_BIT_RX_MSBF        = 13,
  /**<
   * \ingroup spi
   * The RX MSB/LSB select bit.
   */
  SPI_COMM_BIT_RX_ENDIAN      = 14,
  /**<
   * \ingroup spi
   * The RX big/little endian select bit.
   */
  SPI_COMM_BIT_TX_ENDIAN      = 15,
  /**<
   * \ingroup spi
   * The TX big/little endian select bit.
   */
  SPI_COMM_BIT_FINISH_IE      = 16,
  /**<
   * \ingroup spi
   * The finish mode enable/disable bit.
   */
  SPI_COMM_BIT_PAUSE_IE       = 17
  /**<
   * \ingroup spi
   * The pause mode enable/disable bit.
   */
};
typedef enum SPI_COMM_REG_BIT_POS SPI_COMM_REG_BIT_POS;

/** \enum SPI_STATUS1_BIT
 * \ingroup spi
 *
 * @brief
 * Specify the bit position in the SPI status register 1.
 */
enum SPI_STATUS1_BIT
{
  SPI_STATUS1_BIT_FINISH = (1 << 0),
  /**<
   * \ingroup spi
   * The finish bit in the SPI status register 1.
   */
  SPI_STATUS1_BIT_PAUSE = (1 << 1)
  /**<
   * \ingroup spi
   * The pause bit in the SPI status register 1.
   */
};
typedef enum SPI_STATUS1_BIT SPI_STATUS1_BIT;

/** \enum SPI_STATUS2_BIT
 * \ingroup spi
 *
 * @brief
 * Specify the bit position in the SPI status register 2.
 */
enum SPI_STATUS2_BIT
{
  SPI_STATUS2_BIT_BUSY = (1 << 0)
  /**<
   * \ingroup spi
   * The busy bit in the SPI status register 2.
   */
};
typedef enum SPI_STATUS2_BIT SPI_STATUS2_BIT;

/** \enum SPI_STATUS_REG
 * \ingroup spi
 *
 * @brief Choose the supported status registers.
 */
enum SPI_STATUS_REG
{
  SPI_STATUS_REG_1,
  /**<
   * \ingroup spi
   * status register 1
   */
  SPI_STATUS_REG_2
  /**<
   * \ingroup spi
   * status register 2
   */
};
typedef enum SPI_STATUS_REG SPI_STATUS_REG;

typedef enum
{
  SPI_GMC_SLOW_DOWN_ENABLE = 0,
  SPI_GMC_SPLIT_BURST_ENABLE = 4
} SPI_GMC_SLOW_DOWN_REG_BIT_POS;

typedef enum
{
  SPI_ULTRA_HIGH_ENABLE = 0
} SPI_ULTRA_HIGH_PRIORITY_REG_BIT_POS;

#endif
