- [TL;DR](#tldr)
  - [Facts](#facts)
  - [Assumptions](#assumptions)


# TL;DR
**Ciphering not yet DEFEATED...**  


## Facts
* previous v2 ciphering does no longer applies.
* still only on 512B headers, no more
* AES peripheral used in FW

## Assumptions 

1. XXTEA configuration has changed
   1. more rounds ==> NOPE (tested from 0 to 9)
   2. hardcoded key has been modified ==> TBC with FW dump
2. Crypto algorithm has been replaced by an AES
   * Could be CBC and 128b