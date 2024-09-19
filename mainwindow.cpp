#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <windows.h>

//#region Constructor and Destructor

/**
 * Main constructor for MainWindow class.
 * Initializes the UI components, sets up the options for process management,
 * and connects the necessary UI events to their corresponding slots.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), isTopMost(false)
{
    ui->setupUi(this);

    // Initially, hide the process options group box
    ui->gbProcessOptions->setVisible(false);

    // Add selection options for process search
    ui->cbProcessSearchType->addItem("Search by Process Name");
    ui->cbProcessSearchType->addItem("Search by Process ID (WIP)");

    // Add command options for process window management
    ui->cbProcessWindowCommands->addItem("Select a Command...");
    ui->cbProcessWindowCommands->addItem("KILL");
    ui->cbProcessWindowCommands->addItem("MAXIMIZE");
    ui->cbProcessWindowCommands->addItem("MINIMIZE");
    ui->cbProcessWindowCommands->addItem("FOCUS");

    // Connect buttons and UI elements with respective slots
    connect(ui->btnGetProcess, &QPushButton::clicked, this, &MainWindow::onBtnGetProcess_Clicked);
    connect(ui->btnExecuteProcessCommand, &QPushButton::clicked, this, &MainWindow::onBtnExecuteProcessCommand_Clicked);
    connect(ui->cbProcessTopMost, &QCheckBox::stateChanged, this, &MainWindow::onCbProcessTopMost_CheckedChanged);
    connect(ui->txtProcessTitle, &QLineEdit::textChanged, this, &MainWindow::onTxtProcessTitle_TextChanged);
    connect(ui->aCWinTopMost, &QAction::changed, this, &MainWindow::onCWinTopMost_Changed);
    connect(ui->sbProcessWindowHeight, &QSpinBox::valueChanged, this, &MainWindow::onSbProcessWindowSize_Changed);
    connect(ui->sbProcessWindowWidth, &QSpinBox::valueChanged, this, &MainWindow::onSbProcessWindowSize_Changed);
    connect(ui->sbProcessWindowTransparency, &QSpinBox::valueChanged, this, &MainWindow::onSbProcessWindowTransparency_Changed);

    // Resize the main window to appropriate dimensions
    this->resize(540, 535);
}

/**
 * Destructor for MainWindow class.
 * Cleans up the UI components.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

//#endregion

//#region Slot Implementations

/**
 * Slot function called when the "TopMost" checkbox is checked or unchecked.
 * Updates the TopMost state of the selected process window.
 */
void MainWindow::onCbProcessTopMost_CheckedChanged()
{
    bool TopMost = ui->cbProcessTopMost->isChecked();
    processManager.SetProcessWindowTopMost(TopMost, info.getProcessId());
}

/**
 * Slot function triggered when the process title text is changed.
 * Updates the window title of the target process.
 */
void MainWindow::onTxtProcessTitle_TextChanged()
{
    QString Title = ui->txtProcessTitle->text();
    processManager.SetProcessWindowTitle(Title, info.getProcessId());
}

/**
 * Slot function called when the TopMost status of the MainWindow itself is changed.
 * Toggles the MainWindow to stay on top of other windows if selected.
 */
void MainWindow::onCWinTopMost_Changed()
{
    isTopMost = ui->aCWinTopMost->isChecked();

    if (isTopMost) {
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        this->show(); // Apply the updated window flags
    } else {
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->show(); // Apply the updated window flags
    }
}

/**
 * Slot function to handle changes in process window transparency.
 * Sets the transparency (opacity) level for the process window.
 */
void MainWindow::onSbProcessWindowTransparency_Changed()
{
    int value = ui->sbProcessWindowTransparency->value();
    processManager.SetProcessWindowTransparency(value);
}

/**
 * Slot function called when the "Get Process" button is clicked.
 * Retrieves details about the specified process (by name or ID) and updates the UI accordingly.
 */
void MainWindow::onBtnGetProcess_Clicked()
{
    QString processNameOrId = ui->txtProcessName->text();
    processManager.getProcessDetails(processNameOrId, [this](const QString &logMessage) {
        Log(logMessage); // Display logs in the UI
    });

    updateProcessDetails();
    ShowOrHideProcessOptions();
}

/**
 * Slot function called when the "Execute Command" button is clicked.
 * Executes a specific window command (KILL, MAXIMIZE, MINIMIZE, FOCUS) on the selected process window.
 */
void MainWindow::onBtnExecuteProcessCommand_Clicked()
{
    int command = ui->cbProcessWindowCommands->currentIndex();
    QString msg;

    switch(command)
    {
    case 0:
        Log("Invalid command!"); // No valid command selected
        break;
    case 1:
        msg = QString("Execute command -> KILL, Target -> %1(PID: %2)").arg(info.getProcessName()).arg(info.getProcessId());
        Log(msg);
        processManager.KillProcessWindow();
        break;
    case 2:
        msg = QString("Execute command -> MAXIMIZE, Target -> %1(PID: %2)").arg(info.getProcessName()).arg(info.getProcessId());
        Log(msg);
        processManager.MaximizeProcessWindow();
        break;
    case 3:
        msg = QString("Execute command -> MINIMIZE, Target -> %1(PID: %2)").arg(info.getProcessName()).arg(info.getProcessId());
        Log(msg);
        processManager.MinimizeProcessWindow();
        break;
    case 4:
        msg = QString("Execute command -> FOCUS, Target -> %1(PID: %2)").arg(info.getProcessName()).arg(info.getProcessId());
        Log(msg);
        processManager.FocusProcessWindow();
        break;
    default:
        Log("Invalid command!"); // Invalid command
        break;
    }
}

/**
 * Slot function to handle changes in the process window size.
 * Updates the window size of the target process when height or width is modified.
 */
void MainWindow::onSbProcessWindowSize_Changed()
{
    int height = ui->sbProcessWindowHeight->value();
    int width = ui->sbProcessWindowWidth->value();

    processManager.SetProcessWindowSize(height, width);
}

//#endregion

//#region Helper Methods

/**
 * Shows or hides the process options based on whether valid process details were retrieved.
 */
void MainWindow::ShowOrHideProcessOptions()
{
    QString procName = info.getProcessName();
    DWORD procID = info.getProcessId();

    // Check if a valid process name and ID are present
    if (!procName.trimmed().isEmpty() && procID != 0)
    {
        QString title = QString("%1 (ID: %2)").arg(procName).arg(procID);
        ui->gbProcessOptions->setTitle(title);
        ui->gbProcessOptions->setVisible(true); // Show the options
    }
    else
    {
        ui->gbProcessOptions->setVisible(false); // Hide the options
    }
}

/**
 * Updates the process-related details in the UI such as process name, window title, window size, etc.
 */
void MainWindow::updateProcessDetails()
{
    info = processManager.getProcessInfo();

    // Update process title and name
    ui->txtProcessTitle->setText(info.getProcessTitle());
    ui->txtProcessName->setText(info.getProcessName());

    // Update the TopMost checkbox
    ui->cbProcessTopMost->setChecked(info.getTopMost() == "Yes");

    // Update window size and transparency
    ui->sbProcessWindowHeight->setValue(info.getHeight());
    ui->sbProcessWindowWidth->setValue(info.getWidth());
    ui->sbProcessWindowTransparency->setValue(info.getOpacity());
}

/**
 * Logs messages in the UI's log field with a timestamp.
 */
void MainWindow::Log(const QString& message)
{
    // Get current date and time in format "yyyy-MM-dd HH:mm:ss"
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // Append formatted log message to the log display
    ui->txtLog->appendPlainText(QString("[%1]: %2").arg(datetime, message));
}

//#endregion
