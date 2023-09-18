#pragma once
#include <stdlib.h>

extern "C" void InfLoop(void);

template<class T, size_t buffSize>
struct CircBuffer {
  T& push(T el) {
    auto& ret = buffer[pos] = el;
    size_t new_pos = pos + 1;
    if (new_pos >= buffSize) new_pos = 0;
    if (len < buffSize) ++len;
    pos = new_pos;
    return ret;
  }
  T pop() {
    if (len == 0) InfLoop();
    int index = ((pos + buffSize) - len ) % buffSize;
    if (len > 0) --len;
    return buffer[index];
  }
  const T& peek() {
    if (len == 0) InfLoop();
    int index = ((pos + buffSize) - len) % buffSize;
    return buffer[index];
  }
  size_t pos = 0;
  size_t len = 0;
  T buffer[buffSize] = {};
};

//*
//   0 | 0
//   1 | 1
//   2 | 2
//   0 | 3
//
//   0 | 2 Read index 0 ( 0 + 3 - 3 = 0 )
//   0 | 1 Read index 1 ( 0 + 3 - 2 = 1 )
//   0 | 0 Read index 2 ( 0 + 3 - 1 = 2 )
//   0 | 0 Read index 0 ( 0 + 3 - 0 = 0 )
//
//
//   0 | 0
//   1 | 1
//   2 | 2
//   2 | 1 Read index 0 ( 2 + 3 - 2 = 3 )
//   2 | 0 Read index 1 ( 2 + 3 - 1 = 4 = 1 )

