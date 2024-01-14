#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

typedef int tp;
typedef tree<tp,null_type,less<tp>,rb_tree_tag,tree_order_statistics_node_update> pbds;
// tp a;
// T.insert(a), T.erase(a), T.size()
// T.order_of_key(a) -- number of elements strictly less than a
// *T.find_by_order(k) -- k-th element in increasing order
