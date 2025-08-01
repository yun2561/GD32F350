/*!
    \file    usbd_msc_core.h
    \brief   the header file of USB MSC device class core functions

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

#ifndef USBD_MSC_CORE_H
#define USBD_MSC_CORE_H

#include "usbd_core.h"
#include "usb_msc.h"

#define USB_MSC_CONFIG_DESC_SIZE          32U                                   /*!< MSC configuration descriptor size */

#define MSC_EPIN_SIZE                     MSC_DATA_PACKET_SIZE                  /*!< MSC endpoint IN size */
#define MSC_EPOUT_SIZE                    MSC_DATA_PACKET_SIZE                  /*!< MSC endpoint OUT size */

/* USB configuration descriptor structure */
typedef struct {
    usb_desc_config         config;                                             /*!< configuration descriptor */
    usb_desc_itf            msc_itf;                                            /*!< interface descriptor */
    usb_desc_ep             msc_epin;                                           /*!< endpoint IN descriptor */
    usb_desc_ep             msc_epout;                                          /*!< endpoint OUT descriptor */
} usb_desc_config_set;

extern usb_desc msc_desc;
extern usb_class_core msc_class;

#endif /* USBD_MSC_CORE_H */
