#include "processmanager.h"
#include <tlhelp32.h>
#include <algorithm>

// Constructor and Destructor
ProcessManager::ProcessManager() : logCallback(nullptr) {}
ProcessManager::~ProcessManager() {}

// Utility: Normalize the process name for case-insensitive comparison
QString ProcessManager::normalizeProcessName(const QString& processName) const {
    QString normalizedName = processName;
    if (normalizedName.endsWith(".exe", Qt::CaseInsensitive)) {
        normalizedName.chop(4);         // Remove ".exe" extension
    }
    return normalizedName.toLower();    // Convert to lowercase
}

// Find process ID by process name
DWORD ProcessManager::findProcessIdByName(const QString &processName) {
    QString normalizedName = normalizeProcessName(processName);

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            QString currentProcessName = QString::fromWCharArray(processEntry.szExeFile);
            QString normalizedCurrentName = normalizeProcessName(currentProcessName);

            if (normalizedCurrentName == normalizedName) {
                CloseHandle(snapshot);
                processInfo.setProcessName(currentProcessName); // Save process name
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return 0;
}

// Find window handle by process ID
HWND ProcessManager::findWindowByProcessId(DWORD processId) {
    struct EnumWindowsData {
        DWORD processId;
        HWND hWnd;
    } data = { processId, NULL };

    EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        EnumWindowsData* data = reinterpret_cast<EnumWindowsData*>(lParam);
        DWORD windowProcessId;
        GetWindowThreadProcessId(hWnd, &windowProcessId);
        if (windowProcessId == data->processId && IsWindowVisible(hWnd)) {
            data->hWnd = hWnd;
            return FALSE;   // Stop enumerating if window is found
        }
        return TRUE;        // Continue enumerating
    }, reinterpret_cast<LPARAM>(&data));

    return data.hWnd;       // Return window handle (NULL if none found)
}

// Retrieve process details and log information
void ProcessManager::getProcessDetails(const QString &processNameOrId, std::function<void(const QString &)> logCallback) {
    this->logCallback = logCallback;

    bool isId;
    DWORD processId = processNameOrId.toUInt(&isId);        // Try interpreting input as an ID

    if (isId) {
        processInfo.setProcessId(processId);                // Set process ID
        processInfo.setProcessName("");                     // Clear process name if ID is used
    } else {
        processId = findProcessIdByName(processNameOrId);   // Try finding process ID by name
        processInfo.setProcessId(processId);                // Store found process ID
    }

    logCallback(QString("Found process ID: %1").arg(processId));

    if (processId != 0) {
        HWND hWnd = findWindowByProcessId(processId);
        retrieveWindowInfo(hWnd);                           // Get window title and TopMost status
    } else {
        logCallback("Process not found");
    }
}

// Retrieve window information (title, TopMost, size, opacity)
void ProcessManager::retrieveWindowInfo(HWND hWnd) {
    if (hWnd == NULL) {
        logCallback("Window handle not found");
        return;
    }

    // Get window title
    TCHAR windowTitle[256];
    GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(TCHAR));
    processInfo.setProcessTitle(QString::fromWCharArray(windowTitle));

    // Check if window is "TopMost"
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
    bool isTopMost = (style & WS_EX_TOPMOST) != 0;
    processInfo.setTopMost(isTopMost ? "Yes" : "No");

    // Get window size
    RECT rect;
    if (GetWindowRect(hWnd, &rect)) {
        processInfo.setWidth(rect.right - rect.left);
        processInfo.setHeight(rect.bottom - rect.top);
    } else {
        logCallback("Failed to get window size");
    }

    // Get current window opacity
    BYTE alpha = 255;
    if (GetLayeredWindowAttributes(hWnd, NULL, &alpha, NULL)) {
        processInfo.setOpacity(static_cast<int>(alpha));
    }

    logCallback("Window information retrieved");
}

// Set the process window title
void ProcessManager::SetProcessWindowTitle(const QString& title, DWORD processID) {
    HWND hWnd = findWindowByProcessId(processID);

    if (hWnd != NULL) {
        SetWindowText(hWnd, title.toStdWString().c_str());
        logCallback(QString("Window title changed to: %1").arg(title));
    } else {
        logCallback("Window handle not found, cannot change window title.");
    }
}

// Set or remove TopMost status for a process window
void ProcessManager::SetProcessWindowTopMost(bool topMost, DWORD processID) {
    HWND hWnd = findWindowByProcessId(processID);

    if (hWnd != NULL) {
        if (topMost) {
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            logCallback("Window set to topmost.");
        } else {
            SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            logCallback("Window removed from topmost.");
        }
    } else {
        logCallback("Window handle not found, cannot change topmost status.");
    }
}

// Set the process window size
void ProcessManager::SetProcessWindowSize(int width, int height) {
    HWND hWnd = findWindowByProcessId(processInfo.getProcessId());
    if (hWnd != NULL) {
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOACTIVATE);
        logCallback(QString("Window size set to %1x%2").arg(width).arg(height));
    } else {
        logCallback("Window handle not found");
    }
}

// Set the process window transparency (opacity)
void ProcessManager::SetProcessWindowTransparency(int value) {
    HWND hWnd = findWindowByProcessId(processInfo.getProcessId());
    if (hWnd != NULL) {
        BYTE alpha = static_cast<BYTE>(std::clamp(value, 0, 255));
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
        logCallback(QString("Window opacity set to: %1").arg(value));
    } else {
        logCallback("Window handle not found");
    }
}

// Kill the process
void ProcessManager::KillProcessWindow() {
    DWORD processID = processInfo.getProcessId();
    if (processID != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            logCallback("Process killed");
        } else {
            logCallback("Failed to open process for termination");
        }
    } else {
        logCallback("Process ID not set");
    }
}

// Maximize the process window
void ProcessManager::MaximizeProcessWindow() {
    HWND hWnd = findWindowByProcessId(processInfo.getProcessId());
    if (hWnd != NULL) {
        ShowWindow(hWnd, SW_MAXIMIZE);
        logCallback("Window maximized");
    } else {
        logCallback("Window handle not found");
    }
}

// Minimize the process window
void ProcessManager::MinimizeProcessWindow() {
    HWND hWnd = findWindowByProcessId(processInfo.getProcessId());
    if (hWnd != NULL) {
        ShowWindow(hWnd, SW_MINIMIZE);
        logCallback("Window minimized");
    } else {
        logCallback("Window handle not found");
    }
}

// Focus the process window
void ProcessManager::FocusProcessWindow() {
    HWND hWnd = findWindowByProcessId(processInfo.getProcessId());
    if (hWnd != NULL) {
        if (IsIconic(hWnd)) {
            ShowWindow(hWnd, SW_RESTORE); // Restore if minimized
        }
        SetForegroundWindow(hWnd);
        SetFocus(hWnd);
        logCallback("Window focused");
    } else {
        logCallback("Window handle not found");
    }
}

// Return the current process information
ProcessInfo ProcessManager::getProcessInfo() {
    return processInfo;
}
