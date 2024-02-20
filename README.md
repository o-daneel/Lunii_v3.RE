
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
Many people doesn't care about about software security.   
There, Lunii's company failed in many ways:
1. JTAG is still enabled on the PCB (allows internal flash dump)
2. External Flash is not ciphered (allows dump)
3. UART is enabled, then provides a lot of debug
4. Firmware embeds too many debug strings helping decompiling
5. Firmwares are verified/validated through a dumb CRC (Hash would have been better)

# TL;DR v3
Too long, didn't read ?    
They learnt from previous section ! 😅 but not that much finally ==> **Ciphering defeated.**  
I hope they haven't paid an expensive security expert for a 45days analysis to defeat the firmware 😘  

What they improved :
1. XXTEA is replaced with a well known and recommended AES CBC 👍
2. Two layers of cryptography with diversified keys 👍
3. CRC is replaced with a fucking strong ECDSA signature 👍👍
4. JTAG ReadOut Protection Level 1 🫤

More recommendations to learn for Lunii :
1. You have the binary means you have the sources !
2. JTAG is still enabled !!! 🤦‍♂️ (allows internal flash dump, little bit more complex, but still possible)
3. External Flash is not ciphered (allows dump)
4. UART is enabled, then provides a lot of debug
5. Firmware embeds too many debug strings helping decompiling
6. No anti rollback on FW upgrades
7. Two crypto layers that are not so linked, not tight enough

## Reverse state 📈  

### Bootloader - v3.0
| Code size | in KB |
| - | -: |
| Total FW size  | `47` |
| Identified symbols | `36` |
|  |  |
> **76%** of firmware reversed and decompiled

| Symbols | count |
| - | -: |
| Total Symbols (fn + data) | `603` |
| No yet identified | `119` |
|  |  |
> **80%** of Symbols reversed and decompiled

### Main FW - v3.1.2
| Code size | in KB |
| - | -: |
| Total FW size  | `582` |
| Identified symbols | `510` |
|  |  |
> **87%** of firmware reversed and decompiled

| Symbols | count |
| - | -: |
| Total Symbols (fn + data) | `2478` |
| No yet identified | `481` |
|  |  |
> **80%** of Symbols reversed and decompiled

### Main FW - v3.1.3
| Code size | in KB |
| - | -: |
| Total FW size  | `582` |
| Identified symbols | `31` |
|  |  |
> **5%** of firmware reversed and decompiled

| Symbols | count |
| - | -: |
| Total Symbols (fn + data) | `1939` |
| No yet identified | `1140` |
|  |  |
> **41%** of Symbols reversed and decompiled

### Main FW - v3.1.4
| Code size | in KB |
| - | -: |
| Total FW size  | `592` |
| Identified symbols | `510` |
|  |  |
> **86%** of firmware reversed and decompiled

| Symbols | count |
| - | -: |
| Total Symbols (fn + data) | `2092` |
| No yet identified | `515` |
|  |  |
> **75%** of Symbols reversed and decompiled

# Work in Progress

### TODO v3
* Custom Firmware 🚧
  * New bootloader 
    * without FW signature ✅
    * without RDP level 1 🫣
    * should accept genuine updates and modified ✅ 
  * New MainFW with printf enabled (need an extra to perform looong jump ⚠️)  
    (requires SWD debug working)
  * MainFW is manual switch to test mode

      0800812c 4F  F4  80  61    mov.w      r1,#0x400
      08008130 3C  48           ldr        r0=>Peripherals::GPIOA ,[DAT_08008224 ]           = 40020000h
      08008132 F9  F7  F7  FE    bl         HAL_GPIO_ReadPin                                 GPIO_PinState HAL_GPIO_ReadPin(G
      08008136 78  B1           cbz        r0,LAB_08008158

        GVar3 = HAL_GPIO_ReadPin((GPIO_TypeDef *)&Peripherals::GPIOA,0x400);
        if (GVar3 != GPIO_PIN_RESET) {


  * MainFW with story ciphering detection. Should support both
    * encrypted stories
    * custom stories
* Jailbreak (good old times lovers)
  * Describe procedure v1
  * Identify procedure v2
* Stories
  * Install multiple official stories ✅
  * Decipher them ✅
  * Recipher for another Lunii + validate process ✅
  * Update python application for v3 support
* Ghidra
  * on bootloader FW (expecting dump WIP)
    * ~~analyser la partie stdio avec reent~~
    * ~~comprendre le post hash~~ --> sign verify ecdsa secp256r1
    * ~~trouver la comparaison~~
    * ~~pq un hash 256 = 32B = 8 UINT = 0x02~~
      ~~pq dans le fw on a 0x40 ???~~ --> signature ecdsa
    * ~~simuler le post hash avec un hash genuine (verifier qu'il recalcule correctement)~~
    * ~~recalculer un hash modifié~~ --> impossible sans la clé privée
    * ~~verifier que le blob du fw est bien la signature du sha256 du fw, avec la clé ~~ --> nope ecdsa signature to verify
    * ~~update process~~
    * ~~hash verification : WIP~~
      * ~~define type used~~ : SHA256
      * ~~understand final step~~ --> verify sign
    * ~~generate a modified FW with Hash updated and verified~~ --> impossible
    * ~~generate a modified FW with Hash check on 0 block.~~ --> impossible
  * on mainFW
    * ~~update HW section based on FW~~
    * ~~import symbols from v2 : WIP~~
    * ~~identify update decipher~~
    * ~~identify header decipher~~
    * ~~locate AES Key~~
      * ~~STM32 AES sample code usage (for structs)~~
      * ~~review Lunii AES usage~~
    * ~~Review FreeRTOS usage~~
    * ~~Review FatFS~~
    * ~~Review story management~~
    * ~~Review Backend process~~
    * Review Onboarding (**WIP**)
    * ~~Review Wifi AP server & commands~~
* check XXTEA on new v3 files : **FAILED**
* AES crypto : **confirmed**

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
* [Lunii - Qt Manager](https://github.com/o-daneel/Lunii.Qt)
* [TBD Lunii 1/2](https://www.youtube.com/watch?v=ZeYKieOIsC8&t=9s) - [TBD Lunii 2/2](https://www.youtube.com/watch?v=GItJg34bOk0)
* [LuniiOs](https://github.com/bastien8060/Lumiios) - a python player implementation
* [(Hackday) Tsukuyomi Hacking Lunii](https://hackaday.io/project/167629-tsukuyomi)
* [(GitHub) Tsukuyomi](https://github.com/danksz/tsukuyomi)
* [STUdio - Story Teller Unleashed](https://marian-m12l.github.io/studio-website/)
* [(GitHub) STUdio, Story Teller Unleashed](https://github.com/marian-m12l/studio)
* [(GitHub) STUdio ](https://github.com/marian-m12l/studio/wiki/Documentation)