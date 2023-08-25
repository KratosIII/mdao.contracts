#pragma once

#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <eosio/privileged.hpp>
#include <eosio/name.hpp>
#include <eosio/transaction.hpp>
#include <map>

namespace mdao {

using namespace std;
using namespace eosio;

#define TG_TBL [[eosio::table, eosio::contract("mdao.propose")]]
#define PROPOSE_TABLE_NAME(name) [[eosio::table(name), eosio::contract("mdao.propose")]]

static uint128_t get_union_id(const name& account, const uint64_t& proposal_id){
    return ( (uint128_t)account.value ) << 64 | proposal_id;
}

struct option{
    string      title;
    string      desc;
    uint32_t    recv_votes = 0;
    transaction execute_actions;
};

struct TG_TBL proposal_t {
    uint64_t        id;
    name            dao_code;
    uint64_t        proposal_strategy_id;
    uint64_t        vote_strategy_id;
    name            status;
    name            creator;
    string          title;
    string          desc;
    name            type;
    uint32_t        approve_votes; //agree total vote
    uint32_t        deny_votes; //deny total vote
    uint32_t        waive_votes; //waive total vote
    uint32_t        users_count; //total users
    uint32_t        deny_users_count; //deny total users
    uint32_t        waive_users_count; //waive total user
    time_point_sec  created_at = current_time_point();
    time_point_sec  started_at;
    time_point_sec  executed_at;
    map<string, option>    options;

    uint64_t    primary_key()const { return id; }
    uint64_t    scope() const { return 0; }
    uint64_t    by_creator()const {
        return creator.value;
    }
    uint64_t    by_daocode()const {
        return dao_code.value;
    }
    uint128_t by_union_id()const {
        return get_union_id( creator, dao_code.value );
    }
    proposal_t() {}
    proposal_t(const uint64_t& i): id(i) {}

    EOSLIB_SERIALIZE( proposal_t, (id)(dao_code)(proposal_strategy_id)(vote_strategy_id)(status)(creator)(title)(desc)(type)
                (approve_votes)(deny_votes)(waive_votes)(users_count)(deny_users_count)(waive_users_count)(created_at)(started_at)(executed_at)(options) )

    typedef eosio::multi_index <"proposals"_n, proposal_t,
        indexed_by<"creator"_n,  const_mem_fun<proposal_t, uint64_t, &proposal_t::by_creator> >,
        indexed_by<"daocode"_n,  const_mem_fun<proposal_t, uint64_t, &proposal_t::by_daocode> >,
        indexed_by<"unionid"_n,  const_mem_fun<proposal_t, uint128_t, &proposal_t::by_union_id> >
    > idx_t;
};

struct TG_TBL vote_t {
    uint64_t        id;
    name            account;
    name            direction; //approve ｜ deny ｜ waive
    uint64_t        proposal_id;
    string          title;
    uint32_t        vote_weight;
    // asset           quantity;
    time_point_sec  voted_at;

    uint64_t    primary_key()const { return id; }
    uint64_t    scope() const { return 0; }

    vote_t() {}
    vote_t(const uint64_t& pid): proposal_id(pid) {}

    uint64_t by_account() const { return account.value; }
    uint128_t by_union_id()const {
        return get_union_id( account, proposal_id);
    }
    EOSLIB_SERIALIZE( vote_t, (id)(account)(direction)(proposal_id)(title)(vote_weight)(voted_at) )

    typedef eosio::multi_index <"votes"_n, vote_t,
        indexed_by<"accountid"_n,  const_mem_fun<vote_t, uint64_t, &vote_t::by_account> >,
        indexed_by<"unionid"_n,  const_mem_fun<vote_t, uint128_t, &vote_t::by_union_id> >
    > idx_t;
};

struct PROPOSE_TABLE_NAME("global") prop_global_t {
    uint64_t last_propose_id = 0;
    EOSLIB_SERIALIZE( prop_global_t, (last_propose_id) )
};

typedef eosio::singleton< "global"_n, prop_global_t > propose_global_singleton;
} //amax