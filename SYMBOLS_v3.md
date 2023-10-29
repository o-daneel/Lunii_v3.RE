
- [Symbols](#symbols)
  - [Core](#core)
    - [Vector Tables](#vector-tables)
    - [FreeRTOS](#freertos)
    - [Low-Level](#low-level)
    - [OS \& Lib C](#os--lib-c)
    - [Others](#others)
  - [Test mode](#test-mode)
  - [HALs](#hals)
    - [FatFs](#fatfs)
    - [Audio](#audio)
    - [Crypto](#crypto)
    - [Storage](#storage)
    - [Screen](#screen)
    - [NFC](#nfc)
    - [Others](#others-1)
  - [Resources](#resources)
  - [Raw Table](#raw-table)


# Symbols

**!! Work In Progress !!**
WIP

## Core

VectorTable

### Vector Tables
One for each firmware:
* boot - `0x0800 0000`
* main - `0x9000 0000`
* backup - `0x8000 0000` (TBC)

See chapter [9.1.2 Interrupt and exception vectors ](docs/rm0431-stm32f72xxx-and-stm32f73xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
Reset_Handler	900261c4	Function	Global	User Defined	0	1
main	9001b33e	Function	Global	User Defined	1	0


### FreeRTOS
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
task_access_point	90020710	Function	Global	User Defined	0	1
task_audio_loop	9001f25c	Function	Global	User Defined	0	1
task_battery_charge	9002063e	Function	Global	User Defined	0	1
task_esp_parser	900251c2	Function	Global	User Defined	0	1
task_menu	900207fc	Function	Global	User Defined	0	1
task_play_story	90021560	Function	Global	User Defined	0	1
task_test_mode	9002000a	Function	Global	User Defined	0	1
task_wheel_mode	90020272	Function	Global	User Defined	0	1


### Low-Level
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### OS & Lib C

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
sprintf	90026608	Function	Global	User Defined	21	0
strcat	90026760	Function	Global	User Defined	32	0
strcmp	90000f80	Function	Global	User Defined	31	0
strlen	90000f94	Function	Global	User Defined	62	0
strncpy	90026866	Function	Global	User Defined	27	0
memcpy	9000aeb0	Function	Global	User Defined	14	0
memset	9000ad1e	Function	Global	User Defined	14	0
printf	90026190	Function	Global	User Defined	38	0
printf_level	90015164	Function	Global	User Defined	437	0


### Others

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
base64_url_decode	9001e3a2	Function	Global	User Defined	1	0
set_hp_level	9001748a	Function	Global	User Defined	1	0
set_volume	90017580	Function	Global	User Defined	2	0
sleep_ms	9000d7a6	Function	Global	User Defined	10	0
get_battery_level	900161a4	Function	Global	User Defined	4	0
fw_version	9003708a	Data Label	Global	User Defined	7	1

## Test mode

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |



## HALs

### FatFs
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
| clmt_clust | 9000ad40 | Function | Global | User Defined | 3 |
| clst2sect | 9000ad2a | Function | Global | User Defined | 10 |
| create_chain | 9000b4fe | Function | Global | User Defined | 5 |
| dir_clear | 9000aec2 | Function | Global | User Defined | 3 |
| dir_next | 9000b630 | Function | Global | User Defined | 6 |
| dir_read | 9000b75e | Function | Global | User Defined | 3 |
| dir_remove | 9000b862 | Function | Global | User Defined | 2 |
| dir_sdi | 9000b5ae | Function | Global | User Defined | 9 |
| disk_ioctl | 9000acee | Function | Global | User Defined | 4 |
| disk_read | 9000acb6 | Function | Global | User Defined | 6 |
| disk_status | 9000ac7e | Function | Global | User Defined | 2 |
| disk_write | 9000acd2 | Function | Global | User Defined | 16 |
| f_chmod | 9000cd84 | Function | Global | User Defined | 4 |
| f_close | 9000c50c | Function | Global | User Defined | 23 |
| f_closedir | 9000c7f6 | Function | Global | User Defined | 2 |
| f_expand | 9000cf2a | Function | Global | User Defined | 1 |
| f_lseek | 9000c534 | Function | Global | User Defined | 10 |
| f_mkdir | 9000cb02 | Function | Global | User Defined | 2 |
| f_open | 9000bf6e | Function | Global | User Defined | 17 |
| f_opendir | 9000c774 | Function | Global | User Defined | 2 |
| f_read | 9000c13c | Function | Global | User Defined | 9 |
| f_readdir | 9000c814 | Function | Global | User Defined | 1 |
| f_rename | 9000cc70 | Function | Global | User Defined | 7 |
| f_rmdir_recursive | 9001604a | Function | Global | User Defined | 2 |
| f_sync | 9000c46a | Function | Global | User Defined | 1 |
| f_unlink | 9000ca52 | Function | Global | User Defined | 8 |
| f_write | 9000c2c4 | Function | Global | User Defined | 12 |
| fil_loopts | 2002db3c | Data Label | Global | User Defined | 3 |
| follow_path | 9000bca2 | Function | Global | User Defined | 8 |
| get_fat | 9000b3e2 | Function | Global | User Defined | 12 |
| get_ldnumber | 9000ad98 | Function | Global | User Defined | 4 |
| ld_clust | 9000b3d2 | Function | Global | User Defined | 6 |
| ld_dword | 9000ad0a | Function | Global | User Defined | 16 |
| lock_volume | 9000d710 | Function | Global | User Defined | 2 |
| mount_volume | 9000b16c | Function | Global | User Defined | 10 |
| move_window | 9000af1a | Function | Global | User Defined | 20 |
| put_fat | 9000afc4 | Function | Global | User Defined | 5 |
| remove_chain | 9000b48e | Function | Global | User Defined | 5 |
| st_clust | 9000ad62 | Function | Global | User Defined | 6 |
| st_word | 9000ad0e | Function | Global | User Defined | 18 |
| sync_fs | 9000b0d6 | Function | Global | User Defined | 6 |
| sync_window | 9000af0e | Function | Global | User Defined | 4 |
| unlock_volume | 9000b0c0 | Function | Global | User Defined | 28 |
| validate | 9000bc52 | Function | Global | User Defined | 9 |
GET_FATTIME	9001aeae	Function	Global	User Defined	5	0

### Audio
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
set_hp_level
set_volume

### Crypto
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
HAL_CRYP_Decrypt	90002be6	Function	Global	User Defined	5	0
HAL_CRYP_DeInit	900021a8	Function	Global	User Defined	10	0
HAL_CRYP_Encrypt	90002abc	Function	Global	User Defined	5	0
HAL_CRYP_ErrorCallback	900021d6	Function	Global	User Defined	3	0
HAL_CRYP_Init	90002164	Function	Global	User Defined	1	0
HAL_CRYP_MspDeInit	90014c88	Function	Global	User Defined	1	0
HAL_CRYP_MspInit	90014c54	Function	Global	User Defined	1	0
CRYP_AES_ProcessData	900021d8	Function	Global	User Defined	4	0
CRYP_AESCCM_Process	900022aa	Function	Global	User Defined	2	0
CRYP_AESGCM_Process	90002552	Function	Global	User Defined	2	0
CRYP_SetKey	90002122	Function	Global	User Defined	6	0
CRYP_WaitOnCCFlag	900020f4	Function	Global	User Defined	13	0
AES_CONFIG	200185f0	Data Label	Global	User Defined	23	8
TBC_AES_Init	90014c06	Function	Global	User Defined	10	0
TBC_AES_KEY_1	2000a5c8	Data Label	Global	User Defined	24	0
TBC_AES_KEY_2	2000a5cc	Data Label	Global	User Defined	7	0
TBC_AES_KeysInit	90014cae	Function	Global	User Defined	10	0

### Storage
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
HAL_FS_dirCreate	9001d2fe	Function	Global	User Defined	2	0
HAL_FS_dirDelete	90016b10	Function	Global	User Defined	5	0
HAL_FS_fileAllocate	9001d10a	Function	Global	User Defined	5	0
HAL_FS_fileClose	90016700	Function	Global	User Defined	35	0
HAL_FS_fileCreate	90016538	Function	Global	User Defined	10	0
HAL_FS_fileDelete	900166bc	Function	Global	User Defined	9	0
HAL_FS_fileOpen	900165b6	Function	Global	User Defined	19	0
HAL_FS_fileRead	90016868	Function	Global	User Defined	4	0
HAL_FS_fileReadAt	90016898	Function	Global	User Defined	7	0
HAL_FS_fileWrite	90016946	Function	Global	User Defined	5	0
CONFIG_.md	90016972	Function	Global	User Defined	1	0
CONFIG_ADD_wifi.prefs	90019a7a	Function	Global	User Defined	3	0
CONFIG_RM_wifi.prefs	900197e0	Function	Global	User Defined	3	0


### Screen
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
HAL_SCR_displayPicture_fromBuffer	900162c2	Function	Global	User Defined	34	0

### NFC
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### Others
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
HAL_memset	900012e0	Function	Global	User Defined	98	0
ADC_MGR_Read_BatteryVoltage	90014848	Function	Global	User Defined	10	0
HAL_GetTick	90001952	Function	Global	User Defined	98	0
HAL_enterSleep	90016140	Function	Global	User Defined	13	0



## Resources
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
| BITMAP_ABOUT | `0x9006b9dc` | Data Label | Global | User Defined | 1 |
| BITMAP_BACKLIGHT_1 | `0x9006fcf2` | Data Label | Global | User Defined | 1 |
| BITMAP_BACKLIGHT_2 | `0x90071048` | Data Label | Global | User Defined | 1 |
| BITMAP_BACKLIGHT_3 | `0x90072436` | Data Label | Global | User Defined | 1 |
| BITMAP_BACKLIGHT_4 | `0x9006e78e` | Data Label | Global | User Defined | 1 |
| BITMAP_BACKLIGHT_OK | `0x900738ca` | Data Label | Global | User Defined | 1 |
| BITMAP_BATT_CHARGE | `0x9003d278` | Data Label | Global | User Defined | 2 |
| BITMAP_BATT_CHARGE_1 | `0x9003901e` | Data Label | Global | User Defined | 1 |
| BITMAP_BATT_CHARGE_2 | `0x9003a40c` | Data Label | Global | User Defined | 1 |
| BITMAP_BATT_CHARGE_3 | `0x9003ba2a` | Data Label | Global | User Defined | 1 |
| BITMAP_BATT_CHARGE_4 | `0x9003759e` | Data Label | Global | User Defined | 0 |
| BITMAP_BATT_CHARGE_DONE | `0x9003ea3e` | Data Label | Global | User Defined | 0 |
| BITMAP_BATT_EMTPY | `0x9006c93c` | Data Label | Global | User Defined | 1 |
| BITMAP_CONNECT_PC | `0x90041bf6` | Data Label | Global | User Defined | 3 |
| BITMAP_LOOP | `0x9007d208` | Data Label | Global | User Defined | 1 |
| BITMAP_LOW_BATTERY | `0x900405c4` | Data Label | Global | User Defined | 6 |
| BITMAP_LUNII_1 | `0x9004340c` | Data Label | Global | User Defined | 2 |
| BITMAP_LUNII_2 | `0x9004b16e` | Data Label | Global | User Defined | 1 |
| BITMAP_MODE_DAY | `0x900751f0` | Data Label | Global | User Defined | 2 |
| BITMAP_MODE_NIGHT | `0x900782b2` | Data Label | Global | User Defined | 2 |
| BITMAP_NOSDCARD | `0x900447aa` | Data Label | Global | User Defined | 2 |
| BITMAP_SDERROR | `0x90045e36` | Data Label | Global | User Defined | 0 |
| BITMAP_SDERROR | `0x90072436` | Data Label | Global | User Defined | 0 |
| BITMAP_SETTINGS | `0x9007b74c` | Data Label | Global | User Defined | 1 |
| BITMAP_STORY_ERROR | `0x90047712` | Data Label | Global | User Defined | 1 |
| BITMAP_UNK1 | `0x90066db2` | Data Label | Global | User Defined | 1 |
| BITMAP_USB | `0x90049352` | Data Label | Global | User Defined | 4 |
| BITMAP_VERSION | `0x90076c40` | Data Label | Global | User Defined | 1 |
| BITMAP_WIFI_TXRX | `0x900799ba` | Data Label | Global | User Defined | 1 |


## Raw Table
Data to consider for previous section update
```
```
