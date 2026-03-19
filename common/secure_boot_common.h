#ifndef SECURE_BOOT_COMMON_H_
#define SECURE_BOOT_COMMON_H_

#define EFUSE_HSM_EN_INDEX (179)
#define HSM_EN_BIT         (27u)
#define HSM_EN_MASK        (0x1u << HSM_EN_BIT)
#define HSM_EN(x)          ((x & HSM_EN_MASK) >> HSM_EN_BIT)

#ifndef CONFIG_BT_TIMED
#define CONFIG_BT_TIMED 0
#endif

#if CONFIG_BT_TIMED
#define CNT_G1   (0x34)
#endif

#define APP_MEM_BASE (0x80FE000)
/* #define CONFIG_LOAD_VERIFY_IIB_PARALLEL (1) */
#define CONFIG_LOAD_VERIFY_IIB_PARALLEL (0)
#define SUPPORT_HASH_CALC_BY_SOFTWARE (1)

#endif /* SECURE_BOOT_COMMON_H_ */
