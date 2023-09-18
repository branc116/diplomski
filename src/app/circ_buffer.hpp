#pragma once
#include <stdlib.h>

template<class T, size_t buffSize>
struct CircBuffer {
  T& push(T el) {
    auto& ret = buffer[pos++] = el;
    if (pos >= buffSize) pos = 0;
    if (len < buffSize) ++len;
    return ret;
  }
  T pop() {
    int index = (pos - len + buffSize) % buffSize;
    auto ret = buffer[index];
    if (len > 0) --len;
    return ret;
  }
  const T& peek() {
    int index = (pos - len + buffSize) % buffSize;
    return buffer[index];
  }
  size_t pos = 0;
  size_t len = 0;
  T buffer[buffSize] = {};
};

