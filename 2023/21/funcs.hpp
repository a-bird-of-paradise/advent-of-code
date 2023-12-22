#pragma once
#include "context.hpp"
#include "parser.hpp"
#include <map>
#include <set>

auto state_maker(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>;

auto state_maker_top_left(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>;

auto state_maker_top_right(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>;

auto state_maker_bottom_left(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>;

auto state_maker_bottom_right(const std::vector<std::string>& initial, const int imax) ->
    std::map<std::vector<std::string>,std::vector<int>>;

auto neighbours(const std::vector<std::string>& field, const std::pair<int64_t,int64_t>& point) -> std::vector<std::pair<int64_t,int64_t>>;

auto reached( 
    const std::map<std::pair<int64_t,int64_t>, std::vector<std::pair<int64_t,int64_t>>>& lookup_table,
    int64_t steps, 
    const std::pair<int64_t,int64_t>& start) -> std::set<std::pair<int64_t,int64_t>>;