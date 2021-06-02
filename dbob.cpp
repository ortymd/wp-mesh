/*
 * dbob.cpp
 */

#include "dbob.hpp"
#include <iostream>

void DBob::on_signal(const Glib::ustring& sender_name,
            const Glib::ustring& signal_name,
            const Glib::VariantContainerBase& parameters)
{
    std::cout <<"on_signal\n";
}
