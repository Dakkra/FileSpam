#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

//Determines if the threads should stay alive. When false, all threads should finish and close
bool keepalive = true;
std::unordered_map<std::string, std::ifstream *> open_files;

//Gets a filename from the user. This is a simple helper function
std::string get_file_name() {
    std::string input_buffer;
    std::cout << "Enter a filename:";
    std::cin >> input_buffer;
    return input_buffer;
}

//Attempts to create a file with the provided filename
void make_file(const std::string &file_name) {
    //Create the file
    std::cout << "Creating file: " << file_name << std::endl;
    std::ofstream ofile(file_name);
    ofile << " ";
    ofile.flush();
    ofile.close();

    //Verify that file was created
    std::ifstream ifile(file_name);
    if (ifile.good())
        std::cout << "File " << file_name << " created!" << std::endl;
    else
        std::cout << "Could not create file " << file_name << std::endl;
    ifile.close();
}

//Attempts to remove a file with the provided filename
void del_file(const std::string &file_name) {
    //Verify that file exists
    std::ifstream origfile(file_name);
    if (!origfile.good()) {
        std::cout << "Could not find file " << file_name << std::endl;
        origfile.close();
        return;
    }
    origfile.close();

    //Remove file
    std::cout << "Removing file " << file_name << std::endl;
    std::remove(file_name.c_str());

    //Verify that file was removed
    std::ifstream ifile(file_name);
    if (ifile.good())
        std::cout << "Failed to remove file " << file_name << std::endl;
    else
        std::cout << "Removed file " << file_name << std::endl;
    ifile.close();
}

//Attempts to open a file without closing the file stream and registers it with the open files map
void open_file(const std::string &filename) {
    std::cout << "Opening file " << filename << std::endl;
    //Opens the file
    auto ifile = new std::ifstream(filename);

    //Make sure the stream is good
    if (!ifile->good()) {
        ifile->close();
        delete (ifile);
        std::cout << "File " << filename << " could not be opened! (maybe it doesn't exist?)" << std::endl;
    }

    //Register the file in the open file map
    open_files[filename] = ifile;
    std::cout << filename << " opened and registered" << std::endl;
}

//Closes the open stream to the desired registered file
void close_file(const std::string &filename) {
    std::cout << "Closing file " << filename << std::endl;

    //Get the iterator position of the filename
    auto it = open_files.find(filename);

    //Check to see if the file is registered in the map
    if (it == open_files.end()) {
        std::cout << "File not registered with open file map" << std::endl;
        return;
    }

    //Close the file stream and clear it from the map
    std::ifstream *iptr = open_files[filename]; //Get the stream pointer
    iptr->close(); // Close the file stream
    open_files.erase(it);//Removes the registry entry
    delete (iptr); // Free memory from stream object
    std::cout << "File " << filename << " closed" << std::endl;
}

//Lists all currently open files
void list_open_files() {
    std::cout << "|>Listing open file streams:" << std::endl;
    for (std::pair<std::string, std::ifstream *> pair : open_files) {
        std::cout << "|-" << pair.first << std::endl;
    }
    std::cout << "^- End of list" << std::endl;
}

//Runs the interval spam, creating and deleting files with a timed delay
void generate_interval(const size_t &number_of_intervals, const size_t &time_delay_ms, const std::string &filename) {
    //Spit out data about interval spam
    std::cout << "Running interval spam " << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Intervals: " << number_of_intervals << std::endl;
    std::cout << "Delay: " << time_delay_ms << "ms" << std::endl;
    std::cout << "Round trip delay: " << (time_delay_ms * 2) << "ms, " << (time_delay_ms / 500.0) << " seconds"
              << std::endl;

    //Run the spam
    for (size_t i = 0; i < number_of_intervals; i++) {
        std::cout << "|Interval " << (i + 1) << "/" << number_of_intervals << "|" << std::endl;
        make_file(filename);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_delay_ms));
        del_file(filename);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_delay_ms));
    }
    std::cout << "Done running interval spam" << std::endl;
}

//Processes commands received from the command line
void process_command(const std::string &command) {
    if (command == "help") {
        std::cout << "help        -> Prints the help menu" << std::endl;
        std::cout << "make        -> creates a file" << std::endl;
        std::cout << "del         -> removes a file" << std::endl;
        std::cout << "time        -> runs interval file spam" << std::endl;
        std::cout << "open        -> opens a file and does not close the stream" << std::endl;
        std::cout << "close       -> closes the stream to an open file" << std::endl;
        std::cout << "list        -> lists all currently opened files" << std::endl;
        std::cout << "exit        -> quits the application" << std::endl;
    } else if (command == "exit") {
        keepalive = false;
    } else if (command == "open") {
        open_file(get_file_name());
    } else if (command == "close") {
        close_file(get_file_name());
    } else if (command == "list") {
        list_open_files();
    } else if (command == "make") {
        make_file(get_file_name());
    } else if (command == "del") {
        del_file(get_file_name());
    } else if (command == "time") {
        size_t num_ints;
        size_t time_delay_ms;
        std::string file_name = get_file_name();
        std::cout << "Enter number of interval iterations:";
        std::cin >> num_ints;
        std::cout << "Enter number ms between intervals:";
        std::cin >> time_delay_ms;
        generate_interval(num_ints, time_delay_ms, file_name);
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}

int main() {
    std::cout << "|File Interval Spam| ~~ By Chris Soderquist (2019)" << std::endl;
    std::cout << "Single threaded file spam application" << std::endl;
    std::cout << "Designed for use on directory scanners that detect file system changes" << std::endl;
    //Main REPL
    std::string input_buffer;
    std::cout << "You may now enter commands. Type \"help\" for available commands" << std::endl;
    while (keepalive) {
        std::cout << "~>";
        std::cin >> input_buffer;
        process_command(input_buffer);
        input_buffer.clear();
    }
    std::cout << "Closing main thread" << std::endl;
    return 0;
}