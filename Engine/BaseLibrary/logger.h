#ifndef Logger_h
#define Logger_h

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

  class Logger
  {
    private:
    protected:
    public:
      template< class t >
      Logger& operator<<( t message )
      {
        std::cout << message;
        return *this;
      }
  };

  template<>
   Logger& Logger::operator<<( impl::locker message )
  {
    impl::mtx.lock();
    return *this;
  }

  template<>
   Logger& Logger::operator<<( impl::unlocker message )
  {
    impl::mtx.unlock();
    return *this;
  }

  class WLogger
  {
    private:
    protected:
    public:
      template< class t >
      WLogger& operator<<( t message )
      {
        std::wcout << message;
        return *this;
      }
  };

  template<>
   WLogger& WLogger::operator<<( impl::locker message )
  {
    impl::mtx.lock();
    return *this;
  }

  template<>
   WLogger& WLogger::operator<<( impl::unlocker message )
  {
    impl::mtx.unlock();
    return *this;
  }

  static Logger log;
  static WLogger wlog;

  static std::string endl = "\n";
  static std::wstring wendl = L"\n";
}

#endif
