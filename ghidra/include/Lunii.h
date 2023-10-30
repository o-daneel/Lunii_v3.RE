typedef unsigned char   undefined;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef enum AUDIO_HP_LEVEL {
    HP_OFF=0,
    HP_ON=1
} AUDIO_HP_LEVEL;

typedef enum AUDIO_STATE {
    AUDIO_PAUSED=0,
    AUDIO_PLAYS=1,
    AUDIO_STOPPED=2
} AUDIO_STATE;

typedef enum BATT_LEVEL {
    BATT_OK=0,
    BATT_LOW=1,
    BATT_CRITICAL=2
} BATT_LEVEL;

typedef struct CMD_LIST CMD_LIST, *PCMD_LIST;

struct CMD_LIST {
    TerminatedCString * input;
    pointer function;
};

typedef enum CRYPTO_MODE {
    KEY_DEVICE=0,
    KEY_GENERIC=1,
    KEY_NONE=2
} CRYPTO_MODE;

typedef enum FATFS_OPEN_MODE { /* File access mode and open method flags (3rd argument of f_open) */
    FA_OPEN_EXISTING=0,
    FA_READ=1,
    FA_WRITE=2,
    FA_CREATE_NEW=4,
    FA_CREATE_ALWAYS=8,
    FA_OPEN_ALWAYS=16,
    FA_OPEN_APPEND=48
} FATFS_OPEN_MODE;

typedef enum msg_level { /* Level to use for printf_level */
    LVL_ERROR=0,
    LVL_RELEASE=2,
    LVL_DEBUG=3
} msg_level;

typedef enum NODETYPE {
    NODE_INTER=65536,
    NODE_CHOICE=65537
} NODETYPE;

typedef struct story_node story_node, *Pstory_node;

struct story_node {
    int image_ri_idx; /* index in ri file */
    int audio_si_idx; /* index in si file */
    int next_node; /* index loaded from li for next in ni */
    int next_node_cnt; /* how many choices for next_node */
    int undef_1;
    int undef_2;
};

typedef enum RTOS_osPriority {
    osPriorityIdle = -3,
    osPriorityLow = -2,
    osPriorityBelowNormal = -1,
    osPriorityNormal = 0,
    osPriorityAboveNormal = +1,
    osPriorityHigh = +2,
    osPriorityRealtime = +3,
    osPriorityError = 0x84
} RTOS_osPriority;

struct RTOS_Task {
    char * name;
    pointer entrypoint;
    // void (entrypoint)();
    RTOS_osPriority priority;
    uint params;
    uint stack_size;
    byte * buffer1;
    byte * buffer2;
};

typedef struct VectorTable VectorTable, *PVectorTable;

struct VectorTable { /* Interrupt and exception vectors */
    undefined4 field0_0x0; /* Reserved */
    pointer Reset; /* Reset */
    pointer NMI; /* Non maskable interrupt. The RCC Clock Security System (CSS) is linked to the NMI vector. */
    pointer HardFault; /* All class of fault */
    pointer MemManage; /* Memory management */
    pointer BusFault; /* Pre-fetch fault, memory access fault */
    pointer UsageFault; /* Undefined instruction or illegal state */
    undefined4 field7_0x1c; /* Reserved */
    undefined4 field8_0x20; /* Reserved */
    undefined4 field9_0x24; /* Reserved */
    undefined4 field10_0x28; /* Reserved */
    pointer SVCall;
    pointer Debug_Monitor;
    undefined4 field13_0x34; /* Reserved */
    pointer PendSV; /* Pendable request for system service */
    pointer SysTick; /* System tick timer */
    pointer WWDG; /* Window Watchdog interrupt */
    pointer PVD; /* PVD through EXTI line detection interrupt */
    pointer TAMP_STAMP; /* Tamper and TimeStamp interrupts through the EXTI line */
    pointer RTC_WKUP; /* RTC Wakeup interrupt through the EXTI line */
    pointer FLASH; /* Flash global interrupt */
    pointer RCC; /* RCC global interrupt */
    pointer EXTI0; /* EXTI Line0 interrupt */
    pointer EXTI1;
    pointer EXTI2;
    pointer EXTI3;
    pointer EXTI4;
    pointer DMA1_Stream0;
    pointer DMA1_Stream1;
    pointer DMA1_Stream2;
    pointer DMA1_Stream3;
    pointer DMA1_Stream4;
    pointer DMA1_Stream5;
    pointer DMA1_Stream6;
    pointer ADC;
    pointer CAN1_TX;
    pointer CAN1_RX0;
    pointer CAN1_RX1;
    pointer CAN1_SCE;
    pointer EXTI9_5;
    pointer TIM1_BRK_TIM9;
    pointer TIM1_UP_TIM10;
    pointer TIM1_TRG_COM_TIM11;
    pointer TIM1_CC;
    pointer TIM2;
    pointer TIM3;
    pointer TIM4;
    pointer I2C1_EV;
    pointer I2C1_ER;
    pointer I2C2_EV;
    pointer I2C2_ER;
    pointer SPI1;
    pointer SPI2;
    pointer USART1;
    pointer USART2;
    pointer USART3;
    pointer EXTI15_10;
    pointer TRC_Alarm;
    pointer OTG_FS_WKUP;
    pointer TIM8_BRK_TIM12;
    pointer TIM8_UP_TIM13;
    pointer TIM8_TRG_COM_TIM14;
    pointer TIM8_CC;
    pointer DMA1_Stream7;
    pointer FSMC;
    pointer SDMMC1;
    pointer TIM5;
    pointer SPI3;
    pointer UART4;
    pointer UART5;
    pointer TIM6_DAC;
    pointer TIM7;
    pointer DMA2_Stream0;
    pointer DMA2_Stream1;
    pointer DMA2_Stream2;
    pointer DMA2_Stream3;
    pointer DMA2_Stream4;
    undefined4 field77_0x134; /* Reserved */
    undefined4 field78_0x138; /* Reserved */
    undefined4 field79_0x13c; /* Reserved */
    undefined4 field80_0x140; /* Reserved */
    undefined4 field81_0x144; /* Reserved */
    undefined4 field82_0x148; /* Reserved */
    pointer OTG_FS;
    pointer DMA2_Stream5;
    pointer DMA2_Stream6;
    pointer DMA2_Stream7;
    pointer USART6;
    pointer I2C3_EV;
    pointer I2C3_ER;
    pointer OTG_HS_EP1_OUT;
    pointer OTG_HS_EP1_IN;
    pointer OTG_HS_WKUP;
    pointer OTG_HS;
    undefined4 field94_0x178; /* Reserved */
    pointer AES;
    pointer RNG;
    pointer FPU;
    pointer USART7;
    pointer USART8;
    pointer SPI4;
    pointer SPI5;
    undefined4 field102_0x198; /* Reserved */
    pointer SAI1;
    undefined4 field104_0x1a0; /* Reserved */
    undefined4 field105_0x1a4; /* Reserved */
    undefined4 field106_0x1a8; /* Reserved */
    pointer SAI2;
    pointer QuadSPI;
    pointer LP_Timer1;
    undefined4 field110_0x1b8; /* Reserved */
    undefined4 field111_0x1bc; /* Reserved */
    undefined4 field112_0x1c0; /* Reserved */
    undefined4 field113_0x1c4; /* Reserved */
    undefined4 field114_0x1c8; /* Reserved */
    undefined4 field115_0x1cc; /* Reserved */
    undefined4 field116_0x1d0; /* Reserved */
    undefined4 field117_0x1d4; /* Reserved */
    undefined4 field118_0x1d8; /* Reserved */
    pointer SDMMC2;
    undefined4 field120_0x1e0; /* Reserved */
};

