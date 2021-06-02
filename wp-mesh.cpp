/*
 * wp-mesh.cpp
 */

#include "giomm/asyncresult.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "sigc++/functors/ptr_fun.h"
#include <giomm.h>

#include <Poco/Util/Application.h>

using Poco::Util::Application;

//using namespace std;

#include "dbob.hpp"
#include <iostream>
using std::cout;

// main loop.
Glib::RefPtr<Glib::MainLoop> loop;

using gstr = Glib::ustring;
namespace gl = Glib;
namespace dbus_monitor
{

void on_appeared(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                               const gstr& name,
                               const gstr& owner)
{
    cout << "appeared\n";
}

void on_vanished(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                               const gstr& name)
{
    cout << "vanished\n";
}

}

void
on_dbus_proxy_available(Glib::RefPtr<Gio::AsyncResult>& result)
{
    const auto proxy = Gio::DBus::Proxy::create_finish(result);
    if (!proxy)
    {
    std::cerr << "The proxy to the user's session bus was not successfully "
                 "created."
              << std::endl;
        loop->quit();
        return;
    }
    std::cout << "appeared";

}

// =================================
using str = std::string;

// =================================

str busname {"com.wp.sink"};
gd::BusType bustype = gd::BUS_TYPE_SYSTEM;
str objectpath {"/com/wp/sink"};
str ifname {busname};

#include <memory>
template <typename T>
using uptr = std::unique_ptr<T>;
uptr<DBob> dbus_object{nullptr};

void run()
{
    namespace gd = Gio::DBus;

    dbus_object = std::make_unique<DBob>( bustype, busname, objectpath, ifname );

    std::cout << "running\n";
    loop->run();

}

void create_connection()
{
    gl::RefPtr<gd::Connection> connection =
        gd::Connection::get_sync(gd::BUS_TYPE_SYSTEM);
  // Check for an unavailable connection.
  if (!connection)
  {
    std::cerr << "The user's session bus is not available." << std::endl;
    return;
  }

  Gio::DBus::Proxy::create(connection, busname, objectpath,
        ifname, sigc::ptr_fun(&on_dbus_proxy_available));

}

void watch_busname()
{
   uint id = Gio::DBus::watch_name(Gio::DBus::BusType::BUS_TYPE_SYSTEM,
                               busname,
                               sigc::ptr_fun( &dbus_monitor::on_appeared),
                               sigc::ptr_fun( &dbus_monitor::on_vanished));
}

void gioinit()
{
  Gio::init();
  loop = Glib::MainLoop::create();
}

int main()
{

    gioinit();

    watch_busname();

    create_connection();

    run();

  return 0;
}
