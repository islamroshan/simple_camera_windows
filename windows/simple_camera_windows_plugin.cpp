
#include "simple_camera_windows_plugin.h" // This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include <dshow.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

namespace simple_camera_windows {

    Mat img;
    Mat capturedFrame;
    VideoCapture cam;
    atomic<bool> isCameraRunning(false);
    const string windowName = "Camera Preview";
    thread cameraThread;

    void SetCameraWindowIcon(HWND hwnd) {

        HICON hIcon = LoadIcon(GetModuleHandle(NULL), NULL);
        if (hIcon) {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        else {
            cerr << "Error: Could not load icon." << endl;
        }
    }

    HWND GetCameraWindowHandle() {
        int len;
        int stringLen = (int)windowName.length() + 1;
        len = MultiByteToWideChar(CP_UTF8, 0, windowName.c_str(), stringLen, 0, 0);
        wchar_t* wString = new wchar_t[len];
        MultiByteToWideChar(CP_UTF8, 0, windowName.c_str(), stringLen, wString, len);
        HWND hwnd = FindWindow(NULL, wString);
        delete[] wString;
        return hwnd;
    }

    void PositionWindowAtCenter(HWND hwnd) {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int x = (screenWidth - windowWidth) / 2;
        int y = (screenHeight - windowHeight) / 2;
        SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    void CaptureFrame() {
        if (!img.empty()) {
            capturedFrame = img.clone();
        }
    }

    std::vector<uchar> GetCapturedFrame() {
        std::vector<uchar> buf;
        if (!capturedFrame.empty()) {
            imencode(".jpg", capturedFrame, buf);
        }
        return buf;
    }

    void CameraLoop() {
        namedWindow(windowName);
        while (isCameraRunning) {
            HWND hwnd = GetCameraWindowHandle();
            SetCameraWindowIcon(hwnd);
            PositionWindowAtCenter(hwnd);
            cam >> img;
            if (!img.empty()) {
                imshow(windowName, img);
                waitKey(30);
            }
            else {
                cerr << "Error: Could not grab a frame." << endl;
            }
        }
        destroyWindow(windowName);
    }

    void InitializeCamera() {
        if (isCameraRunning) {
            cerr << "Camera already running." << endl;
            return;
        }
        namedWindow(windowName);
        HWND hwnd = GetCameraWindowHandle();
        SetCameraWindowIcon(hwnd);
        PositionWindowAtCenter(hwnd);
    }

    bool StartCamera() {
        if (isCameraRunning) {
            cerr << "Camera already running." << endl;
            return false;
        }
        cam.open(0);
        if (!cam.isOpened()) {
            cerr << "Error: Could not open camera." << endl;
            return false;
        }
        cout << "Camera started." << endl;
        isCameraRunning = true;
        cameraThread = thread(CameraLoop);
        return true;
    }

    void StopCamera() {
        if (!isCameraRunning) {
            cerr << "Camera is not running." << endl;
            return;
        }
        isCameraRunning = false;
        if (cameraThread.joinable()) {
            cameraThread.join();
        }
        cam.release();
        cout << "Camera stopped." << endl;
    }


    // static
    void SimpleCameraWindowsPlugin::RegisterWithRegistrar(
        flutter::PluginRegistrarWindows* registrar) {
        auto channel =
            std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
                registrar->messenger(), "simple_camera_windows",
                &flutter::StandardMethodCodec::GetInstance());

        auto plugin = std::make_unique<SimpleCameraWindowsPlugin>();

        channel->SetMethodCallHandler(
            [plugin_pointer = plugin.get()](const auto& call, auto result) {
                plugin_pointer->HandleMethodCall(call, std::move(result));
            });

        registrar->AddPlugin(std::move(plugin));
    }

    SimpleCameraWindowsPlugin::SimpleCameraWindowsPlugin() {}

    SimpleCameraWindowsPlugin::~SimpleCameraWindowsPlugin() {}

    void SimpleCameraWindowsPlugin::HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue>& method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
        if (method_call.method_name().compare("getPlatformVersion") == 0) {
            std::ostringstream version_stream;
            version_stream << "Windows ";
            if (IsWindows10OrGreater()) {
                version_stream << "10+";
            }
            else if (IsWindows8OrGreater()) {
                version_stream << "8";
            }
            else if (IsWindows7OrGreater()) {
                version_stream << "7";
            }
            result->Success(flutter::EncodableValue(version_stream.str()));
        }
        else if (method_call.method_name().compare("initializeCamera") == 0) {
            InitializeCamera();
            result->Success();
        }
        else if (method_call.method_name().compare("startCamera") == 0) {
            if (StartCamera()) {
                result->Success();
            }
            else {
                result->Error("CAMERA_ERROR", "Failed to start the camera.");
            }
        }
        else if (method_call.method_name().compare("stopCamera") == 0) {
            StopCamera();
            result->Success();
        }
        else if (method_call.method_name().compare("captureFrame") == 0) {
            CaptureFrame();
            result->Success();
        }
        else if (method_call.method_name().compare("getCapturedFrame") == 0) {
            std::vector<uchar> frame = GetCapturedFrame();
            if (!frame.empty()) {
                result->Success(frame);
            }
            else {
                result->Error("NO_FRAME", "No frame captured.");
            }
        }
        else {
            result->NotImplemented();
        }
    }

}  // namespace simple_camera_windows