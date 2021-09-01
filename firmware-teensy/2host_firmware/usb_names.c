#include "usb_names.h"

// Edit these lines to modify the device name.
// The length must match the number of characters in your custom name.

#define PRODUCT_NAME { 't', 'w', 'o', 'h', 'o', 's', 't'}
#define PRODUCT_NAME_LEN 7
#define MANUFACTURER_NAME  {'d','e','n','k','i','o','t','o'}
#define MANUFACTURER_NAME_LEN 8

struct usb_string_descriptor_struct usb_string_product_name = {
  2 + PRODUCT_NAME_LEN * 2,
  3,
  PRODUCT_NAME
};

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
  2 + MANUFACTURER_NAME_LEN * 2,
  3,
  MANUFACTURER_NAME
};
