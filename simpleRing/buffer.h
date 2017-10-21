
#ifndef _BUFFER_H_
#define _BUFFER_H_

// Class shared buffer
class Buffer {
private:
  int buf[128];

  int wr_i;  // Current write index
  int rd_i;  // Current read index
  int size;  // Capacity

public:
  Buffer(int capacity);

  // Add data
  bool add(int n, int* data);

  // Get a number of symbols from buffer,
  // if there is not enough symbols return false
  bool get(int n, int* data);

  // Remove number of symbols from buffer,
  // if there is not enough return false
  bool remove(int n);
};

#endif
