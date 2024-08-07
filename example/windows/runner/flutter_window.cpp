#include "flutter_window.h"
#include <dshow.h>
#include <iostream>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include <thread>
#include <atomic>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <windows.h>
#include "flutter/generated_plugin_registrant.h"
#include "resource.h"

using namespace std;
using namespace cv;

namespace {
    Mat img;
    Mat capturedFrame;
    VideoCapture cam;
    atomic<bool> isCameraRunning(false);
    const string windowName = "Camera Preview";
    thread cameraThread;

    void SetCameraWindowIcon(HWND hwnd) {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CAMERA_ICON));
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
}

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
    : project_(project) {}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
    if (!Win32Window::OnCreate()) {
        return false;
    }

    RECT frame = GetClientArea();
    flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
        frame.right - frame.left, frame.bottom - frame.top, project_);
    if (!flutter_controller_->engine() || !flutter_controller_->view()) {
        return false;
    }

    RegisterPlugins(flutter_controller_->engine());
    SetChildContent(flutter_controller_->view()->GetNativeWindow());
    flutter_controller_->engine()->SetNextFrameCallback([&]() {
        this->Show();
        });
    flutter_controller_->ForceRedraw();

    flutter::MethodChannel<> channel(
        flutter_controller_->engine()->messenger(), "dev.spantik/camera",
        &flutter::StandardMethodCodec::GetInstance());
    channel.SetMethodCallHandler(
        [](const flutter::MethodCall<>& call,
            std::unique_ptr<flutter::MethodResult<>> result) {
                if (call.method_name() == "initializeCamera") {
                    InitializeCamera();
                    result->Success();
                }
                else if (call.method_name() == "startCamera") {
                    if (StartCamera()) {
                        result->Success();
                    }
                    else {
                        result->Error("CAMERA_ERROR", "Failed to start the camera.");
                    }
                }
                else if (call.method_name() == "stopCamera") {
                    StopCamera();
                    result->Success();
                }
                else if (call.method_name() == "captureFrame") {
                    CaptureFrame();
                    result->Success();
                }
                else if (call.method_name() == "getCapturedFrame") {
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
        });

    return true;
}

void FlutterWindow::OnDestroy() {
    if (isCameraRunning) {
        StopCamera();
    }
    if (flutter_controller_) {
        flutter_controller_ = nullptr;
    }
    Win32Window::OnDestroy();
}

LRESULT FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
    WPARAM const wparam, LPARAM const lparam) noexcept {
    if (flutter_controller_) {
        std::optional<LRESULT> result =
            flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam, lparam);
        if (result) {
            return *result;
        }
    }

    switch (message) {
    case WM_FONTCHANGE:
        flutter_controller_->engine()->ReloadSystemFonts();
        break;
    }

    return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
