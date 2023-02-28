//*****************************************************************************
// Library functions for color TFT graphic LCDs with the
// Ilitek ILI9341 controller
// (C) 2010-2013 Pavel Haiduc, HP InfoTech s.r.l., All rights reserved
//
// Compiler: CodeVisionAVR V3.08+
// Version: 1.00
//*****************************************************************************

//*****************************************************************************
// Revision history:
//
// V1.00
// 11102013_1 - Initial release.
//*****************************************************************************

#ifndef _GLCD_ILI9341_INCLUDED_
#define _GLCD_ILI9341_INCLUDED_

#include <glcd_types.h>

// Use fast horizontal & vertical line drawing functions.
#define _GLCD_FAST_LINEH_
#define _GLCD_FAST_LINEV_

// ILI9341 commands
#define ILI9341_CMD_NOP 0x00 // No operation
#define ILI9341_CMD_SOFT_RESET 0x01 // Software reset
#define ILI9341_CMD_GET_DISPLAY_ID 0x04 // Get display identification information:
                                        // LCD module manufacturer ID, module/driver version ID,
                                        // module/driver ID
#define ILI9341_CMD_GET_DISPLAY_STATUS 0x09 // Get display status
#define ILI9341_CMD_GET_PWR_MODE 0x0A // Get the current power mode
#define ILI9341_CMD_GET_MADCTL 0x0B // Get the Memory Address Control status bits: MX, MY, MH, MV, ML
#define ILI9341_CMD_GET_PIXEL_FORMAT 0x0C // Get the display pixel format
#define ILI9341_CMD_GET_DISPLAY_MODE 0x0D // Returns the display image mode status
#define ILI9341_CMD_GET_SIGNAL_MODE 0x0E // Get the current display signal mode from the peripheral
#define ILI9341_CMD_GET_SELF_DIAGNOSTIC 0x0F // Returns the display self-diagnostic result
#define ILI9341_CMD_ENT_SLEEP 0x10 // Enter sleep mode
#define ILI9341_CMD_EXIT_SLEEP 0x11 // Exit from sleep mode
#define ILI9341_CMD_ENT_PARTIAL_MODE 0x12 // Enter partial display mode
#define ILI9341_CMD_ENT_NORMAL_MODE 0x13 // Enter normal display mode
#define ILI9341_CMD_EXIT_INVERT_MODE 0x20 // Exit from inverted display mode
#define ILI9341_CMD_ENT_INVERT_MODE 0x21 // Enter inverted display mode
#define ILI9341_CMD_SET_GAMMA 0x26 // Selects the gamma curve used by the display device
#define ILI9341_CMD_BLANK_DISPLAY 0x28 // Set display off, without clearing the frame buffer
#define ILI9341_CMD_ON_DISPLAY 0x29 // Set display on
#define ILI9341_CMD_SET_COLUMN_ADDR 0x2A // Set the column extent of the frame buffer accessed with the
                                         // ILI9341_CMD_RD_MEM_CONT and ILI9341_CMD_WR_MEM_CONT commands
#define ILI9341_CMD_SET_PAGE_ADDR 0x2B // Set the page extent of the frame buffer accessed with the
                                       // ILI9341_CMD_RD_MEM_CONT and ILI9341_CMD_WR_MEM_CONT commands
#define ILI9341_CMD_WR_MEM_START 0x2C // Transfer image information from uC to ILI9341 starting with the
                                      // location specified by ILI9341_CMD_SET_COLUMN_ADDR and
                                      // ILI9341_CMD_SET_PAGE_ADDR
#define ILI9341_CMD_COLOR_SET 0x2D // Used to define the look-up table for 16-bit to 18-bit color depth conversion
#define ILI9341_CMD_RD_MEM_START 0x2E // Transfer image information from ILI9341 to uC starting with the
                                      // location specified by ILI9341_CMD_SET_COLUMN_ADDR and
                                      // ILI9341_CMD_SET_PAGE_ADDR
#define ILI9341_CMD_SET_PARTIAL_AREA 0x30 // Defines the partial display mode area
#define ILI9341_CMD_SET_SCROLL_AREA 0x33 // Defines the vertical scrolling and fixed areas
#define ILI9341_CMD_SET_TEAR_OFF 0x34 // Disable sending synchronization information from the display
#define ILI9341_CMD_SET_TEAR_ON 0x35 // Enable sending synchronization information from the display
#define ILI9341_CMD_SET_ADDR_MODE 0x36 // Set read order from uC to frame buffer and
                                       // from frame buffer to the display panel
#define ILI9341_CMD_SET_SCROLL_START 0x37 // Set the start of the vertical scrolling area
                                          // in the frame buffer
#define ILI9341_CMD_EXIT_IDLE_MODE 0x38 // Exit idle mode
#define ILI9341_CMD_ENT_IDLE_MODE 0x39 // Enter idle mode
#define ILI9341_CMD_SET_PIXEL_FORMAT 0x3A // Set pixel format
#define ILI9341_CMD_WR_MEM_CONT 0x3C // Transfer image data from uC to ILI9341 continuing from the
                                     // last ILI9341_CMD_WR_MEM_START or ILI9341_CMD_WR_MEM_CONT
#define ILI9341_CMD_RD_MEM_CONT 0x3E // Transfer image data from ILI9341 to uC continuing from the
                                     // last ILI9341_CMD_RD_MEM_START or ILI9341_CMD_RD_MEM_CONT
#define ILI9341_CMD_SET_TEAR_SCANLINE 0x44 // Enable sending the TE signal to the uC when the
                                           // display refresh reaches the provided scanline
#define ILI9341_CMD_GET_TEAR_SCANLINE 0x45 // Get the current scanline
#define ILI9341_CMD_SET_BRIGHTNESS 0x51 // Set display brightness
#define ILI9341_CMD_GET_BRIGHTNESS 0x52 // Get the display brightness
#define ILI9341_CMD_SET_CTRL_DISPLAY 0x53 // Set control the brightness control block,
                                          // display dimming and backlight
#define ILI9341_CMD_GET_CTRL_DISPLAY 0x54 // Get the brightness control block,
                                          // display dimming and backlight status
#define ILI9341_CMD_SET_CONTENT_ADAPTIVE_BRIGHTNESS 0x55 // Set the parameters for content
                                                         // adaptive brightness
#define ILI9341_CMD_GET_CONTENT_ADAPTIVE_BRIGHTNESS 0x56 // Get the parameters for content
                                                         // adaptive brightness
#define ILI9341_CMD_SET_CABC_MIN_BRIGHTNESS 0x5E // Set the minimum display brightness for
                                                 // for the content adaptive brightness function
#define ILI9341_CMD_GET_CABC_MIN_BRIGHTNESS 0x5F // Get the minimum display brightness for
                                                 // for the content adaptive brightness function
#define ILI9341_CMD_SET_RGB_INTF 0xB0 // Set the operation status of the display interface
#define ILI9341_CMD_SET_FRAME_RATE_NORMAL 0xB1 // Set frame rate control parameters in normal mode/full colors
#define ILI9341_CMD_SET_FRAME_RATE_IDLE 0xB2 // Set frame rate control parameters in idle mode/8 colors
#define ILI9341_CMD_SET_FRAME_RATE_PARTIAL 0xB3 // Set frame rate control parameters in partial mode/full colors
#define ILI9341_CMD_SET_DISPLAY_INVERSION 0xB4 // Set display inversion mode parameters
#define ILI9341_CMD_SET_BLANKING_PORCH 0xB5 // Set blanking porch parameters
#define ILI9341_CMD_SET_DISPLAY_FUNTION_CTRL 0xB6 // Set display scan parameters, shift direction, LCD type
#define ILI9341_CMD_SET_ENTRY_MODE 0xB7 // Set standby mode, low voltage detection control,
                                        // output level of gate driver parameters.
#define ILI9341_CMD_SET_BACKLIGHT_CTRL1 0xB8  // Set the grayscale data in user interface image mode
#define ILI9341_CMD_SET_BACKLIGHT_CTRL2 0xB9  // Set the grayscale data in still picture image mode
#define ILI9341_CMD_SET_BACKLIGHT_CTRL3 0xBA  // Set the grayscale data in user icon image mode
#define ILI9341_CMD_SET_BACKLIGHT_CTRL4 0xBB  // Set the minimum limit of grayscale threshold value
#define ILI9341_CMD_SET_BACKLIGHT_CTRL5 0xBC  // Set the transition time of brightness level
#define ILI9341_CMD_SET_BACKLIGHT_CTRL7 0xBE  // Set the frequency of the PWM_OUT output
#define ILI9341_CMD_SET_BACKLIGHT_CTRL8 0xBF  // Set the polarity of the LEDPWM signal and
                                              // controls the LEDON pin
#define ILI9341_CMD_SET_POWER_CTRL1 0xC0 // Set GVDD level
#define ILI9341_CMD_SET_POWER_CTRL2 0xC1 // Set the step-up circuits factor
#define ILI9341_CMD_SET_VCOM_CTRL1 0xC5 // Set the VCOML and VCOMH voltages
#define ILI9341_CMD_SET_VCOM_CTRL2 0xC7 // Set the VCOM offset voltage
#define ILI9341_CMD_SET_POWER_CTRLA 0xCB // ?
#define ILI9341_CMD_SET_POWER_CTRLB 0xCF // ?
#define ILI9341_CMD_NVM_WRITE 0xD0 // Write data to non-volatile memory
#define ILI9341_CMD_NVM_PROTECTION 0xD1 // Enable writing to non-volatile memory
#define ILI9341_CMD_GET_NVM_STATUS 0xD2 // Read the non-volatile memory status
#define ILI9341_CMD_GET_ID1 0xDA // Get the LCD manufacturer ID
#define ILI9341_CMD_GET_ID2 0xDB // Get the LCD module/driver version ID
#define ILI9341_CMD_GET_ID3 0xDC // Get the LCD module/driver ID
#define ILI9341_CMD_GET_ID4 0xD3 // Get the IC device code
#define ILI9341_CMD_SET_POSITIVE_GAMMA_CORRECTION 0xE0 // Set positive gamma correction control parameters
#define ILI9341_CMD_SET_NEGATIVE_GAMMA_CORRECTION 0xE1 // Set negative gamma correction control parameters
#define ILI9341_CMD_SET_DIGITAL_GAMMA_CTRL1 0xE2 // Set macro adjustements for red and blue gamma curves
#define ILI9341_CMD_SET_DIGITAL_GAMMA_CTRL2 0xE3 // Set micro adjustements for red and blue gamma curves
#define ILI9341_CMD_DRIVER_TIMING_CTRLA 0xE8 // ?
#define ILI9341_CMD_DRIVER_TIMING_CTRLB 0xEA // ?
#define ILI9341_CMD_SET_POWER_ON_SEQ_CTRL 0xED // Set power-on sequence: soft start, enable VCL, DDVDH, VG, VL,
                                               // DDVDH enhanced mode
#define ILI9341_CMD_ENABLE_3G 0xF2 // Enable 3 gamma control
#define ILI9341_CMD_SET_INTERFACE_CTRL 0xF6 // Set interface parameters: data transfer method,
                                            // memory write control, LSB/MSB first, display operation mode,
                                            // GDRAM interface, RGB interface, 65k color mode format
#define ILI9341_CMD_SET_PUMP_RATIO_CTRL 0xF7 // Set pump ratio control: DDVHD=2xVCI or DDVHD=3xVCI

// Structure that contains controller specific graphic LCD initialization data
typedef struct
        {
        flash unsigned char *font; // default font after initialization
        // pointer to the function used for reading a byte from external memory
        unsigned char (*readxmem) (GLCDMEMADDR_t addr);
        // pointer to the function used for writing a byte to external memory
        void (*writexmem) (GLCDMEMADDR_t addr, unsigned char data);
        unsigned char reverse_x:1; // reverse display horizontally
        unsigned char reverse_y:1; // reverse display vertically
        unsigned char cl_bits_order:1; // selects the color bits writing order to the display RAM
                                       // =0 -> RGB, =1 ->BGR
        unsigned char pump_ratio:1;    // =0 -> DDVDH=2xVCI, =1 -> DDVDH=3xVCI
        unsigned char ddvdh_enh_mode:1; // DDVDH enhanced mode =0 -> disable, =1 -> enable
        unsigned char cr_timing:1;
        unsigned char eq_timing:1;
        unsigned char precharge_timing:2;
        unsigned char vrh:6; // set VRH=3.0+(vrh-3)*0.05 [V]
        unsigned char vcoml:7; // set VCOML=-2.5+vcoml*0.025 [V]
        unsigned char vcomh:7; // set VCOMH=2.7+vcomh*0.025 [V]
        unsigned char vcom_offset:7; // set VCOM offset=VM+vcom_offset-64
        unsigned char lcd_type:1; // LCD type =0 normally black, =1 normally white
        unsigned char scan_mode:1; // set vertical scan mode =0 interlaced, =1 non-interlaced
        unsigned char frame_rate:5; // set vertical frame rate 61..119Hz
        } GLCDINIT_t;

// Initialization values for lshift
#define ILI9341_LSHIFT_LATCH_FALLING_EDGE 0 // data latch on falling edge
#define ILI9341_LSHIFT_LATCH_RISING_EDGE 1  // data latch on rising edge

// Initialization values for reverse_x
#define ILI9341_REVX_NORM 0 // No horizontal reverse
#define ILI9341_REVX_REV 1 // Horizontal reverse

// Initialization values for reverse_y
#define ILI9341_REVY_NORM 0 // No vertical reverse
#define ILI9341_REVY_REV 1 // Vertical reverse

// Initialization values for cl_bits_order
#define ILI9341_CL_BITS_RGB 0 // RGB
#define ILI9341_CL_BITS_BGR 1 // BGR

// Initialization values for pump_ratio
#define ILI9341_PUMP_RATIO_2X 0 // DDVDH=2xVCI
#define ILI9341_PUMP_RATIO_3X 1 // DDVDH=3xVCI

// Initialization values for ddvdh_enh_mode
#define ILI9341_DDVDH_ENH_MODE_OFF 0
#define ILI9341_DDVDH_ENH_MODE_ON 1

// Initialization values for cr_timing
#define ILI9341_CR_TIMING_DEF0U 1 // CR timing = default
#define ILI9341_CR_TIMING_DEF1U 0 // CR timing = default-1 units

// Initialization values for eq_timing
#define ILI9341_EQ_TIMING_DEF0U 1 // EQ timing = default
#define ILI9341_EQ_TIMING_DEF1U 0 // EQ timing = default-1 units

// Initialization values for precharge_timing
#define ILI9341_PRECHARGE_TIMING_DEF0U 2 // Precharge timing = default
#define ILI9341_PRECHARGE_TIMING_DEF1U 1 // Precharge timing = default-1 units
#define ILI9341_PRECHARGE_TIMING_DEF2U 0 // Precharge timing = default-2 units

// Initialization values for scan_mode
#define ILI9341_SCAN_INTERLACED 0
#define ILI9341_SCAN_NON_INTERLACED 1

// Initialization values for lcd_type
#define ILI9341_LCD_TYPE_BLACK 0
#define ILI9341_LCD_TYPE_WHITE 1

// Initialization values for frame_rate
#define ILI9341_FRAME_RATE_61 0x1F // 61Hz
#define ILI9341_FRAME_RATE_63 0x1E // 63Hz
#define ILI9341_FRAME_RATE_65 0x1D // 65Hz
#define ILI9341_FRAME_RATE_68 0x1C // 68Hz
#define ILI9341_FRAME_RATE_70 0x1B // 70Hz
#define ILI9341_FRAME_RATE_73 0x1A // 73Hz
#define ILI9341_FRAME_RATE_76 0x19 // 76Hz
#define ILI9341_FRAME_RATE_79 0x18 // 79Hz
#define ILI9341_FRAME_RATE_83 0x17 // 83Hz
#define ILI9341_FRAME_RATE_86 0x16 // 86Hz
#define ILI9341_FRAME_RATE_90 0x15 // 90Hz
#define ILI9341_FRAME_RATE_95 0x14 // 95Hz
#define ILI9341_FRAME_RATE_100 0x13 // 100Hz
#define ILI9341_FRAME_RATE_106 0x12 // 106Hz
#define ILI9341_FRAME_RATE_112 0x11 // 112Hz
#define ILI9341_FRAME_RATE_119 0x10 // 119Hz

// Default initialization values for GLCDINIT_t
// Default value for reverse_x
#define ILI9341_DEFAULT_REVX ILI9341_REVX_NORM // No horizontal reverse
// Default value for reverse_y
#define ILI9341_DEFAULT_REVY ILI9341_REVY_NORM // No vertical reverse
// Default value for cl_bits_order
#define ILI9341_DEFAULT_CL_BITS ILI9341_CL_BITS_RGB // write in RGB order
// Default value for pump_ratio
#define ILI9341_DEFAULT_PUMP_RATIO ILI9341_PUMP_RATIO_2X
// Default value for ddvhd_enh_mode
#define ILI9341_DEFAULT_DDVDH_ENH_MODE ILI9341_DDVDH_ENH_MODE_ON
// Default value for cr_timing
#define ILI9341_DEFAULT_CR_TIMING ILI9341_CR_TIMING_DEF0U
// Default value for eq_timing
#define ILI9341_DEFAULT_EQ_TIMING ILI9341_EQ_TIMING_DEF1U
// Default value for precharge_timing
#define ILI9341_DEFAULT_PRECHARGE_TIMING ILI9341_PRECHARGE_TIMING_DEF1U
// Default value for vrh
#define ILI9341_DEFAULT_VRH 0x26
// Default value for vcoml
#define ILI9341_DEFAULT_VCOML 0x3E
// Default value for vcomh
#define ILI9341_DEFAULT_VCOMH 0x35
// Default value for vcom_offset
#define ILI9341_DEFAULT_VCOM_OFFSET 0x3E
// Default value for lcd_type
#define ILI9341_DEFAULT_LCD_TYPE ILI9341_LCD_TYPE_WHITE
// Default value for scan_mode
#define ILI9341_DEFAULT_SCAN_MODE ILI9341_SCAN_INTERLACED
// Default value for frame_rate
#define ILI9341_DEFAULT_FRAME_RATE ILI9341_FRAME_RATE_70 // 70Hz

// Usual color definitions
#if _GLCD_COLOR_BITS_==8
// Color definitions for 256 color mode
// Bits 0..2 -> Blue 0..2
// Bits 3..5 -> Green 0..2
// Bits 6..7 -> Red 0..1
#define GLCD_CL_BLACK 0x00
#define GLCD_CL_WHITE 0xFF
#define GLCD_CL_GRAY 0x5B
#define GLCD_CL_LIGHT_GRAY 0xF6
#define GLCD_CL_GREEN 0x38
#define GLCD_CL_LIME 0xB8
#define GLCD_CL_BLUE 0x07
#define GLCD_CL_RED 0xC0
#define GLCD_CL_AQUA 0x6F
#define GLCD_CL_YELLOW 0xF8
#define GLCD_CL_MAGENTA 0xC7
#define GLCD_CL_CYAN 0x3F
#define GLCD_CL_DARK_CYAN 0x1B
#define GLCD_CL_ORANGE 0xE0
#define GLCD_CL_PINK 0xCF
#define GLCD_CL_BROWN 0x90
#define GLCD_CL_VIOLET 0x8E
#define GLCD_CL_SILVER 0xAC
#define GLCD_CL_GOLD 0xAA
#define GLCD_CL_NAVY 0x03
#define GLCD_CL_MAROON 0x40
#define GLCD_CL_PURPLE 0x43
#define GLCD_CL_OLIVE 0x58

#elif _GLCD_COLOR_BITS_==16
// Color definitions for 64k color mode
// Bits 0..4 -> Blue 0..4
// Bits 5..10 -> Green 0..5
// Bits 11..15 -> Red 0..4
#define GLCD_CL_BLACK 0x0000
#define GLCD_CL_WHITE 0xFFFF
#define GLCD_CL_GRAY 0x7BEF
#define GLCD_CL_LIGHT_GRAY 0xC618
#define GLCD_CL_GREEN 0x07E0
#define GLCD_CL_LIME 0x87E0
#define GLCD_CL_BLUE 0x001F
#define GLCD_CL_RED 0xF800
#define GLCD_CL_AQUA 0x5D1C
#define GLCD_CL_YELLOW 0xFFE0
#define GLCD_CL_MAGENTA 0xF81F
#define GLCD_CL_CYAN 0x07FF
#define GLCD_CL_DARK_CYAN 0x03EF
#define GLCD_CL_ORANGE 0xFCA0
#define GLCD_CL_PINK 0xF97F
#define GLCD_CL_BROWN 0x8200
#define GLCD_CL_VIOLET 0x9199
#define GLCD_CL_SILVER 0xA510
#define GLCD_CL_GOLD 0xA508
#define GLCD_CL_NAVY 0x000F
#define GLCD_CL_MAROON 0x7800
#define GLCD_CL_PURPLE 0x780F
#define GLCD_CL_OLIVE 0x7BE0
#endif

// Writes a command to the ILI9341 controller.
void ili9341_wrcmd(unsigned char cmd);
// Writes a command data byte to the ILI9341 controller.
void ili9341_wrdata(unsigned char data);
// Reads a command result data byte from the ILI9341 controller.
unsigned char ili9341_rddata(void);
// Writes a color data for 1 pixel to the ILI9341 controller's display RAM.
void ili9341_wrdram(GLCDCOL_t color);
// Reads color data for 1 pixel from the ILI9341 controller's display RAM.
GLCDCOL_t ili9341_rddram(void);
// Puts/exits the ILI9341 controller to/from sleep mode.
void ili9341_sleep(bool on);

// Initializes the graphic controller and clears the LCD.
bool glcd_init(GLCDINIT_t *init_data);
// Turns LCD on/off.
void glcd_display(bool on);
// Clears the LCD by setting it's color to the background color.
void glcd_clear(void);
// Sets the color of the pixel at coordinates x, y.
void glcd_putpixel(GLCDX_t x, GLCDY_t y, GLCDCOL_t color);
// Returns the color of the pixel at coordinates x, y.
GLCDCOL_t glcd_getpixel(GLCDX_t x, GLCDY_t y);

// Writes/reads a block of bytes as a bitmap image
// at/from specified coordinates.
void glcd_block(GLCDX_t left, GLCDY_t top, GLCDX_t width, GLCDY_t height,
     GLCDMEM_t memt, GLCDMEMADDR_t addr, GLCDBLOCKMODE_t mode);

// Fast draw a horizontal line from (x0,y0) to (x1,y0).
void glcd_lineh(GLCDX_t x0, GLCDY_t y0, GLCDX_t x1);
// Fast draw a vertical line from (x0,y0) to (x0,y1).
void glcd_linev(GLCDX_t x0, GLCDY_t y0, GLCDY_t y1);

#pragma library glcd_ili9341.lib

#endif




