#include "modules/data_structures/utility/RemovableQueue.hpp"

#include <cassert>

int main() {
  RemovableMinPriorityQueue<I32> pq;
  pq.push(5);
  pq.push(1);
  pq.push(3);
  pq.remove(1);
  assert(pq.top() == 3);
  assert(pq.pop() == 3);
  assert(pq.top() == 5);
  pq.remove(5);
  assert(pq.empty());

  RemovablePriorityQueue<I32> max_pq;
  max_pq.push(2);
  max_pq.push(7);
  max_pq.remove(7);
  assert(max_pq.pop() == 2);
  return 0;
}
