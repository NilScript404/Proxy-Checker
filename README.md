# Proxy Checker

**Proxy Checker** is a lightweight application built in **C**, designed to validate and manage proxy servers. This tool leverages **Raylib** for UI rendering and **libcurl** for HTTP requests, providing a graphical interface to check the functionality of proxies and categorize them as "Good" or "Bad". note that this project is about 25% done , and many features are still left, since i wanted this proxy checker to be very versatile and unique.

## Features
- **Interactive UI**: Built with Raylib, featuring a simple and modern graphical interface.
- **Proxy Validation**: Uses libcurl to perform HTTP requests to validate proxies.
- **Optimized Performance**: Implements efficient memory management for handling large proxy lists.

## Requirements
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
- **Threaded Requests**: Implement multithreading to handle proxy validation for better performance.
- **Proxy Save Feature**: Save categorized proxies to separate text files including additional information regarding each proxy and its requests in (`good_proxies.txt` and `bad_proxies.txt`).
- **Reqest Handling**: Implement many options for configuring the proxies and their requests , such as terminating a request if its being blocked by cf or stuff like that.
- **Advanced Filtering**: Add options to filter proxies based on specific criteria (e.g., response time, country, website filtering).
- **Advanced Profiles**: Add option to create different profiles , and and customize each profile to your need.

### UI Improvements
- **Dynamic UI Updates**: lots of stuff.



