/**
 * @file
 * Type interfaces for the MR-PDP module for libpdp.
 *
 * @author Copyright (c) 2012, Mark Gondree
 * @author Copyright (c) 2012, Alric Althoff
 * @author Copyright (c) 2008, Zachary N J Peterson
 * @date 2008-2013
 * @copyright BSD 2-Clause License,
 *            See http://opensource.org/licenses/BSD-2-Clause
 **/
/** @addtogroup MRPDP
 * @{ 
 */
#ifndef __MR_PDP_TYPES_H__
#define __MR_PDP_TYPES_H__

#include <openssl/rsa.h>
#include <openssl/bn.h>


/// Holds parameter data for MR-PDP
typedef struct {
    unsigned int opts;                  ///< some processing options
    unsigned int block_size;            ///< block size, in bytes
    unsigned int num_blocks;            ///< total number of blocks in file
    unsigned int num_challenge_blocks;  ///< num of blocks to challenge
    unsigned int numReplicas;           ///< number of Replicas to be generated
    size_t prf_key_size;                ///< size of (HMAC) key, in bytes
    size_t prp_key_size;                ///< size of (AES) key, in bytes
    size_t rsa_key_size;                ///< size of (RSA) modulus, in bytes
    size_t prf_w_size;                  ///< max size of PRF, gen_prf_w
    size_t prf_f_size;                  ///< max size of PRF, gen_prf_f
} pdp_mrpdp_ctx_t;


/// Holds key data for MR-PDP
/// Key gen and block tagging function the same as A-PDP
typedef struct {
    RSA *rsa;           ///< RSA key pair
    unsigned char *v;   ///< PRF key
    BIGNUM *g;          ///< APDP generator
} pdp_mrpdp_key_t;


/// Holds data for a single tag
typedef struct {
    unsigned int index;       ///< The index of the block, i
    size_t index_prf_size;    ///< The size of the prf output
    BIGNUM *Tim;              ///< The tag of the message block i
                              ///<   \f$ T_{i,m} = (h(W_i) * g^m)^d \f$
    unsigned char *index_prf; ///< The PRF output of the index 
                              ///    \f$ W_i = w_v(i) \f$
} pdp_mrpdp_tag_t;


/// Holds data for MR-PDP tags
typedef struct {
    pdp_mrpdp_tag_t **tags;      ///< array of pointers to tags
    unsigned int tags_size;     ///< byte length of array
    unsigned int tags_num;      ///< number of items in array
} pdp_mrpdp_tagdata_t;


/// Holds challenge data for MR-PDP
/// ADD: Code to specify replica that needs to be challenged
typedef struct {
    unsigned int c;             ///< Number of blocks to sample
    BIGNUM *g_s;                ///< Random secret base \f$ g_s = g^s \f$
    BIGNUM *s;                  ///< Random secret
    unsigned char *k1;          ///< PRP key
    unsigned char *k2;          ///< PRF key
} pdp_mrpdp_challenge_t;


/// Holds proof data for MR-PDP
typedef struct {
    BIGNUM *T;          ///< The product of tags, T
    BIGNUM *rho_temp;   ///< A running tally of rho
    unsigned char *rho; ///< Final \f$ \rho \f$
    size_t rho_size;    ///< size of the final rho
} pdp_mrpdp_proof_t;


#endif
/** @} */
