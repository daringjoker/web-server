#ifndef webserver__algo__search
#define webserver__algo__search
#include <functional>
#include <iostream>
#include <vector>
template <typename ItemType, typename KeyType, typename MapperType>
long long int _lessThanOrEqual(const std::vector<ItemType>& arr,
                               unsigned long long int low,
                               unsigned long long int high, KeyType& key,
                               MapperType& mapper) {
  long long int ans = -1;

  while (low <= high) {
    long long int mid = low + (high - low + 1) / 2;
    KeyType midVal = mapper(arr[mid]);
    if (midVal < key) {
      ans = mid;
      low = mid + 1;
    } else if (midVal > key) {
      high = mid - 1;
    } else if (midVal == key) {
      return mid;
    }
  }
  return ans;
}
template <typename ItemType, typename KeyType, typename MapperType>
int lessThanOrEqual(const std::vector<ItemType>& arr, const KeyType& key,
                    const MapperType& mapper) {
  return _lessThanOrEqual(arr, 0, arr.size() - 1, key, mapper);
}

template <typename ItemType>
int lessThanOrEqual(std::vector<ItemType>& arr, ItemType& key) {
  return _lessThanOrEqual(arr, 0, arr.size() - 1, key,
                          [](ItemType x) { return x; });
}
#endif  // !webserver__algo__search
