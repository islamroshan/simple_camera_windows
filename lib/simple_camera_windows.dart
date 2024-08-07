import 'package:flutter/services.dart';

import 'simple_camera_windows_platform_interface.dart';

class SimpleCameraWindows {
  Future<void> initializeCamera() {
    return SimpleCameraWindowsPlatform.instance.initializeCamera();
  }

  Future<void> startCamera() {
    return SimpleCameraWindowsPlatform.instance.startCamera();
  }

  Future<void> stopCamera() {
    return SimpleCameraWindowsPlatform.instance.stopCamera();
  }

  Future<Uint8List?> captureFrame() {
    return SimpleCameraWindowsPlatform.instance.captureFrame();
  }
}
