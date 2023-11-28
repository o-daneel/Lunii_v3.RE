- [TL;DR](#tldr)
  - [Facts](#facts)
  - [Assumptions](#assumptions)
  - [Attack path](#attack-path)
    - [🚧 Dump Analysis](#-dump-analysis)
    - [Flash content update](#flash-content-update)
    - [Flash Hot Swap](#flash-hot-swap)
    - [Known plain text attack 💀 (NOPE)](#known-plain-text-attack--nope)
    - [Exploit](#exploit)
    - [Backend auth token](#backend-auth-token)
- [Keys (x3)](#keys-x3)
    - [Assumption](#assumption)
  - [Device](#device)
  - [Story](#story)
  - [Firmware Signature](#firmware-signature)


# TL;DR
**Ciphering not yet DEFEATED...**  


## Facts
* previous v2 ciphering does no longer applies.
* still only on 512B headers, no more
* AES peripheral used in FW
* .md new format
  
1. STM32 ReadOut Protection is set at Level 1
2. Crypto algorithm has been replaced by an AES
   * AES CBC and 128b  ✅
3. CRC on firmware upgrade replaced
   * CRC replaced by a SHA256 ✅  
     SHA256 is to easy to tamper !
   * ECDSA signature to verify for upgrade validation 😢

## Assumptions 

1. ~~XXTEA configuration has changed~~ ❌
   1. ~~more rounds ==> NOPE (tested from 0 to 9)~~
   2. ~~hardcoded key has been modified ==> TBC with FW dump~~

## Attack path

1. 🚧 Dump Analysis with Ghidra (for an hardcoded key)  
   => looking for both, generic and specific keys 
2. Flash content update to dump keys
3. Flash hot swap
4. Exploit
5. ST NFC chip replacement for changing mode (prod/test/idle)

### 🚧 Dump Analysis
**DIFFICULTY** : `EASY`  
**REQUIRES** : materials that Lunii company might have left or incorrectly secured  
**RESULTS** : None, everything is correctly  

### Flash content update
**DIFFICULTY** : `IMPOSSIBLE`  
**REQUIRES** : firmware with a correct signature  

### Flash Hot Swap
**DIFFICULTY** : `MEDIUM`  
**REQUIRES** : hardware / electronic skills  
**RESULTS** : Works and validated, first dump done with this method

Empty fresh Lunii v3 start with requesting to pair
```C
event_loop() {
   ...
      TBC_process_cmd();
      LUNII_load_config();
      res = check_PCROP_active();
      if (res == 0) {
        USB_setCB_setup(USB_CB_setup + 1);
        USB_setCB_unplugged(USB_CB_unplugged + 1);
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);
        RTOS_start_UsbTask(1);
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
      }
      else {
        RTOS_createTask_FromID(0,0);
      }
   ...
}
```

(flash A is the Genuine)  
1. Swap A to B

#### Changes in "B" firmware

* Lunii smilling picture replaced by Brightness Lamp 1/4
* Mp3 pairing required replaced by birds
* Full inernal flash is dumped in .md file instead of orignal contents (refer to DumpBootloader in ghidra project)

```C
  else {
    HAL_FS_fileSeekWrite(fp,&FLASH_INT_0800,0,0xffff,&bytes_read_written);
    HAL_FS_fileClose(fp);
  }
```

### Known plain text attack 💀 (NOPE)
**DIFFICULTY** : `IMPOSSIBLE`

AES is strong, and key can't be retrieve with such attack
~~Using Lunii Wifi to set up a custom network config, for lunii storyteller to update wifi.prefs file.
Iterating through many wifi.prefs file generation with specific pattern injected in network conf might allow a known text attack. 
To be investigated.  
Using WIFI command LINK_FAH (Thanks to @Totol)~~

### Exploit
**DIFFICULTY** : `HARD`  
**REQUIRES** : Overflow in MainFW, missing check on a size

Idea is to get an overflow to inject arbitrary code, ideally a small function that outputs the keys in a file on µSD  

### Backend auth token
TBC


# Keys (x3)

* ✅ Device Key + IV (AES 128) 
* .......  Story Key + IV (AES 128)
* 💀 Update Signature (ECDSA secp256r1) - only public key

### Assumption
Lunii storyteller has a device specific key. All device files are ciphered with this key, including updates.
Resources are ciphered with a generic key that is present in **bt** file. The later is ciphered with device key.  
Reading a story requires to decipher **bt** file to load generic key and process stories.
(Internal flash FW dump is required to confirm)

## Device

Applies to:
* Firmware upgrade
* Backend chalenges for pairing & signin 

Functions:
* HAL_CRYP_KeyDev_Decrypt
* HAL_CRYP_KeyDev_Encrypt
  
Applies to a specific device

## Story

## Firmware Signature