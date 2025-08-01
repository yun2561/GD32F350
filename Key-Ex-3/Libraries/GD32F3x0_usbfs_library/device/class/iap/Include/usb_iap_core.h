/*!
    \file    usb_iap_core.h
    \brief   the header file of IAP driver

    \version 2025-01-01, V2.5.0, firmware for GD32F3x0
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef USB_IAP_CORE_H
#define USB_IAP_CORE_H

#include "usbd_enum.h"
#include "usb_hid.h"

#define USB_SERIAL_STRING_SIZE              0x06U                                  /*!< serial string size */
#define USB_DESC_LEN_IAP_REPORT             35U                                    /*!< report descriptor length */
#define USB_DESC_LEN_IAP_CONFIG_SET         41U                                    /*!< configuration descriptor length */

/* special commands with download request */
#define IAP_READ_OPTION_BYTE                0x01U                                  /*!< read option byte request */
#define IAP_ERASE                           0x02U                                  /*!< erase request */
#define IAP_DOWNLOAD                        0x03U                                  /*!< download request */
#define IAP_LEAVE                           0x04U                                  /*!< leave request */
#define IAP_GETBIN_ADDRESS                  0x05U                                  /*!< get bin address request */
#define IAP_WRITE_OPTION_BYTE               0x06U                                  /*!< write option byte request */
#define IAP_UPLOAD                          0x07U                                  /*!< upload request */
#define IAP_CHECK_RDP                       0x08U                                  /*!< check rdp state request */

#define OPERATION_SUCCESS                   0x02U                                  /*!< operation success status */
#define OPERATION_FAIL                      0x5FU                                  /*!< operation fail status */
#define LEAVE_FINISH                        0x04U                                  /*!< leave finish status */
#define OB_WRITE_SUCCESS                    0x03U                                  /*!< OB write success status */
#define IS_RDP_MODE                         0xBBU                                  /*!< MCU RDP status */
#define IS_NORMAL_MODE                      0xA5U                                  /*!< MCU normal status */

#define IAP_HOST_ID                         0x01U                                  /*!< IAP host ID */
#define IAP_DEVICE_ID                       0x02U                                  /*!< IAP device ID */

typedef struct {
    uint8_t report_buf[IAP_OUT_PACKET + 1U];                                       /*!< report buff */
    uint8_t option_byte[IAP_IN_PACKET];                                            /*!< option byte buff */

    /* state machine variables */
    uint8_t dev_status[IAP_IN_PACKET];                                             /*!< device status */
    uint8_t bin_addr[IAP_IN_PACKET];                                               /*!< load bin address */
    uint8_t reportID;                                                              /*!< report id */
    uint8_t flag;                                                                  /*!< flag */
    uint32_t protocol;                                                             /*!< control request protocol */
    uint32_t idlestate;                                                            /*!< control request idle state */
    uint16_t transfer_times;                                                       /*!< data transfer times */
    uint16_t page_count;                                                           /*!< memory page count */
    uint32_t file_length;                                                          /*!< file length*/
    uint32_t base_address;                                                         /*!< loaded base address */
} usbd_iap_handler;

typedef void (*app_func)(void);

extern usb_desc iap_desc;
extern usb_class_core iap_class;

/* function declarations */
/* send IAP report */
uint8_t iap_report_send(usb_dev *udev, uint8_t *report, uint32_t len);

#endif /* USB_IAP_CORE_H */
