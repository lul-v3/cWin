#include "processinfo.h"

#pragma region Constructor and Destructor

/**
 * @brief Constructs the ProcessInfo object with default values.
 *        Initializes processId to 0 and sets default values for process attributes.
 */
ProcessInfo::ProcessInfo() : processId(0) {
    // Initialize member variables
    processTitle = "";
    topMost = "No";  // By default, the window is not TopMost
    processName = "";
    width = 0;
    height = 0;
    opacity = 255;   // Default opacity is fully opaque
}

/**
 * @brief Destructor for ProcessInfo.
 */
ProcessInfo::~ProcessInfo() {}

#pragma endregion

#pragma region Getters and Setters

/**
 * @brief Returns the window title of the process.
 * @return The window title as a QString.
 */
QString ProcessInfo::getProcessTitle() const {
    return processTitle;
}

/**
 * @brief Sets the window title of the process.
 * @param title The new window title.
 */
void ProcessInfo::setProcessTitle(const QString &title) {
    processTitle = title;
}

/**
 * @brief Returns the TopMost status of the window.
 * @return "Yes" if the window is TopMost, "No" otherwise.
 */
QString ProcessInfo::getTopMost() const {
    return topMost;
}

/**
 * @brief Sets the TopMost status of the window.
 * @param topMostValue "Yes" to make the window TopMost, "No" otherwise.
 */
void ProcessInfo::setTopMost(const QString &topMostValue) {
    topMost = topMostValue;
}

/**
 * @brief Returns the process name.
 * @return The process name as a QString.
 */
QString ProcessInfo::getProcessName() const {
    return processName;
}

/**
 * @brief Sets the process name.
 * @param name The new process name.
 */
void ProcessInfo::setProcessName(const QString &name) {
    processName = name;
}

/**
 * @brief Returns the process ID.
 * @return The process ID as a DWORD.
 */
DWORD ProcessInfo::getProcessId() const {
    return processId;
}

/**
 * @brief Sets the process ID.
 * @param id The new process ID.
 */
void ProcessInfo::setProcessId(DWORD id) {
    processId = id;
}

/**
 * @brief Returns the width of the process window.
 * @return The window width in pixels.
 */
int ProcessInfo::getWidth() const {
    return width;
}

/**
 * @brief Sets the width of the process window.
 * @param w The new width in pixels.
 */
void ProcessInfo::setWidth(int w) {
    width = w;
}

/**
 * @brief Returns the height of the process window.
 * @return The window height in pixels.
 */
int ProcessInfo::getHeight() const {
    return height;
}

/**
 * @brief Sets the height of the process window.
 * @param h The new height in pixels.
 */
void ProcessInfo::setHeight(int h) {
    height = h;
}

/**
 * @brief Returns the window opacity.
 * @return Opacity value (0 = fully transparent, 255 = fully opaque).
 */
int ProcessInfo::getOpacity() const {
    return opacity;
}

/**
 * @brief Sets the window opacity.
 * @param newOpacity The new opacity value (0 = fully transparent, 255 = fully opaque).
 */
void ProcessInfo::setOpacity(int newOpacity) {
    opacity = newOpacity;
}

#pragma endregion
