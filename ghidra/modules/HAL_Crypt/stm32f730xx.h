/******************************************************************************/
/*                                                                            */
/*                       Advanced Encryption Standard (AES)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for AES_CR register  *********************/
#define AES_CR_EN_Pos            (0U)
#define AES_CR_EN_Msk            (0x1UL << AES_CR_EN_Pos)                       /*!< 0x00000001 */
#define AES_CR_EN                AES_CR_EN_Msk                                 /*!< AES Enable */
#define AES_CR_DATATYPE_Pos      (1U)
#define AES_CR_DATATYPE_Msk      (0x3UL << AES_CR_DATATYPE_Pos)                 /*!< 0x00000006 */
#define AES_CR_DATATYPE          AES_CR_DATATYPE_Msk                           /*!< Data type selection */
#define AES_CR_DATATYPE_0        (0x1UL << AES_CR_DATATYPE_Pos)                 /*!< 0x00000002 */
#define AES_CR_DATATYPE_1        (0x2UL << AES_CR_DATATYPE_Pos)                 /*!< 0x00000004 */

#define AES_CR_MODE_Pos          (3U)
#define AES_CR_MODE_Msk          (0x3UL << AES_CR_MODE_Pos)                     /*!< 0x00000018 */
#define AES_CR_MODE              AES_CR_MODE_Msk                               /*!< AES Mode Of Operation */
#define AES_CR_MODE_0            (0x1UL << AES_CR_MODE_Pos)                     /*!< 0x00000008 */
#define AES_CR_MODE_1            (0x2UL << AES_CR_MODE_Pos)                     /*!< 0x00000010 */

#define AES_CR_CHMOD_Pos         (5U)
#define AES_CR_CHMOD_Msk         (0x803UL << AES_CR_CHMOD_Pos)                  /*!< 0x00010060 */
#define AES_CR_CHMOD             AES_CR_CHMOD_Msk                              /*!< AES Chaining Mode */
#define AES_CR_CHMOD_0           (0x001UL << AES_CR_CHMOD_Pos)                  /*!< 0x00000020 */
#define AES_CR_CHMOD_1           (0x002UL << AES_CR_CHMOD_Pos)                  /*!< 0x00000040 */
#define AES_CR_CHMOD_2           (0x800UL << AES_CR_CHMOD_Pos)                  /*!< 0x00010000 */

#define AES_CR_CCFC_Pos          (7U)
#define AES_CR_CCFC_Msk          (0x1UL << AES_CR_CCFC_Pos)                     /*!< 0x00000080 */
#define AES_CR_CCFC              AES_CR_CCFC_Msk                               /*!< Computation Complete Flag Clear */
#define AES_CR_ERRC_Pos          (8U)
#define AES_CR_ERRC_Msk          (0x1UL << AES_CR_ERRC_Pos)                     /*!< 0x00000100 */
#define AES_CR_ERRC              AES_CR_ERRC_Msk                               /*!< Error Clear */
#define AES_CR_CCFIE_Pos         (9U)
#define AES_CR_CCFIE_Msk         (0x1UL << AES_CR_CCFIE_Pos)                    /*!< 0x00000200 */
#define AES_CR_CCFIE             AES_CR_CCFIE_Msk                              /*!< Computation Complete Flag Interrupt Enable */
#define AES_CR_ERRIE_Pos         (10U)
#define AES_CR_ERRIE_Msk         (0x1UL << AES_CR_ERRIE_Pos)                    /*!< 0x00000400 */
#define AES_CR_ERRIE             AES_CR_ERRIE_Msk                              /*!< Error Interrupt Enable */
#define AES_CR_DMAINEN_Pos       (11U)
#define AES_CR_DMAINEN_Msk       (0x1UL << AES_CR_DMAINEN_Pos)                  /*!< 0x00000800 */
#define AES_CR_DMAINEN           AES_CR_DMAINEN_Msk                            /*!< Enable data input phase DMA management  */
#define AES_CR_DMAOUTEN_Pos      (12U)
#define AES_CR_DMAOUTEN_Msk      (0x1UL << AES_CR_DMAOUTEN_Pos)                 /*!< 0x00001000 */
#define AES_CR_DMAOUTEN          AES_CR_DMAOUTEN_Msk                           /*!< Enable data output phase DMA management */

#define AES_CR_GCMPH_Pos         (13U)
#define AES_CR_GCMPH_Msk         (0x3UL << AES_CR_GCMPH_Pos)                    /*!< 0x00006000 */
#define AES_CR_GCMPH             AES_CR_GCMPH_Msk                              /*!< GCM Phase */
#define AES_CR_GCMPH_0           (0x1UL << AES_CR_GCMPH_Pos)                    /*!< 0x00002000 */
#define AES_CR_GCMPH_1           (0x2UL << AES_CR_GCMPH_Pos)                    /*!< 0x00004000 */

#define AES_CR_KEYSIZE_Pos       (18U)
#define AES_CR_KEYSIZE_Msk       (0x1UL << AES_CR_KEYSIZE_Pos)                  /*!< 0x00040000 */
#define AES_CR_KEYSIZE           AES_CR_KEYSIZE_Msk                            /*!< Key size selection */

/*******************  Bit definition for AES_SR register  *********************/
#define AES_SR_CCF_Pos           (0U)
#define AES_SR_CCF_Msk           (0x1UL << AES_SR_CCF_Pos)                      /*!< 0x00000001 */
#define AES_SR_CCF               AES_SR_CCF_Msk                                /*!< Computation Complete Flag */
#define AES_SR_RDERR_Pos         (1U)
#define AES_SR_RDERR_Msk         (0x1UL << AES_SR_RDERR_Pos)                    /*!< 0x00000002 */
#define AES_SR_RDERR             AES_SR_RDERR_Msk                              /*!< Read Error Flag */
#define AES_SR_WRERR_Pos         (2U)
#define AES_SR_WRERR_Msk         (0x1UL << AES_SR_WRERR_Pos)                    /*!< 0x00000004 */
#define AES_SR_WRERR             AES_SR_WRERR_Msk                              /*!< Write Error Flag */
#define AES_SR_BUSY_Pos          (3U)
#define AES_SR_BUSY_Msk          (0x1UL << AES_SR_BUSY_Pos)                     /*!< 0x00000008 */
#define AES_SR_BUSY              AES_SR_BUSY_Msk                               /*!< Busy Flag */

/*******************  Bit definition for AES_DINR register  *******************/
#define AES_DINR_Pos             (0U)
#define AES_DINR_Msk             (0xFFFFFFFFUL << AES_DINR_Pos)                 /*!< 0xFFFFFFFF */
#define AES_DINR                 AES_DINR_Msk                                  /*!< AES Data Input Register */

/*******************  Bit definition for AES_DOUTR register  ******************/
#define AES_DOUTR_Pos            (0U)
#define AES_DOUTR_Msk            (0xFFFFFFFFUL << AES_DOUTR_Pos)                /*!< 0xFFFFFFFF */
#define AES_DOUTR                AES_DOUTR_Msk                                 /*!< AES Data Output Register */

/*******************  Bit definition for AES_KEYR0 register  ******************/
#define AES_KEYR0_Pos            (0U)
#define AES_KEYR0_Msk            (0xFFFFFFFFUL << AES_KEYR0_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR0                AES_KEYR0_Msk                                 /*!< AES Key Register 0 */

/*******************  Bit definition for AES_KEYR1 register  ******************/
#define AES_KEYR1_Pos            (0U)
#define AES_KEYR1_Msk            (0xFFFFFFFFUL << AES_KEYR1_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR1                AES_KEYR1_Msk                                 /*!< AES Key Register 1 */

/*******************  Bit definition for AES_KEYR2 register  ******************/
#define AES_KEYR2_Pos            (0U)
#define AES_KEYR2_Msk            (0xFFFFFFFFUL << AES_KEYR2_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR2                AES_KEYR2_Msk                                 /*!< AES Key Register 2 */

/*******************  Bit definition for AES_KEYR3 register  ******************/
#define AES_KEYR3_Pos            (0U)
#define AES_KEYR3_Msk            (0xFFFFFFFFUL << AES_KEYR3_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR3                AES_KEYR3_Msk                                 /*!< AES Key Register 3 */

/*******************  Bit definition for AES_KEYR4 register  ******************/
#define AES_KEYR4_Pos            (0U)
#define AES_KEYR4_Msk            (0xFFFFFFFFUL << AES_KEYR4_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR4                AES_KEYR4_Msk                                 /*!< AES Key Register 4 */

/*******************  Bit definition for AES_KEYR5 register  ******************/
#define AES_KEYR5_Pos            (0U)
#define AES_KEYR5_Msk            (0xFFFFFFFFUL << AES_KEYR5_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR5                AES_KEYR5_Msk                                 /*!< AES Key Register 5 */

/*******************  Bit definition for AES_KEYR6 register  ******************/
#define AES_KEYR6_Pos            (0U)
#define AES_KEYR6_Msk            (0xFFFFFFFFUL << AES_KEYR6_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR6                AES_KEYR6_Msk                                 /*!< AES Key Register 6 */

/*******************  Bit definition for AES_KEYR7 register  ******************/
#define AES_KEYR7_Pos            (0U)
#define AES_KEYR7_Msk            (0xFFFFFFFFUL << AES_KEYR7_Pos)                /*!< 0xFFFFFFFF */
#define AES_KEYR7                AES_KEYR7_Msk                                 /*!< AES Key Register 7 */

/*******************  Bit definition for AES_IVR0 register   ******************/
#define AES_IVR0_Pos             (0U)
#define AES_IVR0_Msk             (0xFFFFFFFFUL << AES_IVR0_Pos)                 /*!< 0xFFFFFFFF */
#define AES_IVR0                 AES_IVR0_Msk                                  /*!< AES Initialization Vector Register 0 */

/*******************  Bit definition for AES_IVR1 register   ******************/
#define AES_IVR1_Pos             (0U)
#define AES_IVR1_Msk             (0xFFFFFFFFUL << AES_IVR1_Pos)                 /*!< 0xFFFFFFFF */
#define AES_IVR1                 AES_IVR1_Msk                                  /*!< AES Initialization Vector Register 1 */

/*******************  Bit definition for AES_IVR2 register   ******************/
#define AES_IVR2_Pos             (0U)
#define AES_IVR2_Msk             (0xFFFFFFFFUL << AES_IVR2_Pos)                 /*!< 0xFFFFFFFF */
#define AES_IVR2                 AES_IVR2_Msk                                  /*!< AES Initialization Vector Register 2 */

/*******************  Bit definition for AES_IVR3 register   ******************/
#define AES_IVR3_Pos             (0U)
#define AES_IVR3_Msk             (0xFFFFFFFFUL << AES_IVR3_Pos)                 /*!< 0xFFFFFFFF */
#define AES_IVR3                 AES_IVR3_Msk                                  /*!< AES Initialization Vector Register 3 */

/*******************  Bit definition for AES_SUSP0R register  ******************/
#define AES_SUSP0R_Pos           (0U)
#define AES_SUSP0R_Msk           (0xFFFFFFFFUL << AES_SUSP0R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP0R               AES_SUSP0R_Msk                                /*!< AES Suspend registers 0 */

/*******************  Bit definition for AES_SUSP1R register  ******************/
#define AES_SUSP1R_Pos           (0U)
#define AES_SUSP1R_Msk           (0xFFFFFFFFUL << AES_SUSP1R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP1R               AES_SUSP1R_Msk                                /*!< AES Suspend registers 1 */

/*******************  Bit definition for AES_SUSP2R register  ******************/
#define AES_SUSP2R_Pos           (0U)
#define AES_SUSP2R_Msk           (0xFFFFFFFFUL << AES_SUSP2R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP2R               AES_SUSP2R_Msk                                /*!< AES Suspend registers 2 */

/*******************  Bit definition for AES_SUSP3R register  ******************/
#define AES_SUSP3R_Pos           (0U)
#define AES_SUSP3R_Msk           (0xFFFFFFFFUL << AES_SUSP3R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP3R               AES_SUSP3R_Msk                                /*!< AES Suspend registers 3 */

/*******************  Bit definition for AES_SUSP4R register  ******************/
#define AES_SUSP4R_Pos           (0U)
#define AES_SUSP4R_Msk           (0xFFFFFFFFUL << AES_SUSP4R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP4R               AES_SUSP4R_Msk                                /*!< AES Suspend registers 4 */

/*******************  Bit definition for AES_SUSP5R register  ******************/
#define AES_SUSP5R_Pos           (0U)
#define AES_SUSP5R_Msk           (0xFFFFFFFFUL << AES_SUSP5R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP5R               AES_SUSP5R_Msk                                /*!< AES Suspend registers 5 */

/*******************  Bit definition for AES_SUSP6R register  ******************/
#define AES_SUSP6R_Pos           (0U)
#define AES_SUSP6R_Msk           (0xFFFFFFFFUL << AES_SUSP6R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP6R               AES_SUSP6R_Msk                                /*!< AES Suspend registers 6 */

/*******************  Bit definition for AES_SUSP7R register  ******************/
#define AES_SUSP7R_Pos           (0U)
#define AES_SUSP7R_Msk           (0xFFFFFFFFUL << AES_SUSP7R_Pos)               /*!< 0xFFFFFFFF */
#define AES_SUSP7R               AES_SUSP7R_Msk                                /*!< AES Suspend registers 7 */
