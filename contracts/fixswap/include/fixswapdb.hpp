#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/privileged.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
#include <thirdparty/wasm_db.hpp>

using namespace eosio;
using namespace std;
using std::string;

using namespace wasm;
#define SYMBOL(sym_code, precision) symbol(symbol_code(sym_code), precision)

static constexpr uint64_t seconds_per_day                   = 24 * 3600;
static constexpr uint64_t default_expired_secs = 30 * seconds_per_day;

static constexpr uint64_t percent_boost = 10000;
#define HASH256(str) sha256(str.c_str(), str.size())

static constexpr name   APLINK_FARM              = "aplink.farm"_n;
static constexpr symbol   APLINK_SYMBOL              = SYMBOL("APL", 4);

using checksum256 = fixed_bytes<32>;

namespace swap_status_t {
    static constexpr eosio::name created        = "created"_n;
    static constexpr eosio::name initialized        = "initialized"_n;
    static constexpr eosio::name maintaining       = "maintaining"_n;
};

namespace wasm
{
    namespace db
    {

    #define SWAP_TBL [[eosio::table, eosio::contract("fixswap")]]
    #define SWAP_TBL_NAME(name) [[eosio::table(name), eosio::contract("fixswap")]]
    
    struct SWAP_TBL_NAME("global") gswap_t
    {
        name status = swap_status_t::created;
        name fee_collector;
        uint32_t fee_ratio      = 30;
        uint64_t swap_id        = 0;

        uint64_t farm_lease_id  = 0;
        map <extended_symbol, uint32_t> farm_scales;
        EOSLIB_SERIALIZE(gswap_t, (status)(fee_collector)(fee_ratio)(swap_id)(farm_lease_id)(farm_scales))
    };

    typedef eosio::singleton<"global"_n, gswap_t> gswap_singleton;

    struct SWAP_TBL swap_t
    {
        uint64_t id;
        name maker;
        name taker = name(0);
        extended_asset make_asset;
        extended_asset take_asset;
        string code;
        time_point_sec  expired_at;

        uint64_t primary_key() const { return id; }
        uint64_t by_maker()const { return maker.value; }

        swap_t() {}
        swap_t(const uint64_t &pid) : id(pid) {}

        typedef eosio::multi_index<"tswaps"_n, swap_t,
            indexed_by<"maker"_n, const_mem_fun<swap_t, uint64_t, &swap_t::by_maker> >
        > idx_t;

        EOSLIB_SERIALIZE(swap_t, (id)(maker)(taker)(make_asset)(take_asset)(code)(expired_at))
    };
}
}