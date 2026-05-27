#include "modules/data_structures/utility/RemovableQueue.hpp"

int main() {
  RemovableMinPriorityQueue<I64> pq;
  pq.push(4);
  return pq.top() == 4 ? 0 : 1;
}
