#include "LegacyRegistrar.hpp"
#include <iostream>
#include <glib/gstdio.h>

namespace Snnu::Infra::DBus
{

    LegacyRegistrar::LegacyRegistrar() : owner_id_(0)
    {
        dbus_skeleton_ = snnu_registrar_skeleton_new();
    }

    LegacyRegistrar::~LegacyRegistrar()
    {
        if (owner_id_ > 0)
        {
            g_bus_unown_name(owner_id_);
        }
        if (dbus_skeleton_)
        {
            g_object_unref(dbus_skeleton_);
        }
    }

    void LegacyRegistrar::Initialize()
    {
        g_signal_connect(
            dbus_skeleton_,
            "handle-register-window",
            G_CALLBACK(OnRegisterWindow),
            this);

        g_signal_connect(
            dbus_skeleton_,
            "handle-unregister-window",
            G_CALLBACK(OnUnregisterWindow),
            this);

        owner_id_ = g_bus_own_name(
            G_BUS_TYPE_SESSION,
            "com.canonical.AppMenu.Registrar",
            G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT,
            [](GDBusConnection *connection, const gchar *name, gpointer user_data)
            {
                auto *self = static_cast<LegacyRegistrar *>(user_data);
                g_dbus_interface_skeleton_export(
                    G_DBUS_INTERFACE_SKELETON(self->dbus_skeleton_),
                    connection,
                    "/com/canonical/AppMenu/Registrar",
                    nullptr);
                std::cout << "[LegacyRegistrar] Bus name acquired: " << name << std::endl;
            },
            [](GDBusConnection *connection, const gchar *name, gpointer user_data)
            {
                std::cout << "[LegacyRegistrar] Name acquired (handler)" << std::endl;
            },
            [](GDBusConnection *connection, const gchar *name, gpointer user_data)
            {
                std::cout << "[LegacyRegistrar] Failed to acquire bus name: " << name << std::endl;
            },
            this,
            nullptr);
    }

    std::optional<Snnu::Domain::MenuStructure> LegacyRegistrar::GetMenuForWindow(uint32_t windowId)
    {
        std::lock_guard<std::mutex> lock(registry_mutex_);

        auto it = registry_.find(windowId);
        if (it != registry_.end())
        {
            std::cout << "[LegacyRegistrar] Found registered path for window " << windowId << ": " << it->second << std::endl;
            return std::nullopt;
        }

        return std::nullopt;
    }

    gboolean LegacyRegistrar::OnRegisterWindow(SnnuRegistrar *skeleton,
                                               GDBusMethodInvocation *invocation,
                                               guint windowId,
                                               const gchar *menuObjectPath,
                                               gpointer userData)
    {
        auto *self = static_cast<LegacyRegistrar *>(userData);

        self->HandleRegister(windowId, std::string(menuObjectPath));

        snnu_registrar_complete_register_window(skeleton, invocation);
        return TRUE;
    }

    gboolean LegacyRegistrar::OnUnregisterWindow(SnnuRegistrar *skeleton,
                                                 GDBusMethodInvocation *invocation,
                                                 guint windowId,
                                                 gpointer userData)
    {
        auto *self = static_cast<LegacyRegistrar *>(userData);

        self->HandleUnregister(windowId);

        snnu_registrar_complete_unregister_window(skeleton, invocation);
        return TRUE;
    }

    void LegacyRegistrar::HandleRegister(uint32_t windowId, std::string path)
    {
        std::lock_guard<std::mutex> lock(registry_mutex_);
        registry_[windowId] = path;
        std::cout << "[LegacyRegistrar] Registered Window ID: " << windowId << " Path: " << path << std::endl;
    }

    void LegacyRegistrar::HandleUnregister(uint32_t windowId)
    {
        std::lock_guard<std::mutex> lock(registry_mutex_);
        registry_.erase(windowId);
        std::cout << "[LegacyRegistrar] Unregistered Window ID: " << windowId << std::endl;
    }

}