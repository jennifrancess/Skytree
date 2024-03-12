//
// Created by Beheerder on 3/11/2024.
//

#ifndef SOURCE_ADXL343_H
#define SOURCE_ADXL343_H

#include <stdbool.h>                            //!< Include to use standard boolean
#include <stdint.h>                             //!< Include to use integer types
#include "FunctionStatus.h"


/*======================================================================
    REGISTERS
-----------------------------------------------------------------------*/
#define ADXL343_REG_DEVID (0x00)        /**< Device ID */
#define ADXL343_REG_THRESH_TAP (0x1D)   /**< Tap threshold */
#define ADXL343_REG_DUR (0x21)          /**< Tap duration */
#define ADXL343_REG_LATENT (0x22)       /**< Tap latency */
#define ADXL343_REG_WINDOW (0x23)       /**< Tap window */
#define ADXL343_REG_THRESH_ACT (0x24)   /**< Activity threshold */
#define ADXL343_REG_THRESH_INACT (0x25) /**< Inactivity threshold */
#define ADXL343_REG_TIME_INACT (0x26)   /**< Inactivity time */
#define ADXL343_REG_ACT_INACT_CTL (0x27) /**< Axis enable control for activity and inactivity detection */
#define ADXL343_REG_TAP_AXES (0x2A)  /**< Axis control for single/double tap */
#define ADXL343_REG_ACT_TAP_STATUS (0x2B) /**< Source for single/double tap */
#define ADXL343_REG_BW_RATE (0x2C)     /**< Data rate and power mode control */
#define ADXL343_REG_POWER_CTL (0x2D)   /**< Power-saving features control */
#define ADXL343_REG_INT_ENABLE (0x2E)  /**< Interrupt enable control */
#define ADXL343_REG_INT_MAP (0x2F)     /**< Interrupt mapping control */
#define ADXL343_REG_INT_SOURCE (0x30)  /**< Source of interrupts */
#define ADXL343_REG_DATA_FORMAT (0x31) /**< Data format control */
#define ADXL343_REG_DATAX0 (0x32)      /**< X-axis data 0 */
#define ADXL343_REG_DATAX1 (0x33)      /**< X-axis data 1 */
#define ADXL343_REG_DATAY0 (0x34)      /**< Y-axis data 0 */
#define ADXL343_REG_DATAY1 (0x35)      /**< Y-axis data 1 */
#define ADXL343_REG_DATAZ0 (0x36)      /**< Z-axis data 0 */
#define ADXL343_REG_DATAZ1 (0x37)      /**< Z-axis data 1 */
#define ADXL343_REG_FIFO_CTL (0x38)    /**< FIFO control */
#define ADXL343_REG_FIFO_STATUS (0x39) /**< FIFO status */

/*======================================================================
    REGISTERS VALUES
-----------------------------------------------------------------------*/
#define ADXL343_THRESH_TAP (0x14)   /**< Tap threshold value */
#define ADXL343_DUR (0x32)          /**< Tap duration value */
#define ADXL343_THRESH_ACT (0x08)   /**< Activity threshold value */
#define ADXL343_THRESH_INACT (0x02) /**< Inactivity threshold */
#define ADXL343_TIME_INACT (0x05)   /**< Inactivity time */
#define ADXL343_ACT_INACT_CTL (0xFF) /**< Axis enable control for activity and inactivity detection */
#define ADXL343_TAP_AXES (0x07)  /**< Axis control for single/double tap */
#define ADXL343_POWER_CTL (0x28)   /**< Power-saving features control */
#define ADXL343_INT_ENABLE (0xD9)  /**< Interrupt enable control */
#define ADXL343_INT_MAP (0x2F)     /**< Interrupt mapping control */
#define ADXL343_DATA_FORMAT (0x00) /**< Data format control */
#define ADXL343_FIFO_CTL (0x99)    /**< FIFO control */

/*======================================================================
    REGISTERS MASKS
-----------------------------------------------------------------------*/
#define ADXL343_RANGEMASK (0x03)
#define ADXL343_RESOLUTIONMASK (0x08)

union interrupt_config {
    uint8_t value; /**< Composite 8-bit value of the bitfield.*/
    struct {
        uint8_t overrun : 1;    /**< Bit 0 */
        uint8_t watermark : 1;  /**< Bit 1 */
        uint8_t freefall : 1;   /**< Bit 2 */
        uint8_t inactivity : 1; /**< Bit 3 */
        uint8_t activity : 1;   /**< Bit 4 */
        uint8_t double_tap : 1; /**< Bit 5 */
        uint8_t single_tap : 1; /**< Bit 6 */
        uint8_t data_ready : 1; /**< Bit 7 */
    } bits;                   /**< Individual bits in the bitfield. */
};

typedef enum {
    ADXL343_RANGE_2_G = 0x00,  /**< +/- 2g (default value) */
    ADXL343_RANGE_4_G = 0x01,  /**< +/- 4g */
    ADXL343_RANGE_8_G = 0x02,  /**< +/- 8g */
    ADXL343_RANGE_16_G = 0x03, /**< +/- 16g */
} adxl343_range_t;

typedef enum {
    ADXL343_RESOLUTION_10BIT=0x00,
    ADXL343_RESOLUTION_FULL=0x08,
} adxl343_res_t;

extern volatile uint8_t data_ready; /**< set this flag in a  ISR routine of the interrupt pin connected to data_ready  */

FunctionStatus ADXL343_Initialize();
FunctionStatus ADXL343_enableInterrupts(union interrupt_config cfg);
FunctionStatus ADXL343_MapInterrupts(union interrupt_config cfg);
FunctionStatus ADXL343_disableInterrupts();


#endif //SOURCE_ADXL343_H
