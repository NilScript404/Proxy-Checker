# Proxy Checker

**Proxy Checker** is going to be a very vesatile and advanced Proxy checker written in C , with the help of Raylib for the ui and libcurl for handling the request. i wanted this project to be a little different and more complex than a typical proxy checker , in terms of both the implemenation and the ideas. please note that this is a work in progess and its not even close to working ( the proxy Checker which is written in C# is pretty good and ready , you can use that), and its just made as a hobby, i will update this project whenever im bored and just feel like it.

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
- **Threaded Requests**: Implement multithreading to handle proxy validation for better performance ( its already done , i just need to figure out how im going to use it with raylib).
- **Proxy Save Feature**: Save categorized proxies to separate text files including additional information regarding each proxy and its requests in (`good_proxies.txt` and `bad_proxies.txt`)( its already done , i just need to figure out how im going to use it with raylib).
- **Reqest Handling**: Implement many options for configuring the proxies and their requests , such as terminating a request if its being blocked by cf or stuff like that( its already done , i just need to figure out how im going to use it with raylib).
- **Advanced Filtering**: Add options to filter proxies based on specific criteria (e.g., response time, country, website filtering).
- **Advanced Profiles**: Add option to create different profiles , and and customize each profile to your need.

### UI Improvements
- **Dynamic UI Updates**: lots of stuff.



