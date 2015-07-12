#include <iostream>

/*
#include "client/windows/crash_generation/client_info.h"
#include "client/windows/crash_generation/crash_generation_server.h"
#include "client/windows/handler/exception_handler.h"
#include "client/windows/common/ipc_protocol.h"

#include <Windows.h>
#include <tchar.h>

using namespace std;

const wchar_t pipe_name[] = L"\\\\.\\pipe\\BreakpadCrashServices\\EETestServer";
google_breakpad::CustomInfoEntry custom_info[] = 
{
  google_breakpad::CustomInfoEntry(L"prod", L"CrashTestApp"),
  google_breakpad::CustomInfoEntry(L"ver", L"1.0"),
};
int custom_info_count = 2;
google_breakpad::ExceptionHandler* handler = 0;
int max_len = 256;
google_breakpad::CrashGenerationServer* crash_server = 0;

void crashServerStart()
{
  if( crash_server )
    return;

  wstring s = L"C:\\dumps\\";

  if (_wmkdir(s.c_str()) && (errno != EEXIST)) 
  {
    cerr << "unable to Create dump dir" << endl;
    return;
  }

  crash_server = new google_breakpad::CrashGenerationServer( pipe_name,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             0,
                                                             true,
                                                             &s );
  if( !crash_server->Start() )
  {
    cerr << "Couldn't start server" << endl;
    delete crash_server;
    crash_server = 0;
  }
}

void crashServerStop() 
{
  delete crash_server;
    crash_server = 0;
}

int breakpadTest()
{
  crashServerStart();
  _CrtSetReportMode(_CRT_ASSERT, 0);
  google_breakpad::CustomClientInfo ci = {custom_info, custom_info_count};
  handler = new google_breakpad::ExceptionHandler( L"C:\\dumps\\", 
                                                   0, 
                                                   0, 
                                                   0, 
                                                   google_breakpad::ExceptionHandler::HANDLER_ALL, 
                                                   MiniDumpNormal, 
                                                   pipe_name, 
                                                   &ci );

  cout << "hello" << endl;
  
  int* p = 0;
  cout << *p << endl;
  
  int  a;
  cin >> a;
  return 0;
}
*/