#include <iostream>
#include "physfs/physfs.h"

using namespace std;

int physFsTest()
{  
  //debugging confuses physfs, so manually start from D_32 until I figure it out...

  //exe full path
  //args[0] works too
  PHYSFS_init("C:/dev/Excessive-Team/D_32/Test.exe");
  //works
  //PHYSFS_addToSearchPath("../resources/hello.zip", 1);

  //LZMA2 does not work at the moment
  //use LZMA
  PHYSFS_addToSearchPath("../resources/hello.7z", 1);
  
  if(!PHYSFS_exists("hello.txt"))
  {
    cerr << "file does not exist" << endl;
  }

  auto file = PHYSFS_openRead("hello.txt");
  PHYSFS_sint64 size = PHYSFS_fileLength(file);
  char* buf = new char[size+1];
  int length_read = PHYSFS_read(file, buf, 1, size);
  PHYSFS_close(file);
  PHYSFS_deinit();

  buf[size] = '\0';
  cout << buf << endl;

  int  a;
  cin >> a;
  return 0;
}