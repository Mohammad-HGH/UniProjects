/*******************************************************
This program was created by the
CodeWizardAVR V3.13 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : USB Composite Device Example
*******************************************************/

#include "usb_init.h"

// USB device descriptors

flash USB_DEVICE_DESCRIPTOR_t usb_device_descriptor =
{
.bLength=sizeof(USB_DEVICE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_DEVICE,
.bcdUSB=USB_SPEC,
.bDeviceClass=USB_CLASS_INTERFACE_ASSOC_DESCRIPTOR,
.bDeviceSubClass=USB_SUBCLASS_INTERFACE_ASSOC_DESCRIPTOR,
.bDeviceProtocol=USB_PROTOCOL_INTERFACE_ASSOC_DESCRIPTOR,
.bMaxPacketSize0=USB_ENDPOINT0_SIZE,
.idVendor=USB_VENDOR_ID,
.idProduct=USB_PRODUCT_ID,
.bcdDevice=USB_DEVICE_RELEASE,
.iManufacturer=1,
.iProduct=2,
.iSerialNumber=3,
.bNumConfigurations=1
};

// HID Mouse report descriptor
flash unsigned char hid_mouse_report_descriptor[]=
{
HID_RD_USAGE_PAGE(8,1), // Generic desktop
HID_RD_USAGE(8,2), // Mouse
HID_RD_COLLECTION(8,HID_RD_COLLECTION_APPLICATION),
HID_RD_USAGE_PAGE(8,9), // Buttons
HID_RD_USAGE_MIN(8,1), // Button 1
HID_RD_USAGE_MAX(8,3), // Button 3
HID_RD_LOGICAL_MIN(8,0),
HID_RD_LOGICAL_MAX(8,1),
HID_RD_REPORT_COUNT(8,3),
HID_RD_REPORT_SIZE(8,1),
HID_RD_INPUT(8,HID_RD_DATA_F | HID_RD_VARIABLE_F | HID_RD_ABSOLUTE_F),
HID_RD_REPORT_COUNT(8,1),
HID_RD_REPORT_SIZE(8,5),
HID_RD_INPUT(8,HID_RD_CONSTANT_F | HID_RD_VARIABLE_F),
HID_RD_USAGE_PAGE(8,1), // Generic desktop
HID_RD_USAGE(8,0x30), // x
HID_RD_USAGE(8,0x31), // y
HID_RD_LOGICAL_MIN(8,-127),
HID_RD_LOGICAL_MAX(8,127),
HID_RD_REPORT_SIZE(8,8),
HID_RD_REPORT_COUNT(8,2),
HID_RD_INPUT(8,HID_RD_DATA_F | HID_RD_VARIABLE_F | HID_RD_RELATIVE_F),
HID_RD_USAGE(8,0x38), // wheel
HID_RD_REPORT_COUNT(8,1),
HID_RD_INPUT(8,HID_RD_DATA_F | HID_RD_VARIABLE_F | HID_RD_RELATIVE_F),
HID_RD_END_COLLECTION(0)
};

// HID Joystick report descriptor
flash unsigned char hid_joystick_report_descriptor[]=
{
HID_RD_USAGE_PAGE(8,1), // Generic desktop
HID_RD_USAGE(8,4), // Joystick
HID_RD_COLLECTION(8,HID_RD_COLLECTION_APPLICATION),
HID_RD_USAGE(8,1),
HID_RD_COLLECTION(8,HID_RD_COLLECTION_PHYSICAL),
HID_RD_USAGE(8,0x30),
HID_RD_USAGE(8,0x31),
HID_RD_USAGE(8,0x32),
HID_RD_LOGICAL_MIN(16,-100),
HID_RD_LOGICAL_MAX(16,100),
HID_RD_PHYSICAL_MIN(16,-1),
HID_RD_PHYSICAL_MAX(16,1),
HID_RD_REPORT_COUNT(8,3),
HID_RD_REPORT_SIZE(8,8),
HID_RD_INPUT(8,HID_RD_DATA_F | HID_RD_VARIABLE_F | HID_RD_ABSOLUTE_F),
HID_RD_END_COLLECTION(0),
HID_RD_USAGE_PAGE(8,9), // Buttons
HID_RD_USAGE_MIN(8,1), // Button 1
HID_RD_USAGE_MAX(8,2), // Button 2
HID_RD_LOGICAL_MIN(8,0),
HID_RD_LOGICAL_MAX(8,1),
HID_RD_REPORT_SIZE(8,1),
HID_RD_REPORT_COUNT(8,2),
HID_RD_INPUT(8,HID_RD_DATA_F | HID_RD_VARIABLE_F | HID_RD_ABSOLUTE_F),
HID_RD_REPORT_SIZE(8,6),
HID_RD_REPORT_COUNT(8,1),
HID_RD_INPUT(8,HID_RD_CONSTANT_F | HID_RD_ARRAY_F | HID_RD_ABSOLUTE_F),
HID_RD_END_COLLECTION(0)
};

// Strings descriptors
flash unsigned short usb_string0[]=
{
2+sizeof(short)+ // bLength
(USB_DESCRIPTOR_TYPE_STRING<<8), // bDescriptorType
USB_LANG_ID_US_ENGLISH // wLangID
};

flash unsigned short usb_string1[]=
{
2+USB_STR_MANUFACTURER_CHARS*2+ // bLength
(USB_DESCRIPTOR_TYPE_STRING<<8), // bDescriptorType
USB_STR_MANUFACTURER // bString
};

flash unsigned short usb_string2[]=
{
2+USB_STR_PRODUCT_CHARS*2+ // bLength
(USB_DESCRIPTOR_TYPE_STRING<<8), // bDescriptorType
USB_STR_PRODUCT // bString
};

flash unsigned short usb_string3[]=
{
2+USB_STR_SERIAL_NUMBER_CHARS*2+ // bLength
(USB_DESCRIPTOR_TYPE_STRING<<8), // bDescriptorType
USB_STR_SERIAL_NUMBER // bString
};

// USB configuration descriptor
flash struct
{
// Configuration Descriptor
USB_CONFIG_DESCRIPTOR_t config_descriptor;

// Interface 0 Association Descriptor
USB_INTERFACE_ASSOC_DESCRIPTOR_t interface_assoc_descriptor0;

// Interface 0 - CDC Control
USB_INTERFACE_DESCRIPTOR_t interface_descriptor0;
USB_CDC_HEADER_FUNC_DESCRIPTOR_t cdc_header_func_descriptor0;
USB_CDC_ACM_FUNC_DESCRIPTOR_t cdc_acm_func_descriptor0;
USB_CDC_UNION_FUNC_DESCRIPTOR_t cdc_union_func_descriptor0;
USB_ENDPOINT_DESCRIPTOR_t interface_in_endpoint_descriptor0;

// Interface 1 - CDC Data
USB_INTERFACE_DESCRIPTOR_t interface_descriptor1;
USB_ENDPOINT_DESCRIPTOR_t interface_out_endpoint_descriptor1;
USB_ENDPOINT_DESCRIPTOR_t interface_in_endpoint_descriptor1;

// Interface 2 - HID Mouse
USB_INTERFACE_DESCRIPTOR_t interface_descriptor2;
USB_HID_INTERFACE_DESCRIPTOR_t interface_hid_descriptor2;
USB_ENDPOINT_DESCRIPTOR_t interface_in_endpoint_descriptor2;

// Interface 3 - HID Joystick
USB_INTERFACE_DESCRIPTOR_t interface_descriptor3;
USB_HID_INTERFACE_DESCRIPTOR_t interface_hid_descriptor3;
USB_ENDPOINT_DESCRIPTOR_t interface_in_endpoint_descriptor3;
} usb_config_descriptor=
{
// Configuration Descriptor
{
.bLength=sizeof(USB_CONFIG_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_CONFIGURATION,
.wTotalLength=sizeof(usb_config_descriptor),
.bNumInterfaces=4,
.bConfigurationValue=1,
.iConfiguration=USB_STRING_NONE,
.bmAttributes=USB_ATTR_COMPAT1 | USB_ATTR_SELF_POWERED,
.bMaxPower=100/2
},

// Interface 0 Association Descriptor
.interface_assoc_descriptor0=
{
.bLength=sizeof(USB_INTERFACE_ASSOC_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_INTERFACE_ASSOC,
.bFirstInterface=0,
.bInterfaceCount=2,
.bFunctionClass=USB_CLASS_COMM_DEV_COMM,
.bFunctionSubClass=USB_SUBCLASS_ACM,
.bFunctionProtocol=USB_PROTOCOL_CDC_AT,
.iFunction=USB_STRING_NONE
},

// Interface 0 - CDC Control
// Descriptor
{
.bLength=sizeof(USB_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_INTERFACE,
.bInterfaceNumber=0,
.bAlternateSetting=USB_ALTERNATE_SETTING_NONE,
.bNumEndpoints=1,
.bInterfaceClass=USB_CLASS_COMM_DEV_COMM,
.bInterfaceSubClass=USB_SUBCLASS_ACM,
.bInterfaceProtocol=USB_PROTOCOL_CDC_AT,
.iInterface=USB_STRING_NONE
},

// CDC Header Functional Descriptor
{
.bFunctionLength=sizeof(USB_CDC_HEADER_FUNC_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_CS_INTERFACE,
.bDescriptorSubtype=CDC_DST_HEADER,
.bcdCDC=USB_CDC_SPEC
},

// Abstract Control Management Functional Descriptor
{
.bFunctionLength=sizeof(USB_CDC_ACM_FUNC_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_CS_INTERFACE,
.bDescriptorSubtype=CDC_DST_ACM,
// Device supports Send_Break, Set_Line_Coding,
// Set_Control_Line_State, Get_Line_Coding and
// the notification Serial_State
.bmCapabilities=CDC_ACM_SEND_BREAK | CDC_ACM_LC_CLS_SS
},

// Union Functional Descriptor
{
.bFunctionLength=sizeof(USB_CDC_UNION_FUNC_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_CS_INTERFACE,
.bDescriptorSubtype=CDC_DST_UNION,
.bMasterInterface=0, // Interface 0 is used for Control
.bSlaveInterface0=1  // Interface 1 is used for Data
},

// IN Endpoint descriptor - Notification
{
.bLength=sizeof(USB_ENDPOINT_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_ENDPOINT,
.bEndpointAddress=USB_INTERFACE0_IN_EP | USB_ENDPOINT_DIR_IN,
.bmAttributes=USB_TRANSFER_INTERRUPT | USB_EP_ATTR_NO_SYNC | USB_EP_USAGE_DATA,
.wMaxPacketSize=USB_INTERFACE0_IN_EP_SIZE,
.bInterval=USB_INTERFACE0_IN_EP_SERVICE_INTERVAL
},

// Interface 1 - CDC Data
// Descriptor
{
.bLength=sizeof(USB_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_INTERFACE,
.bInterfaceNumber=1, // Interface 1 is used for Data
.bAlternateSetting=USB_ALTERNATE_SETTING_NONE,
.bNumEndpoints=2,
.bInterfaceClass=USB_CLASS_COMM_DEV_DATA,
.bInterfaceSubClass=USB_SUBCLASS_NONE,
.bInterfaceProtocol=USB_PROTOCOL_NONE,
.iInterface=USB_STRING_NONE
},

// OUT Endpoint Descriptor - Data
{
.bLength=sizeof(USB_ENDPOINT_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_ENDPOINT,
.bEndpointAddress=USB_INTERFACE1_OUT_EP | USB_ENDPOINT_DIR_OUT,
.bmAttributes=USB_TRANSFER_BULK | USB_EP_ATTR_NO_SYNC | USB_EP_USAGE_DATA,
.wMaxPacketSize=USB_INTERFACE1_OUT_EP_SIZE,
.bInterval=0
},

// IN Endpoint Descriptor - Data
{
.bLength=sizeof(USB_ENDPOINT_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_ENDPOINT,
.bEndpointAddress=USB_INTERFACE1_IN_EP | USB_ENDPOINT_DIR_IN,
.bmAttributes=USB_TRANSFER_BULK | USB_EP_ATTR_NO_SYNC | USB_EP_USAGE_DATA,
.wMaxPacketSize=USB_INTERFACE1_IN_EP_SIZE,
.bInterval=0
},

// Interface 2 - HID Mouse
// Descriptor
{
.bLength=sizeof(USB_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_INTERFACE,
.bInterfaceNumber=2,
.bAlternateSetting=USB_ALTERNATE_SETTING_NONE,
.bNumEndpoints=1,
.bInterfaceClass=USB_CLASS_HID,
.bInterfaceSubClass=USB_SUBCLASS_HID_BOOT,
.bInterfaceProtocol=USB_PROTOCOL_MOUSE,
.iInterface=USB_STRING_NONE
},

// HID Interface Descriptor - Mouse
{
.bLength=sizeof(USB_HID_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_HID_INTERFACE,
.bcdHID=USB_HID_SPEC,
.bCountryCode=0,
.bNumDescriptors=1,
.bReportDescriptorType=USB_DESCRIPTOR_TYPE_REPORT,
.wDescriptorLength=sizeof(hid_mouse_report_descriptor)
},

// IN Endpoint Descriptor
{
.bLength=sizeof(USB_ENDPOINT_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_ENDPOINT,
.bEndpointAddress=USB_INTERFACE2_IN_EP | USB_ENDPOINT_DIR_IN,
.bmAttributes=USB_TRANSFER_INTERRUPT | USB_EP_ATTR_NO_SYNC | USB_EP_USAGE_DATA,
.wMaxPacketSize=USB_INTERFACE2_IN_EP_SIZE,
.bInterval=USB_INTERFACE2_IN_EP_SERVICE_INTERVAL
},

// Interface 3 - HID Joystick
// Descriptor
{
.bLength=sizeof(USB_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_INTERFACE,
.bInterfaceNumber=3,
.bAlternateSetting=USB_ALTERNATE_SETTING_NONE,
.bNumEndpoints=1,
.bInterfaceClass=USB_CLASS_HID,
.bInterfaceSubClass=USB_SUBCLASS_NONE,
.bInterfaceProtocol=USB_PROTOCOL_NONE,
.iInterface=USB_STRING_NONE
},

// HID Interface Descriptor - Joystick
{
.bLength=sizeof(USB_HID_INTERFACE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_HID_INTERFACE,
.bcdHID=USB_HID_SPEC,
.bCountryCode=0,
.bNumDescriptors=1,
.bReportDescriptorType=USB_DESCRIPTOR_TYPE_REPORT,
.wDescriptorLength=sizeof(hid_joystick_report_descriptor)
},

// IN Endpoint Descriptor
{
.bLength=sizeof(USB_ENDPOINT_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_ENDPOINT,
.bEndpointAddress=USB_INTERFACE3_IN_EP | USB_ENDPOINT_DIR_IN,
.bmAttributes=USB_TRANSFER_INTERRUPT | USB_EP_ATTR_NO_SYNC | USB_EP_USAGE_DATA,
.wMaxPacketSize=USB_INTERFACE3_IN_EP_SIZE,
.bInterval=USB_INTERFACE3_IN_EP_SERVICE_INTERVAL
}
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex)
flash USB_DESCRIPTOR_LIST_t usb_descriptor_list[]=
{
{0,USB_DESCRIPTOR_TYPE_DEVICE,0,&usb_device_descriptor,sizeof(usb_device_descriptor)},
{0,USB_DESCRIPTOR_TYPE_CONFIGURATION,0,&usb_config_descriptor,sizeof(usb_config_descriptor)},
{0,USB_DESCRIPTOR_TYPE_HID_INTERFACE,2 /* Interface # */,&usb_config_descriptor.interface_hid_descriptor2,sizeof(USB_HID_INTERFACE_DESCRIPTOR_t)},
{0,USB_DESCRIPTOR_TYPE_REPORT,2 /* Interface # */,hid_mouse_report_descriptor,sizeof(hid_mouse_report_descriptor)},
{0,USB_DESCRIPTOR_TYPE_HID_INTERFACE,3 /* Interface # */,&usb_config_descriptor.interface_hid_descriptor3,sizeof(USB_HID_INTERFACE_DESCRIPTOR_t)},
{0,USB_DESCRIPTOR_TYPE_REPORT,3 /* Interface # */,hid_joystick_report_descriptor,sizeof(hid_joystick_report_descriptor)},
{0,USB_DESCRIPTOR_TYPE_STRING,0,&usb_string0,sizeof(usb_string0)},
{1,USB_DESCRIPTOR_TYPE_STRING,USB_LANG_ID_US_ENGLISH,&usb_string1,sizeof(usb_string1)},
{2,USB_DESCRIPTOR_TYPE_STRING,USB_LANG_ID_US_ENGLISH,&usb_string2,sizeof(usb_string2)},
{3,USB_DESCRIPTOR_TYPE_STRING,USB_LANG_ID_US_ENGLISH,&usb_string3,sizeof(usb_string3)}
};

// USB device configuration
flash USB_CONFIG_t usb_config=
{
// Interface(s) configuration
.interface=
{
// Interface 0
{
.bInterfaceClass=USB_CLASS_COMM_DEV_COMM,
.bInterfaceProtocol=USB_PROTOCOL_CDC_AT,
.report_data=&usb_cdc_serial[0].parameters,
.report_size=sizeof(USB_CDC_LINE_CODING_t),
// IN endpoint configuration
.in={.ep=USB_INTERFACE0_IN_EP,.type=USB_TRANSFER_INTERRUPT,.size=USB_INTERFACE0_IN_EP_SIZE}
},
// Interface 1
{
.bInterfaceClass=USB_CLASS_COMM_DEV_DATA,
.bInterfaceProtocol=USB_PROTOCOL_NONE,
.report_data=0,
.report_size=0,
// IN endpoint configuration
.in={.ep=USB_INTERFACE1_IN_EP,.type=USB_TRANSFER_BULK,.size=USB_INTERFACE1_IN_EP_SIZE},
// OUT endpoint configuration
.out={.ep=USB_INTERFACE1_OUT_EP,.type=USB_TRANSFER_BULK,.size=USB_INTERFACE1_OUT_EP_SIZE}
},
// Interface 2
{
.bInterfaceClass=USB_CLASS_HID,
.bInterfaceProtocol=USB_PROTOCOL_MOUSE,
.report_data=&usb_mouse,
.report_size=sizeof(USB_MOUSE_t),
// IN endpoint configuration
.in={.ep=USB_INTERFACE2_IN_EP,.type=USB_TRANSFER_INTERRUPT,.size=USB_INTERFACE2_IN_EP_SIZE}
},
// Interface 3
{
.bInterfaceClass=USB_CLASS_HID,
.bInterfaceProtocol=USB_PROTOCOL_NONE,
.report_data=&usb_joystick,
.report_size=sizeof(USB_JOYSTICK_t),
// IN endpoint configuration
.in={.ep=USB_INTERFACE3_IN_EP,.type=USB_TRANSFER_INTERRUPT,.size=USB_INTERFACE3_IN_EP_SIZE}
}
},
.pdescriptor_list=usb_descriptor_list,
.descriptor_list_items=sizeof(usb_descriptor_list)/sizeof(USB_DESCRIPTOR_LIST_t),
.timeout={.tx=USB_TX_TIMEOUT,.rx=USB_RX_TIMEOUT},
// Use the internal 32 MHz RC oscillator, adjusted and
// calibrated by the the DFLL and USB Start of Frame
// to 48 MHz, as USB clock source
.usb_clock=USB_CLOCK_RC32M_SOF,
// USB interrupt priority level: High Level
.int_level=USB_INTLVL_HI_gc,
// No device suspend handler is used
.usb_suspend_handler=NULL,
// No device resume handler is used
.usb_resume_handler=NULL
};
