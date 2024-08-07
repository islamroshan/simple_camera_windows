# simple_camera_windows

A simple plugin with basic camera API to access and capture picture from the camera on windows platform

## Getting Started

This project is equiped with the basic activities to access general functionalities of camera on windows platform using some defined API's using Flutter Method Channel.

This package is built with non granular approach, so make sure if the package meets your requirements before implementation because this package is above the rope of fray, due to its limited access to the camera functionality.

Here are the supported functions exposed by the simple_camera_windows package.

* `startCamera()`: Aims to access and display the camera preview inside a new window.

* `stopCamera()`: After done with the camera, this function is used to terminate the camera access.

* `captureFrame()`: While accessing the camera, call this function to capture a frame, the function returns the frame with typed as `Uint8List?`. you can then use the response as per your needs, you can convert the response into `Base64` or `Image`, please check for the detail instruction to convert `Uint8List` to `Base64` or `Image`.
## Project structure

This template uses the following structure:

* `lib`: Contains the Dart code that defines the API of the plugin, and which
  calls into the native code using `MethodChannels`.

* platform folders (`linux`, `windows`, etc.): Contains the build files
  for building and bundling the native code library with the platform application.

## Building and bundling native code

The `pubspec.yaml` specifies plugins as follows:

```yaml
  plugin:
    platforms:
      linux:
        pluginClass: SimpleCameraWindowsPlugin
      windows:
        pluginClass: SimpleCameraWindowsPluginCApi
```

This configuration invokes the native build for the various target platforms
and bundles the binaries in Flutter applications..

A plugin can have both FFI and method channels:

```yaml
  plugin:
    platforms:
      some_platform:
        pluginClass: SomeName
        ffiPlugin: true
```

The native build systems that are invoked by FFI (and method channel) plugins are:
* For Windows: CMake.
  * See the documentation in windows/CMakeLists.txt.

## Invoking native code

Use `MethodChannel` provided by flutter to invoke native functions defined in the files.

example: 
    `final methodChannel = const MethodChannel('dev.spantik/camera');`

then:
`await methodChannel.invokeMethod('initializeCamera');`

## Package Documentation

For detail documentation follow these steps:
 - Clone the project or download the zip file from the Github repository.
 - After opening the root directory of the project, Navigate into `/doc`
 - Execute the `index.html` file inside `/doc/api/index.html`

## Flutter help

For help getting started with Flutter, view our
[online documentation](https://flutter.dev/docs), which offers tutorials,
samples, guidance on mobile development, and a full API reference.

