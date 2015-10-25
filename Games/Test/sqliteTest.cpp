//#include <iostream>
//#include "sqlite/sqlite3.h"
//
//using namespace std;
//
//static int callback(void* unused, int argc, char** argv, char** colname)
//{
//  for(int i = 0; i < argc; ++i)
//  {
//    cout << colname[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
//  }
//  cout << endl;
//  return 0;
//}
//
////useful: http://sqlitestudio.pl/?act=download
////useful: http://www.sqlite.org/docs.html
//
//int sqliteTest()
//{  
//  sqlite3* db = 0;
//  char* err_msg = 0;
//  int rc = 0;
//
//  rc = sqlite3_open("C:/dev/Excessive-Team/resources/db.db", &db);
//  
//  if(rc)
//  {
//    cerr << sqlite3_errmsg(db) << endl;
//    sqlite3_close(db);
//  }
//
//  rc = sqlite3_exec(db, "select * from hello_world where name='marci'", callback, 0, &err_msg);
//
//  if( rc != SQLITE_OK )
//  {
//    cerr << err_msg << endl;
//    sqlite3_free(err_msg);
//  }
//
//  sqlite3_close(db);
//
//  int  a;
//  cin >> a;
//  return 0;
//}