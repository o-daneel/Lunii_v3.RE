
![](resources/StoryTeller.avif)
Lunii is a French company that made an interactive box for kids to customize stories. Lunii made our children loving stories, a real helper for them to fall asleep.  
 **If you like it, BUY IT !!!  
Even if you don't, children will 😁**

# Sections

* Lunii v2
   1. [Hardware](HARDWARE_v2.md)
   2. [Firmware analysis](ANALYSIS_v2.md)
   3. [Ciphering reverse](CIPHERING_v2.md)
* Lunii v3
   1. [Hardware](HARDWARE_v3.md)
   2. [Analysis](ANALYSIS_v3.md)
   3. [Ciphering reverse](CIPHERING_v3.md)


# TL;DR v2
Too long, didn't read ?    
Many people doesn't care about about software security. There Lunii's company failed in many ways:
1. JTAG is still enabled on the PCB (allows internal flash dump)
2. External Flash is not ciphered (allows dump)
3. UART is enabled, then provides a lot of debug
4. Firmware embeds too many debug strings helping decompiling
5. Firmwares are verified/validated through a dumb CRC (Hash would have been better)

# TL;DR v3
Too long, didn't read ?    
They learnt from previous section ! 😅
Work in progress to extract the root key that seems to be the **device key**.

## Reverse state 📈  

| Code size | in KB |
| - | -: |
| Total FW size  | `582` |
| Identified symbols | `431` |
|  |  |
> **74%** of firmware reversed and decompiled

| Symbols | count |
| - | -: |
| Total Symbols (fn + data) | `2344` |
| No yet identified | `831` |
|  |  |
> **65%** of Symbols reversed and decompiled


# Work in Progress

### TODO v3
* Ghidra on FW dump
  * update HW section based on FW
  * import symbols from v2 : WIP
  * identify update decipher
  * identify header decipher
  * locate AES Key
    * STM32 AES sample code usage (for structs)
    * review Lunii AES usage
* AES crypto : **confirmed**
* check XXTEA on new v3 files : **FAILED**

### TODO v2
* Describe test mode / Try it
* Deep dive in file section to understand format
  * .nm : Night mode (to be tested)
* Decompile 
  * Main FW : in progress > [Main Firmware](ANALYSIS.md#main-firmware) 
* Firmware management
  * Try loading firmware update
  * Create custom firmware (simple internal picture update)
  * Restore original FW
  * Insert dummy patch (just back and forth) + try it (using custom picture)
  * Make less dummy patch with printf call (can't be read without UART acces)

### DONE
* Deep dive in file section to understand format
  * .cfg : **DONE**
  * .pi : **DONE**
  * /rf/ & .ri  : **DONE**
  * /sf/ & .si : **DONE**
  * .ni : **DONE**
  * .li : **DONE**
  * .bt **DONE**
* Undelete on storyteller ? **DONE** > only 50 mp3 files, none french stories removed.
* sample code to process TEA cipher/decipher
  * in C or python ? : **DONE**
  * Try it on Key_A ciphered files : **DONE**
* How to extract Key_B ? **DONE**
* Decompile 
  * Boot FW : **DONE** > [Bootloader Firmware](ANALYSIS.md#bootloader-firmware) 
  * Backup FW : partial but **CLOSED**, no worth > [Backup Firmware](ANALYSIS.md#backup-firmware) 

### FAILED / ABORTED
* NFC chip
  * write NDEF using a dummy card (to test) with Android NXP Write
  * update storyteller to switch to "test"
  * back to production
  * investigate test mode
* Firmware management
  * Make patch to write to SD :
    1.  A dummy file
    2.  File with SNU + DATA
    3.  File with KeyA & KeyB in plain
  
# Links / Similar repos
* [Lunii - Pack Manager CLI](https://github.com/o-daneel/Lunii.PACKS)
* [TBD Lunii 1/2](https://www.youtube.com/watch?v=ZeYKieOIsC8&t=9s) - [TBD Lunii 2/2](https://www.youtube.com/watch?v=GItJg34bOk0)
* [LuniiOs](https://github.com/bastien8060/Lumiios) - a python player implementation
* [linux-cli](https://github.com/Wameuh/lunii-cli) (Did the same reverse analysis 😥 in July 2022, i'm late)
* [(Hackday) Tsukuyomi Hacking Lunii](https://hackaday.io/project/167629-tsukuyomi)
* [(GitHub) Tsukuyomi](https://github.com/danksz/tsukuyomi)
* [STUdio - Story Teller Unleashed](https://marian-m12l.github.io/studio-website/)
* [(GitHub) STUdio, Story Teller Unleashed](https://github.com/marian-m12l/studio)
* [(GitHub) STUdio ](https://github.com/marian-m12l/studio/wiki/Documentation)