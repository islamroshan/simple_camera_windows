import 'package:flutter/services.dart';
import 'simple_camera_windows_platform_interface.dart';

/// An implementation of [SimpleCameraWindowsPlatform] that uses method channels.
class MethodChannelSimpleCameraWindows extends SimpleCameraWindowsPlatform {
  /// The method channel used to interact with the native platform.
  final methodChannel = const MethodChannel('dev.spantik/camera');

  @override
  Future<void> initializeCamera() async {
    try {
      await methodChannel.invokeMethod('initializeCamera');
    } on PlatformException catch (e) {
      throw Exception("Failed to initialize camera: '${e.message}'");
    }
  }

  @override
  Future<void> startCamera() async {
    try {
      await methodChannel.invokeMethod('startCamera');
    } on PlatformException catch (e) {
      throw Exception("Failed to start camera: '${e.message}'");
    }
  }

  @override
  Future<void> stopCamera() async {
    try {
      await methodChannel.invokeMethod('stopCamera');
    } on PlatformException catch (e) {
      throw Exception("Failed to stop camera: '${e.message}'");
    }
  }

  @override
  Future<Uint8List?> captureFrame() async {
    try {
      await methodChannel.invokeMethod('captureFrame');
      Uint8List frame = await methodChannel.invokeMethod('getCapturedFrame');
      return frame;
    } on PlatformException catch (e) {
      throw Exception("Failed to capture frame: '${e.message}'");
    }
  }
}
