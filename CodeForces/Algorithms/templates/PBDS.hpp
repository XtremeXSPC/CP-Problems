#pragma once
#include "Preamble.hpp"

//===----------------------------------------------------------------------===//
/* Policy-Based Data Structures (optional) */

#if defined(PBDS_AVAILABLE) && PBDS_AVAILABLE
using namespace __gnu_pbds;

template <typename T>
using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename T>
using ordered_multiset = tree<T, null_type, std::less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename K, typename V>
using ordered_map = tree<K, V, std::less<K>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename K, typename V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K,
    V,
    std::hash<K>,
    std::equal_to<K>,
    direct_mask_range_hashing<>,
    linear_probe_fn<>,
    hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>>;
#endif
