
import 'simple_camera_windows_platform_interface.dart';

class SimpleCameraWindows {
  Future<String?> getPlatformVersion() {
    return SimpleCameraWindowsPlatform.instance.getPlatformVersion();
  }
}
