//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <simple_camera_windows/simple_camera_windows_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) simple_camera_windows_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "SimpleCameraWindowsPlugin");
  simple_camera_windows_plugin_register_with_registrar(simple_camera_windows_registrar);
}
