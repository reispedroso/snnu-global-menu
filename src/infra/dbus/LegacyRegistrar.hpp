#pragma once

#include <map>
#include <string>
#include <mutex>
#include <memory>
#include "legacy_registrar_generated.h"
#include "../../ports/IMenuProvider.hpp"

namespace Snnu::Infra::DBus {

    class LegacyRegistrar : public Snnu::Ports::IMenuProvider {
    public:
        LegacyRegistrar();
        ~LegacyRegistrar() override;

        void Initialize();

        std::optional<Snnu::Domain::MenuStructure> GetMenuForWindow(uint32_t windowId) override;

    private:
        static gboolean OnRegisterWindow(SnnuRegistrar* skeleton,
                                       GDBusMethodInvocation* invocation,
                                       guint windowId,
                                       const gchar* menuObjectPath,
                                       gpointer userData);

        
        static gboolean OnUnregisterWindow(SnnuRegistrar* skeleton,
                                         GDBusMethodInvocation* invocation,
                                         guint windowId,
                                         gpointer userData);

        void HandleRegister(uint32_t windowId, std::string path);
        void HandleUnregister(uint32_t windowId);

        SnnuRegistrar* dbus_skeleton_;
        guint owner_id_;
        
        std::map<uint32_t, std::string> registry_;
        std::mutex registry_mutex_;
    };

}