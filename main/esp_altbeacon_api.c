/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/



/****************************************************************************
*
* This file is for iBeacon APIs. It supports both iBeacon encode and decode. 
*
* iBeacon is a trademark of Apple Inc. Before building devices which use iBeacon technology,
* visit https://developer.apple.com/ibeacon/ to obtain a license.
*
****************************************************************************/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "esp_gap_ble_api.h"
#include "esp_altbeacon_api.h"


const uint8_t uuid_zeros[ESP_UUID_LEN_128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* For iBeacon packet format, please refer to Apple "Proximity Beacon Specification" doc */
/* Constant part of iBeacon data */
esp_ble_altbeacon_head_t altbeacon_common_head = {
    .flags = {0x02, 0x01, 0x00},
    .length = 0x1B,
    .type = 0xFF,
    .company_id = 0xFFFF,
    .beacon_type = 0xACBE
};

/* Vendor part of iBeacon data*/
esp_ble_altbeacon_vendor_t vendor_config = {
    .beacon_id = BEACON_ID,
    .beacon_id_extra = BEACON_ID_EXTRA,
    .reference_rssi = 0xC5,
    .mfg_reserved = 0x11
};

bool esp_ble_is_ibeacon_packet (uint8_t *adv_data, uint8_t adv_data_len){
    bool result = false;

    if ((adv_data != NULL) && (adv_data_len == 0x1E)){
        if (!memcmp(adv_data, (uint8_t*)&altbeacon_common_head, sizeof(altbeacon_common_head))){
            result = true;
        }
    }

    return result;
}

esp_err_t esp_ble_config_altbeacon_data (esp_ble_altbeacon_vendor_t *vendor_config, esp_ble_altbeacon_t *altbeacon_adv_data){
    if ((vendor_config == NULL) || (altbeacon_adv_data == NULL) || (!memcmp(vendor_config->beacon_id, uuid_zeros, sizeof(uuid_zeros)))){
        return ESP_ERR_INVALID_ARG;
    }

    memcpy(&altbeacon_adv_data->altbeacon_head, &altbeacon_common_head, sizeof(esp_ble_altbeacon_head_t));
    memcpy(&altbeacon_adv_data->altbeacon_vendor, vendor_config, sizeof(esp_ble_altbeacon_vendor_t));

    return ESP_OK;
}

