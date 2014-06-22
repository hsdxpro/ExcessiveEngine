#ifndef logger_h
#define logger_h

#include <mutex>
#include <iostream>
#include <string>
#include <ostream>

namespace my
{
  namespace impl
  {
    static std::mutex mtx;

    class l {};
    class unl {};

    typedef void ( *locker )( l );
    typedef void ( *unlocker )( unl );
  }

  static impl::locker lock;
  static impl::unlocker unlock;

  class logger
  {
    private:
    protected:
    public:
      template< class t >
      logger& operator<<( t message )
      {
        std::cout << message;
        return *this;
      }
  };

  template<>
  inline logger& logger::operator<<( impl::locker message )
  {
    impl::mtx.lock();
    return *this;
  }

  template<>
  inline logger& logger::operator<<( impl::unlocker message )
  {
    impl::mtx.unlock();
    return *this;
  }

  class wlogger
  {
    private:
    protected:
    public:
      template< class t >
      wlogger& operator<<( t message )
      {
        std::wcout << message;
        return *this;
      }
  };

  template<>
  inline wlogger& wlogger::operator<<( impl::locker message )
  {
    impl::mtx.lock();
    return *this;
  }

  template<>
  inline wlogger& wlogger::operator<<( impl::unlocker message )
  {
    impl::mtx.unlock();
    return *this;
  }

  static logger log;
  static wlogger wlog;

  static std::string endl = "\n";
  static std::wstring wendl = L"\n";
}

#endif
