
#ifndef _FRAME_H_
#define _FRAME_H_

// Class frame

class Frame {
public:
  int sender;
  int receiver;
  int data[256];
  int d_length;

  Frame(int sender, int receiver, int length);

  // Create frame from buffer
  Frame(int* buffer, int length);

  void serialize(int* buffer);
};

#endif
