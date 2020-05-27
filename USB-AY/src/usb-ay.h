
// Generic types
typedef unsigned long long  U64;
typedef signed long long    I64;
typedef unsigned long       U32;
typedef signed long         I32;
typedef unsigned short      U16;
typedef signed short        I16;
typedef unsigned char       U8;
typedef signed char         I8;
typedef unsigned long long  u64;
typedef signed long long    i64;
typedef unsigned long       u32;
typedef signed long         i32;
typedef unsigned short      u16;
typedef signed short        i16;
typedef unsigned char       u8;
typedef signed char         i8;

// Types
typedef void (*USB_RX_CB)(char*, int*);

// Pins
#define AD_PORT GPIOC
#define AD_CS   GPIO13
#define AD_CLK  GPIO14
#define AD_SDI  GPIO15

#define PSG_DATA_P GPIOA

#define PSG_CTRL_P GPIOB
#define PSG_BC1   GPIO5
#define PSG_BDIR  GPIO1
#define PSG_RES   GPIO9

#define PSG_TIM   TIM2
#define PSG_CLK_P GPIOB
#define PSG_CLK   GPIO10

#define PSG_ADDR  PSG_BDIR | PSG_BC1 // Write PSG address
#define PSG_WDAT  PSG_BDIR          // Write PSG data
#define PSG_RDAT  PSG_BC1           // Read PSG data

// Vars
// extern const struct usb_device_descriptor dev;
// extern const struct usb_config_descriptor config;
// extern const char *usb_strings[];
// extern uint8_t usbd_control_buffer[128];
extern USB_RX_CB usb_rx_cb;

// Functions
void cdcacm_set_config(usbd_device*, uint16_t);
void init_hw();
void ad5206_write(u8, u8);
void psg_write(u8, u8);
