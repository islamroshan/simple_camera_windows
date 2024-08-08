import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'simple_camera_windows_platform_interface.dart';

/// An implementation of [SimpleCameraWindowsPlatform] that uses method channels.
class MethodChannelSimpleCameraWindows extends SimpleCameraWindowsPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('simple_camera_windows');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
