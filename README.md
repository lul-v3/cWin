# cWin
cWin is a C++ application built with Qt for managing and manipulating windows of running processes on Windows systems. This project is inspired by the "[auWin](https://www.github.com/dodaucy/auwin/)" project by [dodaucy](https://www.github.com/dodaucy), which was originally implemented in AutoIt3. cWin provides similar functionality using modern C++ and Qt, eliminating the need for AutoIt.
<br><br>
This project was intended for learning only. 
I'll update this project when I have time.

## Features
 
- **Process Management:**
    - Search for processes by name or ID(WIP).
    - Retrieve and display detailed information about processes.
- **Window Manipulation:**
    - Change the title of the process window.
    - Set the window to be TopMost or remove it from TopMost status.
    - Resize the window.
    - Adjust the window's transparency (opacity).
- **Window Commands:**
    - Maximize, minimize, or focus the window.
    - Kill the process associated with the window.

## Requirements

- **Qt 5.x or 6.x:** Required for GUI and cross-platform functionality.
- **Windows Operating System:** Designed specifically for Windows environments.

## Building the Project

### Prerequisites

1. **Install Qt:** Download and install Qt from the [official Qt website](https://www.qt.io/download-dev).
2. **Qt Creator:** Using Qt Creator is recommended for a streamlined development experience.

### Building Instructions
1. **Clone the Repository:**
    ```bash
    git clone https://github.com/lul-v3/cWin.git
    cd cWin
    ```
2. **Open Project in Qt Creator:**
    Open ``cWin.pro`` in Qt Creator to load the project and configure build settings automatically.

3. **Build the Project:**
    - Click on the "Build" button in Qt Creator or run the following commands:
    ```bash
    qmake
    make
    ```
    This will compile the project and produce an executable.

## Usage
### Running the Application

1. **Launch cWin:** Execute the compiled binary from your build directory.
2. **Using the Application:** 
    - **Find Process:** Enter the process name or ID(WIP) to fetch its details.
    - **Manage Windows:** Use the provided options to manipulate window attributes or execute commands.

### Contributing
Contributions to cWin are encouraged. To contribute:

1. **Fork the Repository:** Fork the project on GitHub and clone it to your local machine.
2. **Create a Branch:** Develop your changes on a new branch.
3. **Commit Changes:** Commit your updates with clear, descriptive messages.
4. **Push and Create Pull Request:** Push your changes and submit a pull request on GitHub for review.

### License
**cWin** is licensed under the MIT License. For more details, see the [LICENSE](https://github.com/lul-v3/cWin/blob/main/LICENSE) file.

## Acknowledgements
- [Dodaucy](https://www.github.com/dodaucy/): For the original [auWin](https://www.github.com/dodaucy/auwin/) AutoIt3 project that served as the inspiration for **cWin**.
- **Qt Framework:** For providing a robust toolkit for GUI development.

## Contact
For questions or support, please reach out to contact@lulv3.com
