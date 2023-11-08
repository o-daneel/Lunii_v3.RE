- [TL;DR](#tldr)
  - [Facts](#facts)
  - [Assumptions](#assumptions)
  - [Attack path](#attack-path)
    - [🚧 Dump Analysis](#-dump-analysis)
    - [Flash content update](#flash-content-update)
      - [Changes in "C" firmware](#changes-in-c-firmware)
      - [Changes in "D" firmware](#changes-in-d-firmware)
      - [Changes in "E" firmware](#changes-in-e-firmware)
      - [Changes in "F" firmware](#changes-in-f-firmware)
      - [Changes in "G" firmware (TODO)](#changes-in-g-firmware-todo)
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
1. Swap A to B with same contents
2. Swap A to C,D,E,F,G (more and more modifications)

#### Changes in "C" firmware

* Lunii smilling picture replaced by Brightness Lamp 1/4
* Mp3 pairing required replaced by birds

From
```
   90018fe2 4B  F2  6E  10    movw       r0,#0xb16e
   90018fe6 C9  F2  04  00    movt       r0=>BITMAP_LUNII_2 ,#0x9004
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);

        RTOS_start_UsbTask(1);

   90018ff4 48  F6  4D  01    movw       r1,#0x884d
   90018ff8 4C  F6  40  20    movw       r0,#0xca40
   90018ffc C9  F2  01  01    movt       r1=>RTOS_createAPTask+1 ,#0x9001
   90019000 C9  F2  04  00    movt       r0=>MP3_PAIR ,#0x9004
   90019004 FD  F7  09  FA    bl         play_and_call
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
```

To
```
   90018fe2 4F  F6  F2  40    movw       r0,#0xfcf2
   90018fe6 C9  F2  06  00    movt       r0,#0x9006
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_BACKLIGHT_1);
        
        RTOS_start_UsbTask(1);
        
   90018ff4 48  F6  4D  01    movw       r1,#0x884d
   90018ff8 4E  F6  A6  70    movw       r0,#0xefa6
   90018ffc C9  F2  01  01    movt       r1=>RTOS_createAPTask+1 ,#0x9001
   90019000 C9  F2  07  00    movt       r0,#0x9007
   90019004 FD  F7  09  FA    bl         play_and_call
        play_and_call(&MP3_BIRDS,RTOS_createAPTask + 1);
```

#### Changes in "D" firmware

* Lunii smilling picture replaced by Brightness Lamp 2/4
* Mp3 pairing required replaced by birds
* Simple back and forth to FIRMWARE_EOF

From
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);
        RTOS_start_UsbTask(1);
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
      }
```

To
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_BACKLIGHT_2);
        RTOS_start_UsbTask(1);
        dump_code();
      }
      ...

void dump_code(void) {
   play_and_call(&MP3_BIRDS,RTOS_createAPTask + 1);
   return
}
```

#### Changes in "E" firmware

* Lunii smilling picture replaced by Brightness Lamp 3/4
* Mp3 pairing required replaced by birds
* Jump to FIRMWARE_EOF, Create dump.bin file and close


From
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);
        RTOS_start_UsbTask(1);
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
      }
```

To
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_BACKLIGHT_3);
        RTOS_start_UsbTask(1);
        dump_code();
      }
      ...

void dump_code(void) {
   fp = HAL_FS_fileOpen("sd:0:/dump.bin",FA_OPEN_EXISTING);
   HAL_FS_fileClose(fp);
   play_and_call(&MP3_BIRDS,RTOS_createAPTask + 1);
   return
}
```

#### Changes in "F" firmware

* Lunii smilling picture replaced by Brightness Lamp 4/4
* Mp3 pairing required replaced by birds
* Jump to FIRMWARE_EOF, Create dump.bin file write 0x200 from 0x0800be00, and close


From
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);
        RTOS_start_UsbTask(1);
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
      }
```

To
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_BACKLIGHT_3);
        RTOS_start_UsbTask(1);
        dump_code();
      }
      ...

void dump_code(void)
{
  FIL *fp;
  
  fp = HAL_FS_fileOpen("sd:0:/dump.bin",FA_WRITE|FA_CREATE_ALWAYS);
  HAL_memcpy(fread_buffer,(byte *)AES_DEVICE_KEY,0x200);
  f_write(fp,fread_buffer,0x200,(uint *)&story_buffer);
  HAL_FS_fileClose(fp);
  play_and_call(&MP3_BIRDS,RTOS_createAPTask + 1);
  return;
}
```


#### Changes in "G" firmware (TODO)

* Lunii smilling picture replaced by Brightness Lamp Ok
* Mp3 pairing required replaced by birds
* Jump to FIRMWARE_EOF, Create dump.bin write full internal flash, and close


From
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_LUNII_2);
        RTOS_start_UsbTask(1);
        play_and_call(&MP3_PAIR,RTOS_createAPTask + 1);
      }
```

To
```C
        HAL_SCR_displayPicture_fromBuffer(&BITMAP_BACKLIGHT_3);
        RTOS_start_UsbTask(1);
        dump_code();
      }
      ...

void dump_code(void)
{
  FIL *fp;
  
  fp = HAL_FS_fileOpen("sd:0:/dump.bin",FA_WRITE|FA_CREATE_ALWAYS);
  pos = 0;
  while (pos < 0xFFFF) {
      HAL_memcpy(fread_buffer,(byte *)AES_DEVICE_KEY + pos,0x200);
      f_write(fp,fread_buffer,0x200,(uint *)&story_buffer);
      pos += 0x200;
  }
  HAL_FS_fileClose(fp);
  play_and_call(&MP3_BIRDS,RTOS_createAPTask + 1);
  return;
}
```


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
