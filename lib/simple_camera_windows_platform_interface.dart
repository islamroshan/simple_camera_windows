import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'simple_camera_windows_method_channel.dart';

abstract class SimpleCameraWindowsPlatform extends PlatformInterface {
  /// Constructs a SimpleCameraWindowsPlatform.
  SimpleCameraWindowsPlatform() : super(token: _token);

  static final Object _token = Object();

  static SimpleCameraWindowsPlatform _instance = MethodChannelSimpleCameraWindows();

  /// The default instance of [SimpleCameraWindowsPlatform] to use.
  ///
  /// Defaults to [MethodChannelSimpleCameraWindows].
  static SimpleCameraWindowsPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [SimpleCameraWindowsPlatform] when
  /// they register themselves.
  static set instance(SimpleCameraWindowsPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<void> initializeCamera() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<void> startCamera() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<void> stopCamera() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<Uint8List?> captureFrame() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
