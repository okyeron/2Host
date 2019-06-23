#include "usb_names.h"

// Edit these lines to modify the device name.
// The length must match the number of characters in your custom name.

#define PRODUCT_NAME \
    { '2', '-', 'h', 'o', 's', 't'}
#define PRODUCT_NAME_LEN 6


// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_product_name = {
    2 + PRODUCT_NAME_LEN * 2, 3, PRODUCT_NAME
};
