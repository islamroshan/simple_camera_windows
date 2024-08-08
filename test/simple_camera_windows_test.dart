import 'package:flutter_test/flutter_test.dart';
import 'package:simple_camera_windows/simple_camera_windows.dart';
import 'package:simple_camera_windows/simple_camera_windows_platform_interface.dart';
import 'package:simple_camera_windows/simple_camera_windows_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

// class MockSimpleCameraWindowsPlatform
//     with MockPlatformInterfaceMixin
//     implements SimpleCameraWindowsPlatform {

//   @override
//   Future<String?> getPlatformVersion() => Future.value('42');
// }

void main() {
  final SimpleCameraWindowsPlatform initialPlatform = SimpleCameraWindowsPlatform.instance;

  test('$MethodChannelSimpleCameraWindows is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelSimpleCameraWindows>());
  });

  // test('getPlatformVersion', () async {
  //   SimpleCameraWindows simpleCameraWindowsPlugin = SimpleCameraWindows();
  //   MockSimpleCameraWindowsPlatform fakePlatform = MockSimpleCameraWindowsPlatform();
  //   SimpleCameraWindowsPlatform.instance = fakePlatform;

  //   expect(await simpleCameraWindowsPlugin.getPlatformVersion(), '42');
  // });
}
