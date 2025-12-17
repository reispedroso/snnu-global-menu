#include <glib.h>
#include <iostream>
#include <memory>
#include "infra/dbus/LegacyRegistrar.hpp"

int main() {
    std::cout << "[SnnuBackend] Starting service..." << std::endl;

    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);

    auto registrar = std::make_unique<Snnu::Infra::DBus::LegacyRegistrar>();
    registrar->Initialize();

    std::cout << "[SnnuBackend] DBus Registrar initialized. Waiting for apps..." << std::endl;
    std::cout << "[SnnuBackend] Event loop running. Press Ctrl+C to stop." << std::endl;
    
    g_main_loop_run(loop);

    g_main_loop_unref(loop);

    return 0;
}