import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'simple_camera_windows_method_channel.dart';

abstract class SimpleCameraWindowsPlatform extends PlatformInterface {
  SimpleCameraWindowsPlatform() : super(token: _token);

  static final Object _token = Object();

  static SimpleCameraWindowsPlatform _instance = MethodChannelSimpleCameraWindows();

  static SimpleCameraWindowsPlatform get instance => _instance;

  static set instance(SimpleCameraWindowsPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<void> initializeCamera() {
    throw UnimplementedError('initializeCamera() has not been implemented.');
  }

  Future<void> startCamera() {
    throw UnimplementedError('startCamera() has not been implemented.');
  }

  Future<void> stopCamera() {
    throw UnimplementedError('stopCamera() has not been implemented.');
  }

  Future<Uint8List?> captureFrame() {
    throw UnimplementedError('captureFrame() has not been implemented.');
  }
}
