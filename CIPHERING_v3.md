- [TL;DR](#tldr)
  - [Facts](#facts)
  - [Assumptions](#assumptions)
  - [Attack path](#attack-path)
    - [🚧 Dump Analysis](#-dump-analysis)
    - [Flash content update](#flash-content-update)
    - [Known plain text attack](#known-plain-text-attack)
- [Keys](#keys)
  - [Generic](#generic)
  - [Device](#device)


# TL;DR
**Ciphering not yet DEFEATED...**  


## Facts
* previous v2 ciphering does no longer applies.
* still only on 512B headers, no more
* AES peripheral used in FW
* .md analysis (new format ?)

## Assumptions 

1. ~~XXTEA configuration has changed~~ ❌
   1. ~~more rounds ==> NOPE (tested from 0 to 9)~~
   2. ~~hardcoded key has been modified ==> TBC with FW dump~~
2. Crypto algorithm has been replaced by an AES
   * Could be CBC and 128b : it is ✅

## Attack path


1. 🚧 Dump Analysis with Ghidra (for an hardcoded key)  
   => looking for both, generic and specific keys 
2. Flash content update to dump keys
3. ST NFC chip replacement for changing mode (prod/test/idle)
4. Performing attack on AES with a Known Plain text  
   => requires a mean to generate as many ciphered as required with specific patterns in plain

### 🚧 Dump Analysis

### Flash content update

### Known plain text attack

# Keys

Resources are deciphered through callbacks:
* BMP : used with HAL_SCR_displayPicture_fromFile
* MP3 : used in HAL_AUDIO_play, callback MP3_DEC_cb_read_plain

## Generic
Applies to all the devices

Applies to:
* Sync token
* .md file
* bitmaps
* mp3s

Functions:
* HAL_CRYP_KeyDev_Decrypt
* HAL_CRYP_KeyDev_Encrypt
  
## Device
Applies to a specific device
