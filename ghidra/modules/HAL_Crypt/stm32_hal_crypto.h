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
  uint32_t Algorithm;                  /*!<  DES/ TDES Algorithm ECB/CBC
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


/**
  * @brief  CRYP handle Structure definition
  */

typedef struct __CRYP_HandleTypeDef
{
#if defined (CRYP)
  CRYP_TypeDef                      *Instance;            /*!< CRYP registers base address */
#else /* AES*/
  void                       *Instance;            /*!< AES Register base address */
#endif /* End AES or CRYP */

  CRYP_ConfigTypeDef                Init;             /*!< CRYP required parameters */

  FunctionalState                   AutoKeyDerivation;   /*!< Used only in TinyAES to allows to bypass or not key write-up before decryption.
                                                         This parameter can be a value of ENABLE/DISABLE */

  uint32_t                          *pCrypInBuffPtr;  /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */

  uint32_t                          *pCrypOutBuffPtr; /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */

  uint16_t                     CrypHeaderCount;   /*!< Counter of header data */

  uint16_t                     CrypInCount;      /*!< Counter of input data */

  uint16_t                     CrypOutCount;     /*!< Counter of output data */

  uint16_t                          Size;           /*!< length of input data in word */

  uint32_t                          Phase;            /*!< CRYP peripheral phase */

  void                 *hdmain;          /*!< CRYP In DMA handle parameters */

  void                 *hdmaout;         /*!< CRYP Out DMA handle parameters */

  HAL_LockTypeDef                   Lock;             /*!< CRYP locking object */

   HAL_CRYP_STATETypeDef       State;            /*!< CRYP peripheral state */

  uint32_t                     ErrorCode;        /*!< CRYP peripheral error code */

  uint32_t                          KeyIVConfig;      /*!< CRYP peripheral Key and IV configuration flag, used when
                                                           configuration can be skipped */

  uint32_t                          SizesSum;         /*!< Sum of successive payloads lengths (in bytes), stored
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
