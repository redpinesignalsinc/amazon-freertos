/*
*  (C) Copyright 2008-2018 Marvell International Ltd. All Rights Reserved
*
*  MARVELL CONFIDENTIAL
*  The source code contained or described herein and all documents related to
*  the source code ("Material") are owned by Marvell International Ltd or its
*  suppliers or licensors. Title to the Material remains with Marvell
*  International Ltd or its suppliers and licensors. The Material contains
*  trade secrets and proprietary and confidential information of Marvell or its
*  suppliers and licensors. The Material is protected by worldwide copyright
*  and trade secret laws and treaty provisions. No part of the Material may be
*  used, copied, reproduced, modified, published, uploaded, posted,
*  transmitted, distributed, or disclosed in any way without Marvell's prior
*  express written permission.
*
*  No license under any patent, copyright, trade secret or other intellectual
*  property right is granted to or conferred upon you by disclosure or delivery
*  of the Materials, either expressly, by implication, inducement, estoppel or
*  otherwise. Any license under such intellectual property rights must be
*  express and approved by Marvell in writing.
*
*/

/** @file wm_utils.h
*
*  @brief This file contains WMSDK Utility functions and
*         Collection of some common helper functions
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <wmtypes.h>
#include <stddef.h>
#include <stdint.h>
#include <compat_attribute.h>

#define ffs __builtin_ffs

/* For PM3 mode in MW300 define special attribute for non-retention memory */
#if (CONFIG_CPU_MW300) && (CONFIG_ENABLE_MCU_PM3)
#define PM3_BSS \
	__SECTION__(.pm3_bss)
#else
#define PM3_BSS
#endif

/* DMA buffers for SDIO, USB needs to be in special memory region, limitation
 * specific to MC200 only and not for MW300.
 */
#ifdef CONFIG_CPU_MC200
#define _IOBUFS \
	__SECTION__(.iobufs)
#define _IOBUFS_ALIGNED(x) \
	__SECTION__(.iobufs) __ALIGNED__(x)
#define _IOBUFS_PAGE_ALIGNED(x) \
	__SECTION__(.iobufs.page_aligned) __ALIGNED__(x)

#elif (CONFIG_CPU_MW300) && !(CONFIG_ENABLE_MCU_PM3)
#define _IOBUFS
#define _IOBUFS_ALIGNED(x) \
	__ALIGNED__(x)
#define _IOBUFS_PAGE_ALIGNED(x) \
	__ALIGNED__(x)

#elif (CONFIG_CPU_MW300) && (CONFIG_ENABLE_MCU_PM3)
#define _IOBUFS \
	__SECTION__(.pm3_bss)
#define _IOBUFS_ALIGNED(x) \
	__SECTION__(.pm3_bss) __ALIGNED__(x)
#define _IOBUFS_PAGE_ALIGNED(x) \
	__SECTION__(.pm3_bss) __ALIGNED__(x)

#endif

#ifdef __GNUC__

#define WARN_UNUSED_RET __attribute__ ((warn_unused_result))
#define WEAK __attribute__ ((weak))

#ifndef PACK_START
#define PACK_START
#endif
#ifndef PACK_END
#define PACK_END __attribute__((packed))
#endif
#define NORETURN __attribute__ ((noreturn))

/* alignment value should be a power of 2 */
#define ALIGN_X(num, align)  MASK(num, (typeof(num))align - 1)

#define ALIGN_2(num)               ALIGN_X(num, 2)
#define ALIGN_4(num)               ALIGN_X(num, 4)
#define ALIGN_8(num)               ALIGN_X(num, 8)
#define ALIGN_16(num)              ALIGN_X(num, 16)
#define ALIGN_32(num)              ALIGN_X(num, 32)

#else /* __GNUC__ */

#define WARN_UNUSED_RET
#define WEAK __weak

#define PACK_START __packed
#define PACK_END
#define NORETURN

#endif /* __GNUC__ */

/* alignment value should be a power of 2 */
#define __WM_ALIGN__(num, num_type, align)  MASK(num, (num_type)align - 1)
#define MASK(num, mask)    ((num + mask) & ~(mask))

NORETURN void wmpanic(void);

/**
 * Convert a given hex string to a equivalent binary representation.
 *
 * E.g. If your input string of 4 bytes is {'F', 'F', 'F', 'F'} the output
 * string will be of 2 bytes {255, 255} or to put the same in other way
 * {0xFF, 0xFF}
 *
 * Note that hex2bin is not the same as strtoul as the latter will properly
 * return the integer in the correct machine binary format viz. little
 * endian. hex2bin however does only in-place like replacement of two ASCII
 * characters to one binary number taking 1 byte in memory.
 *
 * @param[in] ibuf input buffer
 * @param[out] obuf output buffer
 * @param[in]  max_olen Maximum output buffer length
 *
 * @return length of the binary string
 */
unsigned int hex2bin(const uint8_t *ibuf, uint8_t *obuf,
		     unsigned max_olen);

/**
 * Convert given binary array to equivalent hex representation.
 *
 * @param[in] src Input buffer
 * @param[out]  dest Output buffer
 * @param[in] src_len Length of the input buffer
 * @param[in] dest_len Length of the output buffer
 *
 * @return void
 */
void bin2hex(uint8_t *src, char *dest, unsigned int src_len,
		unsigned int dest_len);

/** Function prototype for a random entropy/seed generator
 *
 * \return a 32bit random number
 */
typedef uint32_t (*random_hdlr_t)(void);

/** Register a random entropy generator handler
 *
 * This API allows applications to register their own random entropy
 * generator handlers that will be internally used by get_random_sequence()
 * to add even more randomization to the byte stream generated by it.
 *
 * \param[in] func Function pointer of type \ref random_hdlr_t
 *
 * \return WM_SUCCESS if successful
 * \return -WM_E_NOSPC if there is no space available for additional handlers
 */
int random_register_handler(random_hdlr_t func);

/** Un-register a random entropy generator handler
 *
 * This API can be used to un-register a handler registered using
 * random_register_handler()
 *
 * \param[in] func Function pointer of type \ref random_hdlr_t used during
 * registering
 *
 * \return WM_SUCCESS if successful
 * \return -WM_E_INVAL if the passed pointer is invalid
 */
int random_unregister_handler(random_hdlr_t func);

/** Register a random seed generator handler
 *
 * For getting better random numbers, the initial seed (ideally required only
 * once on every boot) should also be random. This API allows applications to
 * register their own seed generators. Applications can use any logic such
 * that a different seed is generated every time. A sample seed generator which
 * uses a combination of DAC (generating random noise) and ADC (that internally
 * samples the random noise) along with the flash id has already been provided.
 * Please have a look at sample_initialise_random_seed().
 *
 * The seed generator handler is called only once by the get_random_sequence()
 * function. Applications can also explicitly initialize the seed by calling
 * random_initialize_seed() after registering a handler.
 *
 * \param[in] func Function pointer of type \ref random_hdlr_t
 *
 * \return WM_SUCCESS if successful
 * \return -WM_E_NOSPC if there is no space available for additional handlers
 */
int random_register_seed_handler(random_hdlr_t func);

/** Un-register a random seed generator handler
 *
 * This API can be used to un-register a handler registered using
 * random_register_seed_handler()
 *
 * \param[in] func Function pointer of type \ref random_hdlr_t used during
 * registering
 *
 * \return WM_SUCCESS if successful
 * \return -WM_E_INVAL if the passed pointer is invalid
 */
int random_unregister_seed_handler(random_hdlr_t func);

/** Initialize the random number generator's seed
 *
 * The get_random_sequence() uses a random number generator that is
 * initialized with a seed when get_random_sequence() is called for the first
 * time. The handlers registered using random_register_seed_handler() are
 * used to generate the seed. If an application wants to explicitly initialize
 * the seed, this API can be used. The seed will then not be re-initialized
 * in get_random_sequence().
 */
void random_initialize_seed();

/** Sample random seed generator
 *
 * This is a sample random seed generator handler that can be registered using
 * random_register_seed_handler() to generate a random seed. This uses a
 * combination of DAC (generating random noise) and ADC (that internally
 * samples the random noise) along with the flash id to generate a seed.
 * It is recommended to register this handler and immediately call
 * random_initialize_seed() before executing any other application code,
 * especially if the application is going to use ADC/DAC for its own purpose.
 *
 * \return Random seed
 */
uint32_t sample_initialise_random_seed();

/** Generate random sequence of bytes
 *
 * This function generates random sequence of bytes in the user provided buffer.
 *
 * @param[out] buf The buffer to be populated with random data
 * @param[in] size The number of bytes of the random sequence required
 *
 */
void get_random_sequence(void *buf, unsigned int size);

#ifdef CONFIG_DEBUG_BUILD
/** Dump buffer in hex format on console
 *
 * This function prints the received buffer in HEX format on the console
 *
 * \param[in] data Pointer to the data buffer
 * \param[in] len Length of the data
 */
void dump_hex(const void *data, unsigned len);

/** Dump buffer in hex and ascii format on console
 *
 * This function prints the received buffer in HEX as well as ASCII
 * format on the console
 *
 * \param[in] data Pointer to the data buffer
 * \param[in] len Length of the data
 */
void dump_hex_ascii(const void *data, unsigned len);
void dump_ascii(const void *data, unsigned len);
void print_ascii(const void *data, unsigned len);
void dump_json(const void *buffer, unsigned len);
#else
#define dump_hex(...) do {} while (0)
#define dump_hex_ascii(...) do {} while (0)
#define dump_ascii(...) do {} while (0)
#define print_ascii(...) do {} while (0)
#define dump_json(...) do {} while (0)
#endif /* CONFIG_DEBUG_BUILD */

#ifndef __linux__
/** Returns a pointer to a new string which is a duplicate of the
 *  input string s.
 *  Memory for the new string is obtained allocated by the function.
 *
 *  It is caller's responsibility to free the memory after its use.
 *
 *  \param[in] s Pointer to string to be duplicated
 *
 *  \return Pointer to newly allocated string which is duplicate of
 *  input string
 *  \return NULL on error
 */
char *strdup(const char *s);
#endif /* ! __linux__ */


/** Calculate CRC32 using software algorithm
 *
 * @pre soft_crc32_init()
 *
 * soft_crc32() allows the user to calculate CRC32 values of arbitrary
 * sized buffers across multiple calls.
 *
 * @param[in] data__ Input buffer over which CRC32 is calculated.
 * @param[in] data_size Length of the input buffer.
 * @param[in] crc Previous CRC32 value used as starting point for given
 * buffer calculation.
 *
 * @return Calculated CRC32 value
 */
uint32_t soft_crc32(const void *data__, int data_size, uint32_t crc);
float wm_strtof(const char *str, char **endptr);

/**
 * Fill the given buffer with a sequential pattern starting from
 * given byte.
 *
 * For example, if the 'first_byte' is 0x45 and buffer size of 5 then
 * buffer will be set to {0x45, 0x46, 0x47, 0x48, 0x49}
 * @param[in] buffer The pattern will be set to this buffer.
 * @param[in] size Number of pattern bytes to the be written to the buffer.
 * @param[in] first_byte This is the value of first byte in the sequential
 * pattern.
 *
 * @return void
 */
void fill_sequential_pattern(void *buffer, int size, uint8_t first_byte);

/**
 * Verify if the the given buffer has a sequential pattern starting from
 * given byte.
 *
 * For example, if the 'first_byte' is 0x45 and buffer size of 5 then
 * buffer will be verified for presence of {0x45, 0x46, 0x47, 0x48, 0x49}
 * @param[in] buffer The pattern will be verified from this buffer.
 * @param[in] size Number of pattern bytes to the be verified from the buffer.
 * @param[in] first_byte This is the value of first byte in the sequential
 * pattern.
 *
 * @return 'true' If verification successful.
 * @return 'false' If verification fails.
 */
bool verify_sequential_pattern(const void *buffer, int size,
			       uint8_t first_byte);
#endif
