import 'package:flutter/material.dart';
import 'dart:async';
import 'package:flutter/services.dart';
import 'package:simple_camera_windows/simple_camera_windows.dart';
void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: CameraScreen(),
    );
  }
}

class CameraScreen extends StatefulWidget {
  const CameraScreen({super.key});

  @override
  State<CameraScreen> createState() => _CameraScreenState();
}

class _CameraScreenState extends State<CameraScreen> {
  Uint8List? capturedFrame;
  final _simpleCameraWindowsPlugin = SimpleCameraWindows();

  @override
  void initState() {
    super.initState();
  }

  Future<void> initializeCamera() async {
    try {
      await _simpleCameraWindowsPlugin.initializeCamera();
    } catch (e) {
      showSnackBar(e.toString());
    }
  }

  Future<void> startCamera() async {
    try {
      await _simpleCameraWindowsPlugin.startCamera();
    } catch (e) {
      showSnackBar(e.toString());
    }
  }

  Future<void> stopCamera() async {
    try {
      await _simpleCameraWindowsPlugin.stopCamera();
    } catch (e) {
      showSnackBar(e.toString());
    }
  }

  Future<void> captureFrame() async {
    try {
      Uint8List? frame = await _simpleCameraWindowsPlugin.captureFrame();
      setState(() {
        capturedFrame = frame;
      });
    } catch (e) {
      showSnackBar(e.toString());
    }
  }

  void showSnackBar(String message) {
    if (mounted) {
      ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(message)));
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Simple Camera For Windows'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            ElevatedButton(
              onPressed: startCamera,
              child: const Text('Start Camera'),
            ),
            ElevatedButton(
              onPressed: captureFrame,
              child: const Text('Capture Frame'),
            ),
            ElevatedButton(
              onPressed: stopCamera,
              child: const Text('Stop Camera'),
            ),
            const SizedBox(height: 20),
            capturedFrame != null
                ? Container(
                    width: 300,
                    height: 300,
                    decoration: BoxDecoration(
                      border: Border.all(color: Colors.blueAccent),
                    ),
                    child: Image.memory(capturedFrame!),
                  )
                : Container(
                    width: 300,
                    height: 300,
                    decoration: BoxDecoration(
                      border: Border.all(color: Colors.blueAccent),
                    ),
                    child: const Center(child: Text('No frame captured')),
                  ),
          ],
        ),
      ),
    );
  }
}
