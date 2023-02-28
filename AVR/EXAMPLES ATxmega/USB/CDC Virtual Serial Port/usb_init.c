/*******************************************************
This program was created by the
CodeWizardAVR V3.13 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : USB CDC Virtual Serial Port Example
*******************************************************/

#include "usb_init.h"

// USB device descriptors

flash USB_DEVICE_DESCRIPTOR_t usb_device_descriptor =
{
.bLength=sizeof(USB_DEVICE_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_DEVICE,
.bcdUSB=USB_SPEC,
.bDeviceClass=USB_CLASS_COMM_DEV_COMM,
.bDeviceSubClass=USB_SUBCLASS_NONE,
.bDeviceProtocol=USB_PROTOCOL_NONE,
.bMaxPacketSize0=USB_ENDPOINT0_SIZE,
.idVendor=USB_VENDOR_ID,
.idProduct=USB_PRODUCT_ID,
.bcdDevice=USB_DEVICE_RELEASE,
.iManufacturer=1,
.iProduct=2,
.iSerialNumber=3,
.bNumConfigurations=1
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

// Interface 0 - CDC Control
USB_INTERFACE_DESCRIPTOR_t interface0_descriptor;
USB_CDC_HEADER_FUNC_DESCRIPTOR_t cdc_header_func_descriptor;
USB_CDC_ACM_FUNC_DESCRIPTOR_t cdc_acm_func_descriptor;
USB_CDC_UNION_FUNC_DESCRIPTOR_t cdc_union_func_descriptor;
USB_ENDPOINT_DESCRIPTOR_t interface0_in_endpoint_descriptor;

// Interface 1 - CDC Data
USB_INTERFACE_DESCRIPTOR_t interface1_descriptor;
USB_ENDPOINT_DESCRIPTOR_t interface1_out_endpoint_descriptor;
USB_ENDPOINT_DESCRIPTOR_t interface1_in_endpoint_descriptor;
} usb_config_descriptor=
{
// Configuration Descriptor
{
.bLength=sizeof(USB_CONFIG_DESCRIPTOR_t),
.bDescriptorType=USB_DESCRIPTOR_TYPE_CONFIGURATION,
.wTotalLength=sizeof(usb_config_descriptor),
.bNumInterfaces=2,
.bConfigurationValue=1,
.iConfiguration=USB_STRING_NONE,
.bmAttributes=USB_ATTR_COMPAT1 | USB_ATTR_SELF_POWERED,
.bMaxPower=100/2
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
}
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex)
flash USB_DESCRIPTOR_LIST_t usb_descriptor_list[]=
{
{0,USB_DESCRIPTOR_TYPE_DEVICE,0,&usb_device_descriptor,sizeof(usb_device_descriptor)},
{0,USB_DESCRIPTOR_TYPE_CONFIGURATION,0,&usb_config_descriptor,sizeof(usb_config_descriptor)},
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
}
},
.pdescriptor_list=usb_descriptor_list,
.descriptor_list_items=sizeof(usb_descriptor_list)/sizeof(USB_DESCRIPTOR_LIST_t),
.timeout={.tx=USB_TX_TIMEOUT,.rx=USB_RX_TIMEOUT},
// USB interrupt priority level: High
.int_level=USB_INTLVL_HI_gc,
// No device suspend handler is used
.usb_suspend_handler=NULL,
// No device resume handler is used
.usb_resume_handler=NULL
};
