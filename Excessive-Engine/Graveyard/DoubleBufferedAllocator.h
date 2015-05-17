#pragma once

#include "StackAllocator.h"

template< u32 alignment >
class DoubleBufferedAllocator
{
    u32 current;
    StackAllocator<alignment> stacks[2];

  public:
    DoubleBufferedAllocator() : current(-1) {}

    //half size goes to the first stack, half to the second
    DoubleBufferedAllocator(char* b, u32 s)
    {
      current = 0;
      stacks[0] = StackAllocator<alignment>(b, s/2);
      stacks[1] = StackAllocator<alignment>(b+s/2, s/2);
    }

    void swapBuffers()
    {
      ASSERT(current != -1);
      current = !current;
    }

    void clearCurrentBuffer()
    {
      ASSERT(current != -1);
      stacks[current].clear();
    }

    void* alloc(u32 s)
    {
      ASSERT(current != -1);
      return stacks[current].alloc(s);
    }
};