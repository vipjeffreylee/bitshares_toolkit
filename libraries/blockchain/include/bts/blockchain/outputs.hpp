#pragma once
#include <bts/blockchain/address.hpp>
#include <bts/blockchain/pts_address.hpp>
#include <bts/blockchain/asset.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/io/enum_type.hpp>

namespace bts { namespace blockchain {

enum claim_type_enum
{
   /** basic claim by single address */
   null_claim_type      = 0,
   claim_by_pts         = 1, ///< someone importing a PTS balance
   claim_by_signature   = 2, ///< someone signs with an address
   claim_by_multi_sig   = 3, ///< N of M signatures required
   claim_by_password    = 4, ///< used for cross-chain trading
   // 10->19 reserved for BitShares X
   // 20->29 reserved for BitShares DNS
};

//typedef fc::enum_type<fc::unsigned_int,claim_type_enum> claim_type;
typedef uint8_t claim_type;

/**
 *   An input that references an output that can be claimed by
 *   an address.  A compact signature is used that when combined
 *   with the hash of the transaction containing this input will
 *   allow the public key and therefore address to be discovered and
 *   validated against the output claim function.
 */
struct claim_by_signature_input 
{
   static const claim_type_enum type;
};

struct claim_by_signature_output
{
   static const claim_type_enum type;
   claim_by_signature_output( const address& a ):owner(a){}
   claim_by_signature_output(){}
   address  owner; // checksummed hash of public key
};

struct claim_by_pts_output
{
   static const claim_type_enum type;
   claim_by_pts_output( const pts_address& a ):owner(a){}
   claim_by_pts_output(){}
   pts_address  owner; // checksummed hash of public key
};

/**
 *  This output can be claimed if sighed by payer & payee or
 *  signed by payee and the proper password is provided.
 */
struct claim_by_password_output
{
    static const claim_type_enum type;
    address          payer;
    address          payee;
    fc::ripemd160    hashed_password;
};

/**
 *   Provide the password such that ripemd160( password ) == claim_by_password_output::hashed_password
 */
struct claim_by_password_input
{
    static const claim_type_enum type;
    fc::uint128     password; ///< random number generated for cross chain trading
};


/**
 *  Used for multi-sig transactions that require N of M addresses to sign before
 *  the output can be spent.
 */
struct claim_by_multi_sig_output
{
    static const claim_type_enum type;
    fc::unsigned_int    required;
    std::vector<address> addresses;
};

struct claim_by_multi_sig_input
{
    static const claim_type_enum type;
};


} } // bts::blockchain

FC_REFLECT_ENUM( bts::blockchain::claim_type_enum, 
    (null_claim_type)
    (claim_by_pts)
    (claim_by_signature)
    (claim_by_multi_sig)
    (claim_by_password)
    )

FC_REFLECT( bts::blockchain::claim_by_signature_output, (owner) )
FC_REFLECT( bts::blockchain::claim_by_pts_output, (owner) )
FC_REFLECT( bts::blockchain::claim_by_multi_sig_output, (required)(addresses) )
FC_REFLECT( bts::blockchain::claim_by_password_output, (payer)(payee)(hashed_password) )

FC_REFLECT( bts::blockchain::claim_by_signature_input,    BOOST_PP_SEQ_NIL )
FC_REFLECT( bts::blockchain::claim_by_password_input,     (password)       )

