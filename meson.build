project('snnu-global-menu', ['c', 'cpp'],
  version: '0.1.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++20'
  ]
)

gnome = import('gnome')

deps = [
  dependency('glib-2.0'),
  dependency('gio-2.0'),
  dependency('gio-unix-2.0'),
  dependency('libwnck-3.0')
]

registrar_generated = gnome.gdbus_codegen(
  'legacy_registrar_generated',
  sources: 'data/dbus-interfaces/com.canonical.AppMenu.Registrar.xml',
  interface_prefix: 'com.canonical.AppMenu.',
  namespace: 'Snnu',
  object_manager: false
)

dbusmenu_generated = gnome.gdbus_codegen(
  'dbusmenu_generated',
  sources: 'data/dbus-interfaces/com.canonical.dbusmenu.xml',
  interface_prefix: 'com.canonical.',
  namespace: 'Snnu',
  object_manager: false
)

sources = [
  'src/main.cpp',
  'src/infra/dbus/LegacyRegistrar.cpp',
  registrar_generated,
  dbusmenu_generated 
]

executable('snnu-backend',
  sources,
  dependencies: deps,
  install: true
)