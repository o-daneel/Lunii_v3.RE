# Summary
- [Summary](#summary)
- [Firmwares](#firmwares)
  - [Memory layout](#memory-layout)
  - [Bootloader Firmware v3 (TBC)](#bootloader-firmware-v3-tbc)
  - [Device specific](#device-specific)
  - [Main Firmware](#main-firmware)
    - [Mapping](#mapping)
    - [Modules dependencies](#modules-dependencies)
  - [Backup Firmware](#backup-firmware)
- [Test Mode](#test-mode)
  - [How to enable ?](#how-to-enable-)
  - [Behavior](#behavior)
  - [Debug commands](#debug-commands)
    - [Handlers](#handlers)
    - [List](#list)
- [Wifi](#wifi)
  - [Messages](#messages)
    - [LINK\_FAH](#link_fah)
    - [SCAN\_WIFI](#scan_wifi)
    - [LIST\_WIFI](#list_wifi)
    - [Response](#response)
    - [ADD\_WIFI](#add_wifi)
    - [REMOVE\_WIFI](#remove_wifi)
    - [FACTORY\_RESET](#factory_reset)
  - [Responses](#responses)
- [Backend](#backend)
  - [Functions](#functions)
  - [Endpoints](#endpoints)
    - [User Info](#user-info)
    - [Registration](#registration)
    - [Sign In](#sign-in)
    - [Sign Out](#sign-out)
    - [Audio Books](#audio-books)
- [Onboarding](#onboarding)
  - [Process](#process)
- [Security study](#security-study)
  - [Ghidra Project](#ghidra-project)
    - [How to import](#how-to-import)
  - [Thoughts](#thoughts)
    - [Firmware Hash](#firmware-hash)
    - [Custom firmware](#custom-firmware)
    - [NFC chip](#nfc-chip)
    - [Finding SD Ciphering](#finding-sd-ciphering)
- [Crypt-Analysis](#crypt-analysis)
- [Symbols](#symbols)
- [Resources](#resources)
  - [Internal res](#internal-res)
    - [Bitmaps](#bitmaps)
    - [MP3](#mp3)
  - [SD structure \& Files](#sd-structure--files)
  - [Stories Format](#stories-format)
    - [Resources : BMP](#resources--bmp)
    - [Audio : MP3](#audio--mp3)
    - [Indexes](#indexes)
  - [Files Format](#files-format)
    - [version](#version)
    - [cmd](#cmd)
    - [wifi.prefs](#wifiprefs)
    - [.bgt](#bgt)
    - [.md](#md)
    - [.pi](#pi)
    - [.cfg](#cfg)
    - [.nm](#nm)
    - [.content/XXXXYYYY](#contentxxxxyyyy)
    - [.content/XXXXYYYY/bt](#contentxxxxyyyybt)
    - [.content/XXXXYYYY/ni](#contentxxxxyyyyni)
    - [.content/XXXXYYYY/li](#contentxxxxyyyyli)
    - [.content/XXXXYYYY/ri](#contentxxxxyyyyri)
    - [.content/XXXXYYYY/si](#contentxxxxyyyysi)
    - [.content/XXXXYYYY/rf/000/YYYYYYYY](#contentxxxxyyyyrf000yyyyyyyy)
    - [.content/XXXXYYYY/sf/000/YYYYYYYY](#contentxxxxyyyysf000yyyyyyyy)
  - [Story UUIDs](#story-uuids)
    - [Known stories](#known-stories)
    - [Full story database](#full-story-database)
- [Links](#links)


# Firmwares
##  Memory layout
<img src="resources/mem_map.png" width="400">

Three of them are of interest:
1. QuadSPI external flash from `0x80000000_0x9FFFFFFF`   
   2048K are split in two 1024K parts. One for current firmware, and another for backup.
    * 1st 1024 KB pane : Main Firmware in `0x90000000_0x900FFFFF`   
    * 2nd 1024 KB pane : Backup Firmware in `0x900FFFFF_0x901FFFFF`   
2. Internal flash from `0x08000000_0x0800FFFF`
   * 1st part 47 KB: contains Bootloader Firmware from `0x08000000_0x0800BDFF`
   * 2nd part 154 B: contains device specific data from `0x0800BE00_0x0800BE9A`
3. Internal RAM from `0x20000000_0x2003FFFF`  
   For FW 3.1.2, RAM split as :  
    * Zero Initialized `0x2000AF0C -> 0x20038FA4`, size 188568 Bytes (0x2E098)
    * Initialized `0x2000A0D8 -> 0x2000AF0C`, size 3636 Bytes (0xE34)  
      From flash segment `0x9009071C`


## Bootloader Firmware v3 (TBC)

Console is still available on USART4 (PA0/1), on J3 Pin Header

    (22:57:56.490) [BOOTLOADER]Bootloader v3.0
    (22:57:56.515) [BOOTLOADER]GPIO init
    (22:57:56.515) [BOOTLOADER]DMA init
    (22:57:56.515) [BOOTLOADER]ADC3 init
    (22:57:56.515) [BOOTLOADER]SDMMC2 init
    (22:57:56.515) [BOOTLOADER]QUADSPI init
    (22:57:56.515) [BOOTLOADER]Battery level: 3634
    (22:57:56.525) [BOOTLOADER]check_STOP...
    (22:57:56.525) [BOOTLOADER]Check for firmware update file
    (22:57:56.530) [BOOTLOADER]SDMMC2 init
    (22:57:56.540) [BOOTLOADER]FATFS init
    (22:57:56.540) [BOOTLOADER]FATFS mounted
    (22:57:56.540) [BOOTLOADER]found fa.bin: 611328 bytes
    (22:57:59.526) [BOOTLOADER]Unmount FATFS
    (22:57:59.535) [BOOTLOADER]Unmount result: 0
    (22:58:01.270) [BOOTLOADER]Start Lunii firmware...

  
Bootloader process as following:
1. Initialize many peripheral (FPU, RCC, GPIOF/C/A/D, ADC3, UART4, CRC, QSPI)
2. Check that batt level not critical (or abort)
3. Check for Firmware update
   1. only if USB is plugged
   2. preapre SDMMC2
   3. mount sdcard
   4. Try to open `fa.bin`
   5. if exists
      1. decipher the binary
      2. compute Hash and check
      3. if hash ok : erase sectors and write main firmware
      4. close, remove update file, umount SD
4. Read main FW Hash and check it
5. if failed, backup FW
   1. Read backup FW Hash and check it
   2. if Hash ok : erase sectors and write backup firmware to main 
   3. if any error, dead loop
6. Init QPSI (if not done yet)
7. Switch to main FW

This FW contains FatFs (different config) for SD access and performs read/write to QSPI flash though commands   
It also contains storyteller identification data :
* `0x0800BE00_0x0800BE0F` - AES DEVICE KEY  
* `0x0800BE10_0x0800BE1F` - AES DEVICE IV  
* `0x0800BE20_0x0800BE37` - SNU  
* `0x0800BE48_0x0800BE57` - Wifi SSID  
* `0x0800BE58_0x0800BE67` - Wifi Password


## Device specific

This section seems to be personalized in factory through test mode, using CMD_write_metadata.  
It receives a text block (0x130 Bytes long) where everything must be hex text like : "03FA...FFFF"
1. checks the contents, only chars `0-9A-F` and length `0x130`
2. converts all hex text to uint values
3. write them to flash until `0x0800BE98` 

| Address | Length | Usage |
|-:|-|-|
|`0x0800BE00`|0x10|AES Device Key|
|`0x0800BE10`|0x10|AES Device IV|
|`0x0800BE20`|0x18|Unique identifier (SNU)|
|`0x0800BE38`|0x10|TBD|
|`0x0800BE48`|0x10|Wifi AP SSID|
|`0x0800BE58`|0x10|Wifi AP Password|
|`0x0800BE68`|0x30|TBD|
|`0x0800BE98`|||


## Main Firmware
The full firmware ! located at `0x90000000`  
**Version :** 3.1.2   
**Note :** This firmware can be updated

### Mapping

Few interesting offsets :
* `0x90000000` - 0x1E0 Bytes : VectorTable
* `0x90000400` - 4 Bytes : Pointer to firmware End (previously pointer to CRC)

### Modules dependencies
| Module Name | Git URL |
|-|-|
| FreeRTOS | https://github.com/STMicroelectronics/STM32CubeF7.git |
| FatFS | From FreeRTOS (ThridParty/Middleware) |
| coreJSON | https://github.com/FreeRTOS/coreJSON.git |

## Backup Firmware
A short mini firmware ! might be located at `0x90100000`   
**Version :** 3.1.2   
**Objective :** make sure that an USB mass storage is accessible for MainFW reload   
**Note :** This firmware in not expected to be updated, nor updatable

# Test Mode

## How to enable ?
update of NFC chip
## Behavior

## Debug commands

Handled by a shell function here
```
lunii_shell	90013d50	Function	Global	User Defined	1	0
```

This function compares text read (from UART ? TBC), against a table of handlers.

### Handlers
The handlers are located at `9006b7c8  CMD_LIST[0x1C]`

Each CMD_LIST is made of :
* `char * COMMAND`
* `void (* fn)(void)`


### List
- CMD_audio_jack
- CMD_audio_off
- CMD_audio_speaker
- CMD_autotest
- CMD_buttons
- CMD_disp_backlight
- CMD_disp_backlight_apply
- CMD_disp_black
- CMD_disp_mire
- CMD_display_black
- CMD_display_white
- CMD_hardware_version
- CMD_jack_presence
- CMD_mcu_uuid
- CMD_nfc_read
- CMD_nfc_uuid
- CMD_nfc_write
- CMD_onboarding_state
- CMD_reboot
- CMD_sdcard_checksum
- CMD_sdcard_mounted
- CMD_sdcard_version
- CMD_set_rdp
- CMD_software_version
- CMD_vbat
- CMD_wheel
- CMD_wifi
- CMD_write_metadata

# Wifi
**(huge thanks to @ToToL)**

Those commands are to be sent on **Lunii-SNUXXXXXX** hotspot, on server **192.168.4.1:3334**  
TODO : add pictures / ref

## Get Wifi PSK

Wifi PSK is given in the Android APK if the APK can't connect to the WIFI. It
display the message "Se connecter manuellement au WiFi" and give the PSK for
your Lunii. PSK is different for each Lunii.

This Wifi PSK came from
https://server-backend-prod.lunii.com/factory/products/SNUXXXXXX key
json.wap.secret

```JSON 
{
  "createdAt": "2023-08-10T09:11:15.983Z",
  "reference": "LUNII_03_FR_STD",
  "serialNumber": "SNUXXXXXX",
  "updatedAt": "2023-08-10T09:11:15.983Z",
  "version": "v3",
  "wap": {
    "secret" : "mamie929",
    "ssid"    : "Lunii-subSNUXXXXXX"
  }
}
```

## DHCP

It seems that there is a DHCP that give you IP on range 192.168.4.2-254.
TODO : Need to be more tested

## Communicate with Lunii

A simple "telnet 192.168.4.1 3334" is enought to send commands.

Requests are always JSON like:
```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "key1": "value1",
    "key2": "value2",
    ...
  }
}
```

- id: seems to be a uniq ID just to link with the answer
- timestamp: the timestamp :), it seems that old timestamps works
- message: keys/values for the command

Answers are always in two parts:
- First part: a JSON to indicate if the command is OK or not
```JSON 
{
  "source_id": "f3f61d43-8130-11ee-b0ad-cd0c2f4a20b4",
  "timestamp":1699775774,
  "type":0,
  "status":0
}
```

- source_id: the same id as the request
- timestamp: maybe the timestamp of the request. It seems to be identical
- type: 0 for first response, 1 for second response
- status: 0 for OK, -1 for KO

- Second part: a JSON with the returned data of the command, if the command
  return a data
```JSON
{
  "source_id": "f3f61d43-8130-11ee-b0ad-cd0c2f4a20b4",
  "timestamp":1699775774,
  "type":1,
  "status":0,
  "data": the_data
}
```

- source_id: the same id as the request
- timestamp: maybe the timestamp of the request. It seems to be identical
- type: 0 for first response, 1 for second response
- status: 0 for OK, -1 for KO
- data: the data, maybe array [] or dictionnary {}

## Making a fake Lunii

You can write a simple TCP server that correctly respond to commands to
simulate a Lunii for the Android APK

## Messages
Commands are defined a JSON format with those commands supported:

- LINK_FAH  
- SCAN_WIFI
- LIST_WIFI  
- ADD_WIFI  
- REMOVE_WIFI  
- FACTORY_RESET  

### LINK_FAH 

#### Request
 
```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command" : "LINK_FAH",
    "ssid"    : "wifi_ssid",
    "password": "wifi_password",
    "token"   : "base64_encoded_token"
  }
}
```

#### Response

TODO : How to generate this command ?

### SCAN_WIFI

Scan for Wifi the Lunii can see

#### Request

```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command": "SCAN_WIFI"
  }
}
```

#### Response

```JSON 
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": 1699754392,
  "type":0,
  "status":0
}
{
  "source_id":"c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "type":1,
  "status":0,
  "timestamp":1699754392,
  "data": {
    "scan_duration": 2,
    "networks": [
      {
        "ssid": "HOME",
        "bssid": "XX:XX:XX:XX:XX:XX",
        "secure": 4,
        "rssi": -40
      },
      {
        "ssid": "OpenWifi",
        "bssid": "XX:XX:XX:XX:XX:XX",
        "secure": 4,
        "rssi": -40
      }
    ]
  }
}
```

### LIST_WIFI 

Returns the list of configured wifi in the Lunii 

#### Request

```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command": "LIST_WIFI"
  }
}
```

#### Response

```JSON 
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": 1699754392,
  "type":0,
  "status":0
}
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "type": 1,
  "status": 0,
  "timestamp": 1699754392,
  "data": [
    {"bssid":"%s","ssid":"%s"},
    {"bssid":"%s","ssid":"%s"},
    ...
  ]
}
```

### ADD_WIFI

Tries to connect, if success, it adds the network if it was not existing, or it updates the existing one.  

#### Request

```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command": "ADD_WIFI",
    "ssid"    : "wifi_ssid",
    "bssid"    : "wifi_bssid",
    "password": "wifi_password",
  }
}
```

#### Response

```JSON 
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": 1699754392,
  "type":0,
  "status":0
}
```

Then the Lunii connect to the Wifi

### REMOVE_WIFI  
Removes a Wifi conf entry based on a BSSID

#### Request

```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command": "REMOVE_WIFI",
    "bssid"    : "wifi_bssid",
  }
}
```

#### Response

```JSON 
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": 1699754392,
  "type":0,
  "status":0
}
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": 1699754392,
  "type":1,
  "status":0
}
```

### FACTORY_RESET  

#### Request

```JSON 
{
  "id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "message" : {
    "command": "FACTORY_RESET"
  }
}
```

#### Response

```JSON 
{
  "source_id": "c5c96de5-39d6-4133-8301-b33cd1afa5ac",
  "timestamp": "1699754392",
  "type": "0",
  "status": "0",
}
```

# Backend
## Functions
    backend_authtoken_ready
    backend_pairing_device
    backend_signin
    backend_signout
    backend_synchro
    backend_synchronisation_request
    backend_upload_progress

## Endpoints
    https://server-backend-prod.lunii.com/user/devices
    https://server-backend-prod.lunii.com/devices
    https://server-backend-prod.lunii.com/devices/%s/signin
    https://server-backend-prod.lunii.com/devices/%s/signout
    https://server-backend-prod.lunii.com/devices/%s/audiobooks/
    https://server-backend-prod.lunii.com/devices/%s/audiobooks/%s

### User Info
#### Payload
#### Request
```bash
curl -x socks5://localhost:9050 \
  --header 'Authorization: Bearer eyJ0eXA.USER.TOKEN...' \
  --request GET \
  https://server-backend-prod.lunii.com/user/devices
```

#### Response
```JSON
[
  {
    "id": "....................",
    "name": "Fabrique 1",
    "reference": "LUNII - 2 FR RETAIL",
    "version": "v2",
    "packId": "....................",
    "serialNumber": "0020121234567890",
    "serialNumberTypedByUser": false,
    "illustrationUrl": "/public/images/hardwares/icons/v2.png",
    "createdAt": "2003-06-13T18:10:00.189Z",
    "updatedAt": "2004-06-13T18:10:00.189Z",
    "theme": {
        "name": "green",
        "hardware_color_300": "007371",
        "hardware_color_200": "047B79",
        "hardware_color_100": "038987",
        "hardware_color_50": "CEE6E6"
    },
    "metadata": {
        "deviceId": "....................",
        "vendorId": "0x0483",
        "productId": "0xa341",
        "firmwareVersion": "-1.-1_2.22",
        "sdCardSize": 7939817472,
        "createdAt": "1970-01-01T00:00:00.000Z",
        "updatedAt": "1970-01-01T00:00:00.000Z"
    },
    "associationStatus": "DONE"
  },
]
```  

### Registration
Receive token, ciphered with dev key and sent back to server as JSON data

#### Payload
```JSON
{
  "serialNumber" : "2302300012345",
  "pairingToken" : "XXX_base64_endcode_XXX"
}
```  

#### Request

```bash
curl -x socks5://localhost:9050 \
  --header "Content-Type: application/json" \
  --header "User-Agent: FaHv3"\
  --request POST \
  --data '{"serialNumber" : "2302300012345", "pairingToken" : "XXX_base64_endcode_XXX"}' \ 
  https://server-backend-prod.lunii.com/devices/
```
#### Response

### Sign In 
#### Payload
```JSON
{
    "vendorId": "0x0483",
    "productId": "0xa341",
    "firmwareVersion": "3.1.2",
    "sdCardSize": 1024,
    "sdCardFree": 1000,
    "sdCardUsed": 24,
    "batteryLevel": 95,
    "batteryCharging": false,
    "wifiLevel": 5,
    "wifiSsid": "LUNII_AP"
}
```

#### Request

```bash
    curl -x socks5://localhost:9050 \
      --header "Content-Type: application/json" \
      --header "User-Agent: FaHv3"\
      --request POST \
      --data '{"vendorId": "0x0483", "productId": "0xa341", "firmwareVersion": "3.1.2", "sdCardSize": 1024, "sdCardFree": 1000, "sdCardUsed": 24,"batteryLevel": 95, "batteryCharging": false, "wifiLevel": 5, "wifiSsid": "LUNII_AP"}' \
      https://server-backend-prod.lunii.com/devices/23023030012345/signin
```

#### Response
```bash
    {"challenge":"oMjxuoautvnhMYbJDMcPx2nMmO7T1YRMPP_s9kOxf4w","isUpdateAvailable":false}
    # b64 : oMjxuoautvnhMYbJDMcPx2nMmO7T1YRMPP_s9kOxf4w
    # hex : A0C8F1BA86AEB6F9E13186C90CC70FC769CC98EED3D5844C3CFB3D90EC5FE3
    # txt : lunii: ???????
```

### Sign Out
TBF 

### Audio Books
TBF

# Onboarding

## Process

# Security study

## Ghidra Project

You will find Ghidra archive in this repo. These archives contains all the work performed on understanding, renaming, decompiling the Lunii Firmware.

Thanks to the following tools:
* https://securelist.com/how-to-train-your-ghidra/108272/
* https://github.com/leveldown-security/SVD-Loader-Ghidra   
  https://raw.githubusercontent.com/posborne/cmsis-svd/master/data/STMicro/STM32F7x3.svd
* https://github.com/TorgoTorgo/ghidra-findcrypt
* ...
### How to import
1. Open Ghidra
2. Ensure that there is no active project
3. File / Restore Project
4. Pick `...something.../Lunii.RE/ghidra/Lunii_2023_MM_DD.gar` in Archive File
5. **Restore Directory** & **Project Name** must be filled automatically with
   * `...something.../Lunii.RE/ghidra`
   * `Lunii`


## Thoughts

### Firmware Hash
Should be a SHA256  
refer to [MainFW Mapping](#mapping)

### Custom firmware
* build the firmware archive
* to output internal values ?
* to decipher files on uart ?

Firmware archive seems to be named as :  
`boot.bin @08006294 : fa.bin`   
Firmware update seems to be a simple file starting from vectors, ending after expected CRC

#### Boot procedure (TBC)
1. check for USB power source connected   
   Otherwise upgrade process is skipped
2. Looks for `fa.bin` file
3. Check internal Hash
4. Write to Main FW memory segment
5. jump to Main FW


### NFC chip
NFC chip is a simple tag using NDEF standard. A basic 512 byte memory.   

RF Acces level are Read Only. Writting require to present a password that can't be reset nor dumped. Changing values through RF is thus impossible.   

Based on FW analysis, SNU and Version are restored at each boot on Production mode. It means that memory contents a Read&Write allowed through I2C interface.

Using an android application like [NFC Tools](https://play.google.com/store/apps/details?id=com.wakdev.wdnfc), you can dump contents :
  
![](resources/ndef/full_dump.jpg)


If `pi` tag is available, it is copied to SD to `.pi` file. File is overwritten.


### Finding SD Ciphering
There are two functions that performs the same action but from different source :
* `HAL_SCR_displayPicture_fromBuffer` (from a bitmap read in firmware, as identified in this section [Bitmaps](#bitmaps))
* `HAL_SCR_displayPicture_fromFile` (from a file in internal SD card, which are ciphered)

It means that the second handles deciphering, and next displays it (calling the first **HAL_SCR_displayPicture_fromBuffer** or calling same internal functions).

Analysis of `HAL_SCR_displayPicture_fromBuffer`
```
Outgoing References - HAL_SCR_displayPicture_fromBuffer
+- printf_level
+- FUN_9000e334
  +- FUN_9000ecec
  +- bmp_decoder_ReadDataBuff
  +- FUN_9000ed1c
  +- FUN_9000ed4c
+- FUN_900117d8
```

Analysis of `HAL_SCR_displayPicture_fromFile`
```
Outgoing References - HAL_SCR_displayPicture_fromFile
+- printf_level
+- FUN_9000e334
  +- FUN_9000ecec
  +- bmp_decoder_ReadDataBuff
  +- FUN_9000ed1c
  +- FUN_9000ed4c
+- HAL_FS_fileClose
+- sleep_ms
+- FUN_900117d8
```

Same stack.  `bmp_decoder_ReadDataBuff` might adapt based on global variables, and skip f_read part if using flash pointer.    

```
If ciphered, it has to be unciphered.
If written ciphered, it has to be 1. read, 2. unciphered
Checking all f_read() calls

HAL_FS_fileRead -> reads & decipher
HAL_FS_decipher

No opposite operation with write. Most of writtings might be performed by host computer, already ciphered. 
```

# Crypt-Analysis 
[Here](CIPHERING.md)

# Symbols
TODO
<!-- [Here](SYMBOLS.md.md) -->

# Resources

## Internal res

### Bitmaps

| Address | Label | Image |
|-|-|-|
| `0x9006B9DC` | BITMAP_ABOUT | <img src="dump/_v3/res/BITMAP_ABOUT.bmp" width="100"> |
| `0x9006FCF2` | BITMAP_BACKLIGHT_1 | <img src="dump/_v3/res/BITMAP_BACKLIGHT_1.bmp" width="100"> |
| `0x90071048` | BITMAP_BACKLIGHT_2 | <img src="dump/_v3/res/BITMAP_BACKLIGHT_2.bmp" width="100"> |
| `0x90072436` | BITMAP_BACKLIGHT_3 | <img src="dump/_v3/res/BITMAP_BACKLIGHT_3.bmp" width="100"> |
| `0x9006E78E` | BITMAP_BACKLIGHT_4 | <img src="dump/_v3/res/BITMAP_BACKLIGHT_4.bmp" width="100"> |
| `0x900738CA` | BITMAP_BACKLIGHT_OK | <img src="dump/_v3/res/BITMAP_BACKLIGHT_OK.bmp" width="100"> |
| `0x9003D278` | BITMAP_BATT_CHARGE_1 | <img src="dump/_v3/res/BITMAP_BATT_CHARGE_1.bmp" width="100"> |
| `0x9003901E` | BITMAP_BATT_CHARGE_2 | <img src="dump/_v3/res/BITMAP_BATT_CHARGE_2.bmp" width="100"> |
| `0x9003A40C` | BITMAP_BATT_CHARGE_3 | <img src="dump/_v3/res/BITMAP_BATT_CHARGE_3.bmp" width="100"> |
| `0x9003BA2A` | BITMAP_BATT_CHARGE_4 | <img src="dump/_v3/res/BITMAP_BATT_CHARGE_4.bmp" width="100"> |
| `0x9003759E` | BITMAP_BATT_CHARGE_DONE | <img src="dump/_v3/res/BITMAP_BATT_CHARGE_DONE.bmp" width="100"> |
| `0x9003EA3E` | BITMAP_BATT_CHARGE | <img src="dump/_v3/res/BITMAP_BATT_CHARGE.bmp" width="100"> |
| `0x9006C93C` | BITMAP_BATT_EMTPY | <img src="dump/_v3/res/BITMAP_BATT_EMTPY.bmp" width="100"> |
| `0x900684A6` | BITMAP_BLACK | <img src="dump/_v3/res/BITMAP_BLACK.bmp" width="100"> |
| `0x90041BF6` | BITMAP_CONNECT_PC | <img src="dump/_v3/res/BITMAP_CONNECT_PC.bmp" width="100"> |
| `0x900405C4` | BITMAP_LOW_BATTERY | <img src="dump/_v3/res/BITMAP_LOW_BATTERY.bmp" width="100"> |
| `0x9004340C` | BITMAP_LUNII_1 | <img src="dump/_v3/res/BITMAP_LUNII_1.bmp" width="100"> |
| `0x9004B16E` | BITMAP_LUNII_2 | <img src="dump/_v3/res/BITMAP_LUNII_2.bmp" width="100"> |
| `0x90068AC2` | BITMAP_MIRE | <img src="dump/_v3/res/BITMAP_MIRE.bmp" width="100"> |
| `0x900751F0` | BITMAP_MODE_DAY | <img src="dump/_v3/res/BITMAP_MODE_DAY.bmp" width="100"> |
| `0x900782B2` | BITMAP_MODE_NIGHT | <img src="dump/_v3/res/BITMAP_MODE_NIGHT.bmp" width="100"> |
| `0x9006915E` | BITMAP_MODE_TEST | <img src="dump/_v3/res/BITMAP_MODE_TEST.bmp" width="100"> |
| `0x90081804` | BITMAP_NO_WIFI | <img src="dump/_v3/res/BITMAP_NO_WIFI.bmp" width="100"> |
| `0x900447AA` | BITMAP_NOSDCARD | <img src="dump/_v3/res/BITMAP_NOSDCARD.bmp" width="100"> |
| `0x90066DB2` | BITMAP_FACTORY_RESET | <img src="dump/_v3/res/BITMAP_FACTORY_RESET.bmp" width="100"> |
| `0x90045E36` | BITMAP_SDERROR | <img src="dump/_v3/res/BITMAP_SDERROR.bmp" width="100"> |
| `0x9007B74C` | BITMAP_SETTINGS | <img src="dump/_v3/res/BITMAP_SETTINGS.bmp" width="100"> |
| `0x90047712` | BITMAP_STORY_ERROR | <img src="dump/_v3/res/BITMAP_STORY_ERROR.bmp" width="100"> |
| `0x90086DA6` | BITMAP_SYNC_1 | <img src="dump/_v3/res/BITMAP_SYNC_1.bmp" width="100"> |
| `0x90088A78` | BITMAP_SYNC_2 | <img src="dump/_v3/res/BITMAP_SYNC_2.bmp" width="100"> |
| `0x9008A97C` | BITMAP_SYNC_3 | <img src="dump/_v3/res/BITMAP_SYNC_3.bmp" width="100"> |
| `0x9008CC8E` | BITMAP_SYNC_4 | <img src="dump/_v3/res/BITMAP_SYNC_4.bmp" width="100"> |
| `0x900850F6` | BITMAP_SYNC_DONE | <img src="dump/_v3/res/BITMAP_SYNC_DONE.bmp" width="100"> |
| `0x90083242` | BITMAP_SYNC_FAILED | <img src="dump/_v3/res/BITMAP_SYNC_FAILED.bmp" width="100"> |
| `0x9007D208` | BITMAP_SYNC_OK | <img src="dump/_v3/res/BITMAP_SYNC_OK.bmp" width="100"> |
| `0x90049352` | BITMAP_USB | <img src="dump/_v3/res/BITMAP_USB.bmp" width="100"> |
| `0x90076C40` | BITMAP_VERSION | <img src="dump/_v3/res/BITMAP_VERSION.bmp" width="100"> |
| `0x90061838` | BITMAP_WIFI_1 | <img src="dump/_v3/res/BITMAP_WIFI_1.bmp" width="100"> |
| `0x9006349A` | BITMAP_WIFI_2 | <img src="dump/_v3/res/BITMAP_WIFI_2.bmp" width="100"> |
| `0x900650FE` | BITMAP_WIFI_3 | <img src="dump/_v3/res/BITMAP_WIFI_3.bmp" width="100"> |
| `0x9005FB88` | BITMAP_WIFI_DONE | <img src="dump/_v3/res/BITMAP_WIFI_DONE.bmp" width="100"> |
| `0x900799BA` | BITMAP_WIFI_OK | <img src="dump/_v3/res/BITMAP_WIFI_OK.bmp" width="100"> |

### MP3 
| Address | Size | Label | 
|-|-|-|
| 0x900800d2 | 0x0C60 (3168) | [Howl](dump/_v2/mp3/howl.mp3) |
| 0x9007efa6 | 0x1128 (4392) | [Birds](dump/_v2/mp3/birds.mp3) |
| 0x9004ca40 | 0x1147D (70781) | [Pairing](dump/_v3/res/MP3_PAIR.mp3) |

## SD structure & Files 
NOTE : **Ciphered files are only protected on first 0x200 block !**

Keys :
* <u>Generic</u> : stands for Generic Key (common to all sotrytellers)
* <u>Device</u> : stands for Device Key (specific to one specific device)

| File | Key | Contents|
|-|-|-|
|[`sd:0:\.pi`](#pi) | None | Pack Index<br>recreated by main FW |
|[`sd:0:\.md`](#md) | Generic | Metadata<br>(contents from internal flash, two block of 512B, 1st with SNU, 2nd with ciphered data and Key_B)
|[`sd:0:\.cfg`](#cfg) | None | Configuration file |
|[`sd:0:\.nm`](#nm) | None | Night mode enabled if file exists (paramters are loaded from [config file](#cfg))
|`sd:0:\version` | None | contains a simple date      
|[`sd:0:\.content\XXXXXXXX\bt`](#contentxxxxyyyybt) | Device | Authoriaztion file. To validate that this device is authorized to play this story  |
|`sd:0:\.content\XXXXXXXX\li` | Generic | Action Nodes index |
|[`sd:0:\.content\XXXXXXXX\ni`](#contentxxxxyyyyni) | None | Stage Nodes index |
|`sd:0:\.content\XXXXXXXX\nm` | None | Night Mode related |
|[`sd:0:\.content\XXXXXXXX\ri`](#contentxxxxyyyyri) | Generic | Resource Index : Ciphered text file that contains resource list   
|[`sd:0:\.content\XXXXXXXX\si`](#contentxxxxyyyysi) | Generic | Story Index : Ciphered text file that contains story list   
|`sd:0:\.content\XXXXXXXX\rf\` | N/A | Resource Folder 
|[`sd:0:\.content\XXXXXXXX\rf\000\YYYYYYYY`](#contentxxxxyyyyrf000yyyyyyyy) | Generic | Resources (BMP)  
|`sd:0:\.content\XXXXXXXX\sf\` | N/A | Story Folder
|[`sd:0:\.content\XXXXXXXX\sf\000\YYYYYYYY`](#contentxxxxyyyysf000yyyyyyyy) | Generic | Story, audio part and heros names (MP3)

## Stories Format
### Resources : BMP
```
> mediainfo 2C3123BB.bmp
General
Complete name                            : 2C3123BB.bmp
Format                                   : Bitmap
File size                                : 6.52 KiB

Image
Format                                   : RLE4
Width                                    : 320 pixels
Height                                   : 240 pixels
Color space                              : RGB
Bit depth                                : 4 bits
```
### Audio : MP3
```
> mediainfo 0835BAD5.mp3
General
Complete name                            : 0835BAD5.mp3
Format                                   : MPEG Audio
File size                                : 14.6 KiB
Duration                                 : 1 s 880 ms
Overall bit rate mode                    : Variable
Overall bit rate                         : 61.8 kb/s
Writing library                          : LAME3.100

Audio
Format                                   : MPEG Audio
Format version                           : Version 1
Format profile                           : Layer 3
Duration                                 : 1 s 881 ms
Bit rate mode                            : Variable
Bit rate                                 : 61.8 kb/s
Minimum bit rate                         : 32.0 kb/s
Channel(s)                               : 1 channel
Sampling rate                            : 44.1 kHz
Frame rate                               : 38.281 FPS (1152 SPF)
Compression mode                         : Lossy
Stream size                              : 14.2 KiB (97%)
Writing library                          : LAME3.100
Encoding settings                        : -m m -V 4 -q 0 -lowpass 17.5 --vbr-new -b 32
```

### Indexes
Stories are built against index files that allow to create a navigation path through choices, building a custom story.

Indexes can be seen as a table of content.

Indexes files :
1) **ni** : [Node index](#contentxxxxyyyyni)   
   This one is the main story file that list all nodes for the story.   
   Each choice is named a node and is defined by a bitmap, an audio, and a next node.
   A story starts at the first node.
2) **li** : [Jump index](#contentxxxxyyyyli)
3) **ri** : [Resource index](#contentxxxxyyyyri)
4) **si** : [Story index](#contentxxxxyyyysi)

## Files Format
### version
* **Length** : 0x14 (20B)
* **Key** : plain
This is a config file to store a date to define SDcard version. Fixed size of 20 Bytes, no ciphering applied on it.  

``` 
2023-06-14 14:51 UTC
``` 

### cmd
* **Length** : variable
* **Key** : device
This file seems to be a script file to store personnalization step to perform on Lunii.

| Location | Command | Comments |
| - | -: | - |
| `0x9005E521`	| MSC_CMD_SET_ONBOARDING | - |
| `0x9005E53D`	| MSC_CMD_RESET_ONBOARDING | - |
| `0x9005E55B`	| MSC_CMD_CREATE | - |
| `0x9005E56F`	| MSC_CMD_DELETE | - |
| `0x9005E583`	| MSC_CMD_COPY | - |
| `0x9005E595`	| MSC_CMD_MOVE | - |
| `0x9005E5A7`	| MSC_CMD_REBOOT | - |
| `0x9005E5ED`	| MSC_CMD_END | - |

### wifi.prefs
* **Length** : 0x74 * 0xA = 0x488 (1160B)
* **Key** : device (fully ciphered)

This file has the following structure:

``` C
struct wifi_entry {
    BYTE used;
    char SSID[0x21];
    char BSSID[0x12];
    char PWD[0x40];
};

#define MAX_WIFI_ENTRIES    10
#define WIFI_ENTRY_SIZE     0x74

struct wifi_entry WIFI_CONF[WIFI_ENTRY_SIZE];
```

Symboles using 
* CONFIG_LOAD_wifi.prefs
* WIFI_CONF 
* WIFI_addNetConf
* WIFI_getNetConf
* WIFI_udpateNetPwd

### .bgt
* **Length** : 0x01
* **Key** : None


This is a config file to store brightness level. Fixed size of 1 Byte, no ciphering applied on it.  
The brightness must be in range `0-100`

### .md
* **Length** : 0x70 (112B)
* **Key** : plain / generic

Structure:

  `--- First 64B Block --- PLAIN ---`
``` 
0600 (Static)
332E312E3200000000000000000000000000000000000000 : Software version (24 Bytes)
                                                   > 3.1.2 
323031323131313132323333343400000000000000000000 : SNU - Storyteller Unique ID  (24 Bytes)
                                                   > 20121111223344 
0000830441A30000 (Static)
00000001 : Hardware version major
00000010 : Hardware version minor
           > v1.10
``` 

  `--- Second 48B Block --- CIPHERED ---`  
Contains the device key, ciphered with generic key
``` 
TO BE CONFIRMED
v2 :
    31333934XXYYZZ 0700 2600 3EF0112233 : 7Bytes of Unique Dev ID + 2 WORDS + 5 static Bytes
    0000XXYY 60 times (0xF0) : TBD
v3 :
    SNU copied twice and ciphered
    323031323131313132323333343400000000000000000000 : SNU - Storyteller Unique ID  (24 Bytes)
                                                       > 20121111223344 
    323031323131313132323333343400000000000000000000 : SNU - Storyteller Unique ID  (24 Bytes)
                                                       > 20121111223344 
```
### .pi
* **Length** : variable
* **Key** : None

This file is the root files that stores all stories available in device. It contains a simple list of UUID (16 Bytes).
```
C4139D59-872A-4D15-8CF1-76D34CDF38C6
....
123e4567-e89b-12d3-a456-426652340000
```

End of UUIDs (last 8 Bytes) are used to sort stories in subdirectories.

You can retreive more details on existing UUIDs on the dedicated section : [Story UUIDs](#story-uuids)


### .cfg
* **Length** : 0x26
* **Key** : None


This is a config file. Fixed size of 38 Bytes, no ciphering applied on it.   
File is made of 8 tags :   
```
0100
  XXXX YYYY (TAG_00 VALUE_00)
  XXXX YYYY (TAG_01 VALUE_01)
  ...
  XXXX YYYY (TAG_08 VALUE_08)
```
| ID | Tag Len | Value Len | Default Value | Max Value | Role |
|-|-|-|-:|-:|-|
| 0 | WORD | WORD | 300s | 3600s | idle time before sleep mode |
| 1 | WORD | WORD | 60s | 600s | TBD |
| 2 | WORD | WORD | 5s | 10s | Time to display Low battery message |
| 3 | WORD | BYTE | 0 | | Night mode - Enable |
| 4 | WORD | WORD | 0 | | Night mode - Volume level |
| 5 | WORD | WORD | 3 | | Night mode - Stories to play before auto sleep mode |
| 6 | WORD | WORD | ? | | Boolean related to 05<br>If True => (uint)CFG_TAG_04) / (CFG_TAG_05 - 1) |
| 7 | WORD | BYTE | 1 | | TBD |
| 8 | WORD | BYTE | 1 | | Request to recreate `.nm` file |
| 9 | WORD | BYTE | 1 | | related to `uplugged` file |
| 10 | WORD | WORD | ? | | Brightness Level read from `.bgt` |

Constraints can be retreived in the function `LUNII_load_config()`

### .nm
* **Length** : 0
* **Key** : None
This file acts as a boolean to enable or disable Night Mode (more details [here](https://support.lunii.com/hc/fr/articles/4404835664145-Quelles-sont-les-fonctionnalit%C3%A9s-du-Mode-Nuit-))

This mode helps your children to fall asleep by playing automatically many stories, with a decreasing volume, and a limited screen backlight. Three paramaters that might be retrieved in [configuration file](#cfg)

### .content/XXXXYYYY
This is the root directory for a specific story. The name `XXXXYYYY` is based on the lower part of the UUID.

For example, the "Suzanne et Gaston" story :
* UUID : C4139D59-872A-4D15-8CF1-76D3`4CDF38C6`

### .content/XXXXYYYY/bt
* **Length** : 0x40
* **Key** : device specific

This file seems to be the authorization file that is checked to avoid illegal stories copy.

It is made by ciphering the 0x40 first bytes for .ri file with device specific key.

### .content/XXXXYYYY/ni
* **Length** : 0x200 + N*0x2C
* **Key** : None
* **Purpose** : Nodes to navigate in story choices. 
 
This file has the following structure:

``` C
typedef enum <int> { 
    NODE_CHOICE    = 0x10001,
    NODE_INTER     = 0x10000,
} NODETYPE;

// Header is 0x200 long (512 Bytes)
typedef struct {
    int tbd_a[3];
    int index_count;
    int tbd_b[124];
} ni_header;

// Node list is 0x2C * index_count long
typedef struct {
    int image_ri_idx;         // index in ri file
    int audio_si_idx;         // index in si file
    int next_node;            // index loaded from li for next in ni
    int next_node_cnt;        // how many choices for next_node
                              // choices indexes are in range (li(next_node), li(next_node)+next_cnt )
    int tbd_a[4];             //
    NODETYPE type;            // type of node
    NODETYPE tbd;             //
    int tbd_b[1];
} node;

// Node Index file parsing
ni_header header;
node nodes[header.index_count];
```

Story starts at ``nodes[0]``   
You can retreive this management in Ghidra archive
* `story_selection()` @90010004
* `story_node_selection` @9000fda8

### .content/XXXXYYYY/li
* **Length** : N*4
* **Key** : generic
* **Purpose** : Looks to be an indirection table to jump to next node
 
This file is a simple ``int`` table has the following structure:

``` C
// L Index file parsing
int indexes[FileSize()/4];
```


### .content/XXXXYYYY/ri
* **Length** : variable
* **Key** : generic

This file is the Resource Index that stores all resources available for the `XXXXYYYY` story. It is a text plain file (not ciphered).   
The file is organized as a list of ``12 char strings``
```
000\AABBCCDD000\BBCCDDEE...000\CCDDEEFF 
```
### .content/XXXXYYYY/si
* **Length** : variable
* **Key** : generic
 
This file is the Story Index that stores all resources available for the `XXXXYYYY` story. It is a text plain file (not ciphered).   
The file is organized as a list of ``12 char strings``
```
000\AABBCCDD000\BBCCDDEE...000\CCDDEEFF 
```
### .content/XXXXYYYY/rf/000/YYYYYYYY
* **Length** : variable
* **Key** : generic

These files store Resources, meaning images in a Bitmap format   

**FORMAT DETAILS** : [here](#resources--bmp)

### .content/XXXXYYYY/sf/000/YYYYYYYY
* **Length** : variable
* **Key** : generic

These files store Stories, meaning audio in a MP3 format   

**FORMAT DETAILS** : [here](#audio--mp3)

## Story UUIDs
### Known stories
| UUID | Story name | Picture |
|-|-|-|
| C4139D59-872A-4D15-8CF1-76D34CDF38C6 | Suzanne et Gaston | <img src="resources/stories/C4139D59-872A-4D15-8CF1-76D34CDF38C6.bmp" width="100">
| 03933BA4-4FBF-475F-9ECC-35EFB4D11DC9 | Panique aux 6 Royaumes | <img src="resources/stories/03933BA4-4FBF-475F-9ECC-35EFB4D11DC9.bmp" width="100">
| 22137B29-8646-4335-8069-4A4C9A2D7E89 | Au Pays des Loups | <img src="resources/stories/22137B29-8646-4335-8069-4A4C9A2D7E89.bmp" width="100">
| 29264ADF-5A9F-451A-B1EC-2AE21BBA473C | Sur les bancs de l'école | <img src="resources/stories/29264ADF-5A9F-451A-B1EC-2AE21BBA473C.bmp" width="100">
| 2F0F3109-BFAE-4E09-91D7-CA0C2643948D | Le loup dans tous ses états | <img src="resources/stories/2F0F3109-BFAE-4E09-91D7-CA0C2643948D.bmp" width="100">
| 3712AF6D-CF9D-4154-8E98-56821362862A | Pandaroux et les Turbo-Héros | <img src="resources/stories/3712AF6D-CF9D-4154-8E98-56821362862A.bmp" width="100">
| 59A710E9-2F7A-4D0C-AB2D-47E8DD2E29B7 | Pandaroux | <img src="resources/stories/59A710E9-2F7A-4D0C-AB2D-47E8DD2E29B7.bmp" width="100">
| 9C836C24-34C4-4CC1-B9E6-D8646C8D9CF1 | Les Aventures de Zoé -<br> Les 6 Royaumes | <img src="resources/stories/9C836C24-34C4-4CC1-B9E6-D8646C8D9CF1.bmp" width="100">
| 9D9521E5-84AC-4CC8-9B09-8D0AFFB5D68A | Suzanne et Gaston fêtent Pâques | <img src="resources/stories/9D9521E5-84AC-4CC8-9B09-8D0AFFB5D68A.bmp" width="100">
| AA0BC5DD-16FA-4362-859C-0DB158139FE6 | Les bandes a écouter de Yakari | <img src="resources/stories/AA0BC5DD-16FA-4362-859C-0DB158139FE6.bmp" width="100">
| BF573171-5E5D-4A50-BA89-403277175114 | En attendant Noël | <img src="resources/stories/BF573171-5E5D-4A50-BA89-403277175114.bmp" width="100">
| D56A4975-417E-4D04-AEB3-21254058B612 | Oh les pirates ! | <img src="resources/stories/D56A4975-417E-4D04-AEB3-21254058B612.bmp" width="100">
| FB2B7DF4-BE3F-4998-83F0-BFBBDA75B9D7 | A la poursuite des 12 joyaux | <img src="resources/stories/FB2B7DF4-BE3F-4998-83F0-BFBBDA75B9D7.bmp" width="100">
| ... | ... |

### Full story database
The full list of all existing packs can be extracted from this file [packs.json](resources/packs.json)
# Links

- https://www.youtube.com/watch?v=ZeYKieOIsC8
- https://fr.wikipedia.org/wiki/Tiny_Encryption_Algorithm
- https://github.com/coderarjob/tea-c
