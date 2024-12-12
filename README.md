# Proxy Checker

**Proxy Checker** is a lightweight application built in **C**, designed to validate and manage proxy servers. This tool leverages **Raylib** for UI rendering and **libcurl** for HTTP requests, providing a graphical interface to check the functionality of proxies and categorize them as "Good" or "Bad".

## Features
- **Interactive UI**: Built with Raylib, featuring a simple and modern graphical interface.
- **Proxy Validation**: Uses libcurl to perform HTTP requests to validate proxies.
- **Dynamic List Management**: Manages lists of good and bad proxies dynamically in memory.
- **Scrollable Listbox**: Displays proxies in a scrollable list for better accessibility.
- **Optimized Performance**: Implements efficient memory management for handling large proxy lists.

## Requirements
To build and run this project, you need:
- **C Compiler**: GCC, Clang, or MSVC.
- **Raylib**: A simple and easy-to-use library for games and graphical interfaces.
- **libcurl**: A library for making HTTP requests.
- **Proxies List**: A file named `proxies.txt` containing a list of proxies to validate.

## Usage
1. Ensure `proxies.txt` is present in the application directory, with one proxy per line.
2. Launch the application.
3. Click the **Start** button to begin validating proxies.
4. View the categorized proxies (Good/Bad) in the listbox.

## TODO
### Functional Enhancements
- **Threaded Requests**: Implement multithreading to handle proxy validation asynchronously for better performance.
- **Proxy Save Feature**: Save categorized proxies to separate text files (`good_proxies.txt` and `bad_proxies.txt`).
- **Timeout Handling**: Implement fine-grained timeout settings for proxy validation requests.
- **Advanced Filtering**: Add options to filter proxies based on specific criteria (e.g., response time).

### UI Improvements
- **Dynamic UI Updates**: Update the listbox in real time as proxies are validated.
- **Error Feedback**: Provide clear feedback in the UI for errors encountered during validation.

### Code Enhancements
- **Memory Management**: Optimize memory allocation and deallocation for dynamic arrays.
- **Refactoring**: Clean up and modularize the code for better readability and maintainability.

### Testing
- Create unit tests to validate the behavior of core functions (e.g., `add_string`, `Responsehandler`, etc.).
- Simulate different scenarios to ensure stability and correctness.



