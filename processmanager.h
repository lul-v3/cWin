#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QString>
#include <windows.h>
#include <functional>
#include "processinfo.h"

/**
 * @brief The ProcessManager class manages operations on system processes, such as fetching details,
 *        modifying window attributes, and executing window commands.
 */
class ProcessManager {
public:
    ProcessManager();   // Constructor
    ~ProcessManager();  // Destructor

    #pragma region Process Details

    /**
     * @brief Fetches process details (such as name, ID) based on the provided process name or ID.
     * @param processNameOrId The name or ID of the process.
     * @param logCallback Callback function to handle logging messages.
     */
    void getProcessDetails(const QString &processNameOrId, std::function<void(const QString &)> logCallback);

    /**
     * @brief Returns the collected process information.
     * @return A ProcessInfo object containing details about the current process.
     */
    ProcessInfo getProcessInfo();

    #pragma endregion

    #pragma region Window Modifications

    /**
     * @brief Sets the window title for the given process.
     * @param title The new window title.
     * @param processID The ID of the process whose window title should be set.
     */
    void SetProcessWindowTitle(const QString& title, DWORD processID);

    /**
     * @brief Sets the TopMost flag for the process window, determining whether it stays above other windows.
     * @param topMost True to make the window TopMost, false otherwise.
     * @param processID The ID of the process to modify.
     */
    void SetProcessWindowTopMost(bool topMost, DWORD processID);

    /**
     * @brief Sets the size of the process window.
     * @param width The desired window width.
     * @param height The desired window height.
     */
    void SetProcessWindowSize(int width, int height);

    /**
     * @brief Sets the transparency (opacity) of the process window.
     * @param value The transparency level (0 = fully transparent, 255 = fully opaque).
     */
    void SetProcessWindowTransparency(int value);

    #pragma endregion

    #pragma region Window Commands

    /**
     * @brief Terminates (kills) the process window.
     */
    void KillProcessWindow();

    /**
     * @brief Maximizes the process window.
     */
    void MaximizeProcessWindow();

    /**
     * @brief Minimizes the process window.
     */
    void MinimizeProcessWindow();

    /**
     * @brief Brings the process window into focus.
     */
    void FocusProcessWindow();

    #pragma endregion

private:
    ProcessInfo processInfo;  // Stores current process information

    #pragma region Process and Window Helpers

    /**
     * @brief Finds the process ID by the process name.
     * @param processName The name of the process.
     * @return The ID of the found process.
     */
    DWORD findProcessIdByName(const QString &processName);

    /**
     * @brief Finds the window handle (HWND) by the process ID.
     * @param processId The process ID.
     * @return The window handle associated with the process.
     */
    HWND findWindowByProcessId(DWORD processId);

    /**
     * @brief Retrieves window information and updates the processInfo object.
     * @param hWnd Handle to the process window.
     */
    void retrieveWindowInfo(HWND hWnd);

    #pragma endregion

    #pragma region Utility

    std::function<void(const QString &)> logCallback;  // Callback for logging messages

    /**
     * @brief Normalizes the process name (removes extensions, converts to lowercase, etc.).
     * @param processName The name of the process.
     * @return A normalized process name.
     */
    QString normalizeProcessName(const QString& processName) const;

    #pragma endregion
};

#endif // PROCESSMANAGER_H
