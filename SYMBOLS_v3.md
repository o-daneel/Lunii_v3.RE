
# Symbols

**!! Work In Progress !!**
WIP

## HALs

### FatFs
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
lock_volume
unlock_volume
disk_status
validate
f_lseek
create_chain
get_fat
put_fat
move_window
sync_window
dir_clear
disk_write
ld_word
st_word
clst2sect
ld_clust
st_clust



### Audio
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### Screen
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### NFC
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### Others
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

## Core

VectorTable

### Vector Tables
One for each firmware:
* boot - `0x0800 0000`
* main - `0x9000 0000`
* backup - `0x8000 0000`

See chapter [9.1.2 Interrupt and exception vectors ](docs/rm0431-stm32f72xxx-and-stm32f73xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |



### Low-Level
| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |

### OS & Lib C

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |


### Others


| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |



## Test mode

| Name | Location | Type | Namespace | Source | Ref count |
| --- | --- | --- | :---: | --- | ---: |
| CMD_write_key_uuid | `0x90013aa8` | Function | Global | User Defined | 0 |


## Raw Table
Data to consider for previous section update
```
```
