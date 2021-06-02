/*
 * dbob.hpp
 */

#ifndef DBOB_HPP
#define DBOB_HPP

#include <glibmm.h>
#include <giomm.h>

namespace gd = Gio::DBus;

class DBob : public gd::Proxy
{
public:

    DBob(gd::BusType bus_type,
        const Glib::ustring& busname,
        const Glib::ustring& object_path,
        const Glib::ustring& interface_name
        ): gd::Proxy (
                bus_type, busname, object_path, interface_name
              )
        {  }

protected:

    void on_signal(const Glib::ustring& sender_name,
            const Glib::ustring& signal_name,
            const Glib::VariantContainerBase& parameters) override;
};

#endif /* !DBOB_HPP */
