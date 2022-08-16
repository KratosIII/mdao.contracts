#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/time.hpp>
#include <xdao.conf/xdao.conf.hpp>
#include "xdao.propose.db.hpp"
#include <thirdparty/wasm_db.hpp>

using namespace eosio;
using namespace wasm::db;
using namespace xdao;
using namespace std;

// static constexpr symbol   AM_SYMBOL = symbol(symbol_code("AMAX"), 8);

// static constexpr name AMAX_TOKEN{"amax.token"_n};
// static constexpr name XDAO_CONF{"xdao.conf"_n};
// static constexpr name XDAO_STG{"xdao.stg"_n};
// static constexpr name XDAO_GOV{"xdao.gov"_n};
// static constexpr name XDAO_VOTE{"xdao.vote"_n};
// static constexpr name AMAX_MULSIGN{"amax.mulsign"_n};

namespace propose_status {
    static constexpr name CREATED    = "create"_n;
    static constexpr name RUNNING    = "running"_n;
    static constexpr name EXCUTING   = "excuting"_n;
    static constexpr name CANCELLED  = "cancelled"_n;

};

namespace vote_type {
    static constexpr name PLEDGE     = "pledge"_n;
    static constexpr name TOKEN      = "token"_n;

};

// namespace conf_status {
//     static constexpr name INITIAL    = "initial"_n;
//     static constexpr name RUNNING    = "running"_n;
//     static constexpr name CANCEL     = "cancel"_n;

// };

// namespace manager {
//     static constexpr name INFO       = "info"_n;
//     static constexpr name STRATEGY   = "strategy"_n;
//     static constexpr name GOV        = "gov"_n;
//     static constexpr name WALLET     = "wallet"_n;
//     static constexpr name TOKEN      = "token"_n;
//     static constexpr name VOTE       = "vote"_n;

// };

enum class propose_err: uint8_t {
    ACCOUNT_NOT_EXITS       = 0,
    RECORD_NOT_FOUND        = 1,
    STRATEGY_NOT_FOUND      = 2,
    NOT_REPEAT_RECEIVE      = 4,
    RECORD_EXITS            = 5,
    GOVERNANCE_NOT_FOUND    = 6,
    SIZE_TOO_MUCH           = 7,
    WALLET_NOT_FOUND        = 8,
    CODE_REPEAT             = 9,
    TITLE_REPEAT            = 10,
    PERMISSION_DENIED       = 11,
    CANNOT_ZERO             = 12,
    INVALID_FORMAT          = 13,
    INCORRECT_FEE           = 14,
    NOT_AVAILABLE           = 15,
    SYSTEM_ERROR            = 16,
    INSUFFICIENT_VOTES      = 17,
    VOTED                   = 18,
    STATUS_ERROR            = 19,
    OPTS_EMPTY              = 20

};

class [[eosio::contract("xdao.propose")]] xdaopropose : public contract {

using conf_t = xdao::conf_global_t;
using conf_table_t = xdao::conf_global_singleton;

private:
    dbc                 _db;
    std::unique_ptr<conf_table_t> _conf_tbl_ptr;
    std::unique_ptr<conf_t> _conf_ptr;

    const conf_t& _conf();

public:
    using contract::contract;
    xdaopropose(name receiver, name code, datastream<const char*> ds):_db(_self),  contract(receiver, code, ds){}

    [[eosio::action]]
    ACTION create(const name& creator,const string& name, const string& desc,
                            const uint64_t& stgid, const uint32_t& votes);

    [[eosio::action]]
    ACTION cancel(const name& owner, const uint64_t& proposeid);

    [[eosio::action]]
    ACTION addoption( const name& owner, const uint64_t& proposeid, const string& title );

    [[eosio::action]]
    ACTION start(const name& owner, const uint64_t& proposeid);

    [[eosio::action]]
    ACTION excute(const name& owner, const uint64_t& proposeid);

    [[eosio::action]]
    ACTION votefor(const name& voter, const uint64_t& proposeid, const uint32_t optid);

};