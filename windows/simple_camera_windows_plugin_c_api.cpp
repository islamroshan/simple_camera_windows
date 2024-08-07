#include "include/simple_camera_windows/simple_camera_windows_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "simple_camera_windows_plugin.h"

void SimpleCameraWindowsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  simple_camera_windows::SimpleCameraWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
