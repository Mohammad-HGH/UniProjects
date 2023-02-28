/*******************************************************
USB library initialization created by the
CodeWizardAVR V3.11 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project: USB Generic HID Example
*******************************************************/

#ifndef _USB_INIT_INCLUDED_
#define _USB_INIT_INCLUDED_

#include <usb_device.h>
#include <usb_hid.h>

// Manufacturer string
#define USB_STR_MANUFACTURER 'H','P',' ','I','n','f','o','T','e','c','h',' ','S','.','R','.','L','.'
// Number of characters in the manufacturer string
#define USB_STR_MANUFACTURER_CHARS 18

// Product string
#define USB_STR_PRODUCT 'C','o','d','e','V','i','s','i','o','n','A','V','R',' ','U','S','B',' ','D','e','v','i','c','e'
// Number of characters in the product string
#define USB_STR_PRODUCT_CHARS 24

// Serial number string
#define USB_STR_SERIAL_NUMBER '0','0','0','0','1'
// Number of characters in the serial number string
#define USB_STR_SERIAL_NUMBER_CHARS 5

// idVendor
// Vendor ID, must match the INF file
#define USB_VENDOR_ID 0x03EB

// idProduct value
// Product ID, must match the INF file
#define USB_PRODUCT_ID 0x311F

// bcdDevice value
// Device release number in BCD format
#define USB_DEVICE_RELEASE 0x311

// Transmission timeout period [ms]
#define USB_TX_TIMEOUT 100

// Reception timeout period [ms]
#define USB_RX_TIMEOUT 250

// Interface 0 HID Report Vendor Page Number
#define USB_INTERFACE0_VENDOR_PAGE_NUMBER 0xFFAB

// Interface 0 HID Report Collection Usage
#define USB_INTERFACE0_COLLECTION_USAGE 0x0200

// Interface 0 IN endpoint settings
#define USB_INTERFACE0_IN_EP 3
#define USB_INTERFACE0_IN_EP_SIZE 64
#define USB_INTERFACE0_IN_EP_SERVICE_INTERVAL 5

// Interface 0 OUT endpoint settings
#define USB_INTERFACE0_OUT_EP 4
#define USB_INTERFACE0_OUT_EP_SIZE 64
#define USB_INTERFACE0_OUT_EP_SERVICE_INTERVAL 5

// USB configuration table
extern flash USB_CONFIG_t usb_config;
#endif

