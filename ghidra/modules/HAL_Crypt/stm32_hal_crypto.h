/** 
  * @brief  HAL Status structures definition  
  */  
typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum
{
  HAL_CRYP_STATE_RESET             = 0x00U,  /*!< CRYP not yet initialized or disabled  */
  HAL_CRYP_STATE_READY             = 0x01U,  /*!< CRYP initialized and ready for use    */
  HAL_CRYP_STATE_BUSY              = 0x02U  /*!< CRYP BUSY, internal processing is ongoing  */
} HAL_CRYP_STATETypeDef;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;

typedef enum 
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U  
} HAL_LockTypeDef;

typedef enum
{
  HAL_CRYP_STATE_RESET             = 0x00U,  /*!< CRYP not yet initialized or disabled  */
  HAL_CRYP_STATE_READY             = 0x01U,  /*!< CRYP initialized and ready for use    */
  HAL_CRYP_STATE_BUSY              = 0x02U  /*!< CRYP BUSY, internal processing is ongoing  */
} HAL_CRYP_STATETypeDef;

#define HAL_CRYP_ERROR_NONE              0x00000000U  /*!< No error        */
#define HAL_CRYP_ERROR_WRITE             0x00000001U  /*!< Write error     */
#define HAL_CRYP_ERROR_READ              0x00000002U  /*!< Read error      */
#define HAL_CRYP_ERROR_DMA               0x00000004U  /*!< DMA error       */
#define HAL_CRYP_ERROR_BUSY              0x00000008U  /*!< Busy flag error */
#define HAL_CRYP_ERROR_TIMEOUT           0x00000010U  /*!< Timeout error */
#define HAL_CRYP_ERROR_NOT_SUPPORTED     0x00000020U  /*!< Not supported mode */
#define HAL_CRYP_ERROR_AUTH_TAG_SEQUENCE 0x00000040U  /*!< Sequence are not respected only for GCM or CCM */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)
#define  HAL_CRYP_ERROR_INVALID_CALLBACK ((uint32_t)0x00000080U)    /*!< Invalid Callback error  */
#endif

typedef enum
{
  ERROR_NONE              = 0x00000000U,  /*!< No error        */
  ERROR_WRITE             = 0x00000001U,  /*!< Write error     */
  ERROR_READ              = 0x00000002U,  /*!< Read error      */
  ERROR_DMA               = 0x00000004U,  /*!< DMA error       */
  ERROR_BUSY              = 0x00000008U,  /*!< Busy flag error */
  ERROR_TIMEOUT           = 0x00000010U,  /*!< Timeout error */
  ERROR_NOT_SUPPORTED     = 0x00000020U,  /*!< Not supported mode */
  ERROR_AUTH_TAG_SEQUENCE = 0x00000040U,  /*!< Sequence are not respected only for GCM or CCM */
} HAL_CRYP_ERRORTypeDef;

#define CRYP_DATAWIDTHUNIT_WORD   0x00000000U  /*!< By default, size unit is word */
#define CRYP_DATAWIDTHUNIT_BYTE   0x00000001U  /*!< By default, size unit is word */

#define CRYP_TIMEOUT_KEYPREPARATION      82U         /*The latency of key preparation operation is 82 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMINITPHASE     299U        /*  The latency of  GCM/CCM init phase to prepare hash subkey is 299 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMHEADERPHASE   290U        /*  The latency of  GCM/CCM header phase is 290 clock cycles.*/

#define  CRYP_PHASE_READY                0x00000001U /*!< CRYP peripheral is ready for initialization. */
#define  CRYP_PHASE_PROCESS              0x00000002U /*!< CRYP peripheral is in processing phase */

#if defined(CRYP)

#define CRYP_DES_ECB     CRYP_CR_ALGOMODE_DES_ECB
#define CRYP_DES_CBC     CRYP_CR_ALGOMODE_DES_CBC
#define CRYP_TDES_ECB    CRYP_CR_ALGOMODE_TDES_ECB
#define CRYP_TDES_CBC    CRYP_CR_ALGOMODE_TDES_CBC
#define CRYP_AES_ECB     CRYP_CR_ALGOMODE_AES_ECB
#define CRYP_AES_CBC     CRYP_CR_ALGOMODE_AES_CBC
#define CRYP_AES_CTR     CRYP_CR_ALGOMODE_AES_CTR

#define CRYP_AES_GCM     CRYP_CR_ALGOMODE_AES_GCM
#define CRYP_AES_CCM     CRYP_CR_ALGOMODE_AES_CCM

#else /* AES*/
#define CRYP_AES_ECB            0x00000000U                       /*!< Electronic codebook chaining algorithm                   */
#define CRYP_AES_CBC            AES_CR_CHMOD_0                    /*!< Cipher block chaining algorithm                          */
#define CRYP_AES_CTR            AES_CR_CHMOD_1                    /*!< Counter mode chaining algorithm                          */
#define CRYP_AES_GCM_GMAC       (AES_CR_CHMOD_0 | AES_CR_CHMOD_1) /*!< Galois counter mode - Galois message authentication code */
#define CRYP_AES_CCM            AES_CR_CHMOD_2                    /*!< Counter with Cipher Mode                                 */
#endif /* End AES or CRYP */


typedef enum {
  ALGO_AES_ECB = 0x00000000U,
  ALGO_AES_CBC = AES_CR_CHMOD_0,
  ALGO_AES_CTR = AES_CR_CHMOD_1,
  ALGO_AES_GCM_GMAC = (AES_CR_CHMOD_0 | AES_CR_CHMOD_1),
  ALGO_AES_CCM = AES_CR_CHMOD_2,
} HAL_CRYP_AESTypeDef;

#if defined(CRYP)
#define CRYP_KEYSIZE_128B         0x00000000U
#define CRYP_KEYSIZE_192B         CRYP_CR_KEYSIZE_0
#define CRYP_KEYSIZE_256B         CRYP_CR_KEYSIZE_1
#else /* AES*/
#define CRYP_KEYSIZE_128B         0x00000000U          /*!< 128-bit long key */
#define CRYP_KEYSIZE_256B         AES_CR_KEYSIZE       /*!< 256-bit long key */
#endif /* End AES or CRYP */

#if defined(CRYP)
#define CRYP_DATATYPE_32B         0x00000000U
#define CRYP_DATATYPE_16B         CRYP_CR_DATATYPE_0
#define CRYP_DATATYPE_8B          CRYP_CR_DATATYPE_1
#define CRYP_DATATYPE_1B          CRYP_CR_DATATYPE
#else /* AES*/
#define CRYP_DATATYPE_32B         0x00000000U  /*!< 32-bit data type (no swapping)        */
#define CRYP_DATATYPE_16B         AES_CR_DATATYPE_0       /*!< 16-bit data type (half-word swapping) */
#define CRYP_DATATYPE_8B          AES_CR_DATATYPE_1       /*!< 8-bit data type (byte swapping)       */
#define CRYP_DATATYPE_1B          AES_CR_DATATYPE         /*!< 1-bit data type (bit swapping)        */
#endif /* End AES or CRYP */

#if !defined(AES)
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U     /*!< Encryption mode(Mode 1)  */
#define CRYP_OPERATINGMODE_KEYDERIVATION             AES_CR_MODE_0   /*!< Key derivation mode  only used when performing ECB and CBC decryptions (Mode 2) */
#define CRYP_OPERATINGMODE_DECRYPT                   AES_CR_MODE_1   /*!< Decryption    (Mode 3)    */
#define CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT     AES_CR_MODE     /*!< Key derivation and decryption only used when performing ECB and CBC decryptions (Mode 4) */
#define CRYP_PHASE_INIT                              0x00000000U        /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            AES_CR_GCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           AES_CR_GCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             AES_CR_GCMPH       /*!< GCM/GMAC or CCM  final phase */
#else  /* CRYP */
#define CRYP_PHASE_INIT                              0x00000000U             /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            CRYP_CR_GCM_CCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           CRYP_CR_GCM_CCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             CRYP_CR_GCM_CCMPH       /*!< GCM/GMAC or CCM  final phase */
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U             /*!< Encryption mode   */
#define CRYP_OPERATINGMODE_DECRYPT                   CRYP_CR_ALGODIR         /*!< Decryption        */
#endif /* End CRYP or  AES */


typedef enum {
  AES_PHASE_INIT            = 0x00000000U,             /*!< GCM/GMAC (or CCM) init phase */
  AES_PHASE_HEADER          = CRYP_CR_GCM_CCMPH_0,     /*!< GCM/GMAC or CCM header phase */
  AES_PHASE_PAYLOAD         = CRYP_CR_GCM_CCMPH_1,     /*!< GCM(/CCM) payload phase      */
  AES_PHASE_FINAL           = CRYP_CR_GCM_CCMPH,       /*!< GCM/GMAC or CCM  final phase */
  AES_OPERATINGMODE_ENCRYPT = 0x00000000U,             /*!< Encryption mode   */
  AES_OPERATINGMODE_DECRYPT = CRYP_CR_ALGODIR,         /*!< Decryption        */
} HAL_CRYP_PHASETypeDef;

/*  CTR1 information to use in CCM algorithm */
#define CRYP_CCM_CTR1_0                  0x07FFFFFFU
#define CRYP_CCM_CTR1_1                  0xFFFFFF00U
#define CRYP_CCM_CTR1_2                  0x00000001U

#define CRYP_KEYIVCONFIG_ALWAYS        0x00000000U            /*!< Peripheral Key and IV configuration to do systematically */
#define CRYP_KEYIVCONFIG_ONCE          0x00000001U            /*!< Peripheral Key and IV configuration to do only once      */

/**
  * @brief CRYP Init Structure definition
  */

typedef struct
{
  uint32_t DataType;                   /*!< 32-bit data, 16-bit data, 8-bit data or 1-bit string.
                                        This parameter can be a value of @ref CRYP_Data_Type */
  uint32_t KeySize;                    /*!< Used only in AES mode : 128, 192 or 256 bit key length in CRYP1.
                                        128 or 256 bit key length in TinyAES This parameter can be a value of @ref CRYP_Key_Size */
  uint32_t *pKey;                      /*!< The key used for encryption/decryption */
  uint32_t *pInitVect;                 /*!< The initialization vector used also as initialization
                                         counter in CTR mode */
  HAL_CRYP_AESTypeDef Algorithm;                  /*!<  DES/ TDES Algorithm ECB/CBC
                                        AES Algorithm ECB/CBC/CTR/GCM or CCM
                                        This parameter can be a value of @ref CRYP_Algorithm_Mode */
  uint32_t *Header;                    /*!< used only in AES GCM and CCM Algorithm for authentication,
                                        GCM : also known as Additional Authentication Data
                                        CCM : named B1 composed of the associated data length and Associated Data. */
  uint32_t HeaderSize;                /*!< The size of header buffer in word  */
  uint32_t *B0;                       /*!< B0 is first authentication block used only  in AES CCM mode */
  uint32_t DataWidthUnit;              /*!< Data With Unit, this parameter can be value of @ref CRYP_Data_Width_Unit*/
  uint32_t HeaderWidthUnit;            /*!< Header Width Unit, this parameter can be value of @ref CRYP_Header_Width_Unit*/
  uint32_t KeyIVConfigSkip;            /*!< CRYP peripheral Key and IV configuration skip, to config Key and Initialization
                                           Vector only once and to skip configuration for consecutive processings.
                                           This parameter can be a value of @ref CRYP_Configuration_Skip */

} CRYP_ConfigTypeDef;

typedef struct __CRYP_HandleTypeDef
{
#if defined (CRYP)
  CRYP_TypeDef                      *Instance;            /*!< CRYP registers base address */
#else /* AES*/
  void                       *Instance;            /*!< AES Register base address */
#endif /* End AES or CRYP */
  CRYP_ConfigTypeDef    Init;                /*!< CRYP required parameters */
  FunctionalState       AutoKeyDerivation;   /*!< Used only in TinyAES to allows to bypass or not key write-up before decryption.
                                                         This parameter can be a value of ENABLE/DISABLE */
  uint32_t  *pCrypInBuffPtr;  /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */
  uint32_t  *pCrypOutBuffPtr; /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */
  uint16_t  CrypHeaderCount;   /*!< Counter of header data */
  uint16_t  CrypInCount;      /*!< Counter of input data */
  uint16_t  CrypOutCount;     /*!< Counter of output data */
  uint16_t  Size;           /*!< length of input data in word */
  HAL_CRYP_PHASETypeDef  Phase;            /*!< CRYP peripheral phase */
  void      *hdmain;          /*!< CRYP In DMA handle parameters */
  void      *hdmaout;         /*!< CRYP Out DMA handle parameters */
  HAL_LockTypeDef       Lock;             /*!< CRYP locking object */
  HAL_CRYP_STATETypeDef State;            /*!< CRYP peripheral state */
  HAL_CRYP_ERRORTypeDef ErrorCode;        /*!< CRYP peripheral error code */
  uint32_t    KeyIVConfig;      /*!< CRYP peripheral Key and IV configuration flag, used when
                                     configuration can be skipped */
  uint32_t    SizesSum;         /*!< Sum of successive payloads lengths (in bytes), stored
                                     for a single signature computation after several
                                     messages processing */

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)
  void (*InCpltCallback)(struct __CRYP_HandleTypeDef *hcryp);      /*!< CRYP Input FIFO transfer completed callback  */
  void (*OutCpltCallback)(struct __CRYP_HandleTypeDef *hcryp);     /*!< CRYP Output FIFO transfer completed callback */
  void (*ErrorCallback)(struct __CRYP_HandleTypeDef *hcryp);       /*!< CRYP Error callback */

  void (* MspInitCallback)(struct __CRYP_HandleTypeDef *hcryp);    /*!< CRYP Msp Init callback  */
  void (* MspDeInitCallback)(struct __CRYP_HandleTypeDef *hcryp);  /*!< CRYP Msp DeInit callback  */

#endif /* (USE_HAL_CRYP_REGISTER_CALLBACKS) */
} CRYP_HandleTypeDef;
