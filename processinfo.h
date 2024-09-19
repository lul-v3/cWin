#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QString>
#include <windows.h> // For the DWORD type

/**
 * @class ProcessInfo
 * @brief Stores information about a process, such as its name, ID, window size, and attributes like TopMost and opacity.
 */
class ProcessInfo {
public:
    #pragma region Constructors and Destructor

    /**
     * @brief Default constructor for initializing the ProcessInfo object.
     */
    ProcessInfo();

    /**
     * @brief Destructor for cleaning up the ProcessInfo object.
     */
    ~ProcessInfo();

    #pragma endregion

    #pragma region Getters and Setters

    /**
     * @brief Retrieves the window title of the process.
     * @return The window title as a QString.
     */
    QString getProcessTitle() const;

    /**
     * @brief Sets the window title of the process.
     * @param title The new window title.
     */
    void setProcessTitle(const QString &title);

    /**
     * @brief Retrieves the TopMost status of the process window.
     * @return The TopMost status as a QString ("Yes" or "No").
     */
    QString getTopMost() const;

    /**
     * @brief Sets the TopMost status of the process window.
     * @param topMost The new TopMost status ("Yes" or "No").
     */
    void setTopMost(const QString &topMost);

    /**
     * @brief Retrieves the name of the process.
     * @return The process name as a QString.
     */
    QString getProcessName() const;

    /**
     * @brief Sets the name of the process.
     * @param name The new process name.
     */
    void setProcessName(const QString &name);

    /**
     * @brief Retrieves the ID of the process.
     * @return The process ID as a DWORD.
     */
    DWORD getProcessId() const;

    /**
     * @brief Sets the process ID.
     * @param id The new process ID.
     */
    void setProcessId(DWORD id);

    /**
     * @brief Retrieves the width of the process window.
     * @return The window width in pixels.
     */
    int getWidth() const;

    /**
     * @brief Sets the width of the process window.
     * @param width The new window width in pixels.
     */
    void setWidth(int width);

    /**
     * @brief Retrieves the height of the process window.
     * @return The window height in pixels.
     */
    int getHeight() const;

    /**
     * @brief Sets the height of the process window.
     * @param height The new window height in pixels.
     */
    void setHeight(int height);

    /**
     * @brief Retrieves the opacity of the process window.
     * @return The window opacity as an integer (0-255).
     */
    int getOpacity() const;

    /**
     * @brief Sets the opacity of the process window.
     * @param opacity The new opacity level (0 for fully transparent, 255 for fully opaque).
     */
    void setOpacity(int opacity);

    #pragma endregion

private:
    #pragma region Member Variables

    QString processTitle;   // Title of the process window
    QString topMost;        // Indicates if the window is TopMost ("Yes"/"No")
    QString processName;    // Name of the process
    DWORD processId;        // ID of the process
    int width;              // Window width in pixels
    int height;             // Window height in pixels
    int opacity;            // Window opacity (0-255)

    #pragma endregion
};

#endif // PROCESSINFO_H
