- [TL;DR](#tldr)
  - [Open questions](#open-questions)
  - [Facts](#facts)
  - [Assumptions](#assumptions)
  - [Attack path](#attack-path)
    - [🚧 Dump Analysis](#-dump-analysis)
    - [❌ Flash content update](#-flash-content-update)
    - [✅ Flash Hot Swap](#-flash-hot-swap)
    - [✅ JTAG RAM Dump](#-jtag-ram-dump)
    - [❌ Known plain text attack 💀 (NOPE)](#-known-plain-text-attack--nope)
    - [Exploit](#exploit)
    - [Backend auth token](#backend-auth-token)
- [Keys (x3)](#keys-x3)
    - [~~Assumption~~ \> Facts](#assumption--facts)
  - [Device Key](#device-key)
  - [Story Key](#story-key)
  - [Firmware Signature](#firmware-signature)


# TL;DR

**Device Ciphering PARTIALLY defeated...**  
* 2 out of N defeated  

**Story Ciphering PARTIALLY defeated...**  
* N defeated (depends on Device Ciphering)

No need to go further: 

>**> Ciphering Defeated 😎**  
>Being able to extract the story keyset on a specific Lunii, for a given story "A", offers the capability to cipher all custom stories using this specific key of Story "A".  
Any Lunii owner that officially owns Story A, has the corresponding **bt** file (refer to this [section](ANALYSIS_v3.md#contentxxxxyyyybt)). So the same **bt** file can be reused for custom stories to have a correct cipehring of custom story key without having to know the internal device keys... 🤟  
**All greatings to Dantsu for the idea 👏**

## Open questions
1. Are device key & iv related between two luniis ?  
   (my guess is : **NO**, that would be a huge flaw)
2. What is the 0x10 bytes long data undefined in perso area ?
   * not ciphered with device key

## Facts
* previous v2 ciphering does no longer applies.
* still only on 512B headers, no more
* AES peripheral used in FW
* .md new format
  
1. STM32 ReadOut Protection is set at Level 1  
   (still allows some memory access, but not all)
2. Crypto algorithm has been replaced by an AES
   * AES CBC and 128b  ✅
3. CRC on firmware upgrade replaced
   * CRC replaced by a SHA256 ✅  
     SHA256 is to easy to tamper !
   * ECDSA signature to verify for upgrade validation 😢
5. Is FW signature the same for all luniis ? **YES**  
   (common EC key ?)  
   Need to defeat another lunii (comming soon)
6. Is story key same for all stories ? **NOPE** unique per story  
   (I guess it is not)


## Assumptions 

1. ~~XXTEA configuration has changed~~ ❌
   1. ~~more rounds ==> NOPE (tested from 0 to 9)~~
   2. ~~hardcoded key has been modified ==> TBC with FW dump~~

## Attack path

1. 🚧 Dump Analysis with Ghidra (for an hardcoded key)  
   => looking for both, generic and specific keys 
2. ❌ Flash content update to dump keys
3. ✅ Flash hot swap
4. ✅ JTAG RAM Dump
5. Exploit
6. ST NFC chip replacement for changing mode (prod/test/idle)

### 🚧 Dump Analysis
**DIFFICULTY** : `EASY`  
**REQUIRES** : materials that Lunii company might have left or incorrectly secured  
**RESULTS** : None, everything is correctly  

### ❌ Flash content update
**DIFFICULTY** : `IMPOSSIBLE`  
**REQUIRES** : firmware with a correct signature  

### ✅ Flash Hot Swap
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

### ✅ JTAG RAM Dump
**DIFFICULTY** : `MEDIUM / EASY`  
**REQUIRES** : soldering skills  
**RESULTS** : Works and validated, stories have been deciphered using this method

#### HowTo

1. Solder the SWD connector on the right like [here](HARDWARE_v3.md#jtag--swd)  
   *Pins from top to bottom*
   | Pin | Comment |  
   |-|-|
   |SWDIO| To be connected |
   |GND| To be connected |
   |SWCLK| To be connected |
   |3v3| DO NOT CONNECT |
   |NRST| To be connected |
2. Install STLink Utility ([here](./tools/en.stsw-link004.zip)), or download it from STMicro website (requires account creation)
   
##### Extract device keys
3. Connect your ST Link v2 to your PC (USB device must be detected and recognized)
4. Connect Lunii USB C to PC, in order to start onto mass storage directly
5. PowerUp Lunii
6. Update Address and Size to (depending on your fw version)
   *  **v3.1.2** : `0x20018650` & `0x20`, press ENTER  
   *  **v3.1.3** : `0x20018584` & `0x20`, press ENTER  
7. Connect to target
8. Save to a file. You have your Device IV & Key
9. Disconnect from STLink and power off the Lunii

##### Extract story keys
(this method allows to get story key without having to decipher bt file externally)

Repeat all previous steps from [previous section](#extract-device-keys).  

3. Connect your ST Link v2 to your PC (USB device must be detected and recognized)
4. PowerUp your Lunii
5. Turn the wheel to select the story
6. Update Address and Size to (depending on your fw version)
   *  **v3.1.2** : `0x20018650` & `0x20`, press ENTER  
   *  **v3.1.3** : `0x20018584` & `0x20`, press ENTER  
7. Connect to target
8. Save to a file. You have your Story IV & Key
9. Disconnect from STLink and power off the Lunii

### ❌ Known plain text attack 💀 (NOPE)
**DIFFICULTY** : `IMPOSSIBLE`

AES is strong, and key can't be retrieved with such attack  
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
* ✅ Story  Key + IV (AES 128)
* 💀 Update Signature (ECDSA secp256r1) - only public key

### ~~Assumption~~ > Facts
Lunii storyteller has a device specific key. All device files are ciphered with this key, including updates.  
Resources are ciphered with a story dedicated key that is present in **bt** file. The later is ciphered with device key.  
Reading a story requires to decipher **bt** file to load generic (key,iv) and process story.
(Internal flash FW dump is required to confirm)

## Device Key

Applies to:
* Firmware upgrade
* Backend challenges for pairing & signin 
* wifi.prefs

Functions:
* `HAL_CRYP_KeyDev_Decrypt()`
* `HAL_CRYP_KeyDev_Encrypt()`
  
Applies to a specific device

## Story Key
One per story, stored in **bt** file.

Functions:
* `HAL_CRYP_loadStoryKey()`
 
## Firmware Signature

Made of ECDSA signature that can be verified against 

> Public key (**unique for all Luniis**)

      23 8A 11 E0 54 D8 87 3D 43 96 9A E7 AF 87 83 FE
      14 97 D3 52 CA 2F 04 41 73 71 76 C0 30 1D AE 82 
      A8 9F 8B 13 94 80 91 DD A3 B6 0D 46 EB E6 0F 44 
      0F 85 60 A6 E7 92 8E 31 19 ED DC 37 77 49 52 13

Same firmware for two different storytellers share the same signature.  

> Signature for Firmware - v3.1.2  

      06 12 F2 AD F7 A7 03 29 26 C1 19 66 AE 88 0C D0
      54 9E DC 51 FA 8F 80 41 2B C6 CD 17 A1 8B 8B DC
      47 33 27 93 DE 31 61 22 61 E0 FC A3 97 90 FD 8B
      B3 B8 37 19 4F 4B 71 EA 4E EE AD 9F D1 0D 0B 63

> Signature for Firmware - v3.1.3  

      65 F7 CF 56 A8 AF F8 34 75 96 44 2F DA 61 4F 8C
      C2 8E 44 41 C9 65 62 57 14 03 D4 64 01 8D A7 F0
      A6 80 A6 34 86 14 2A 2A 2A F2 2C B4 70 D4 71 0E
      09 32 DE CC 4E C3 A9 74 A2 F5 EA CA D9 36 80 25

**NOTE** :   
Thanks to *public* key, we can only `verify()` the signature. Computing it, `sign()` requires the *private* key