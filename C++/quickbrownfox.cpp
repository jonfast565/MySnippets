//
//  main.cpp
//  QuickBrownFox
//
//  Created by Jonathan Fast & Luke Hines on 10/13/13.
//

//headers and namespace for std
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;

//prototypes
int main(int, const char*[]);
void rwThread(int threadNumber, ifstream* readFile, ofstream* writeFile);

// simple counting semaphore
class semaphore
{
private:
    boost::mutex mutex_;
    boost::condition_variable condition_;
    unsigned long count_;
    
public:
    semaphore()
    : count_()
    {}
    
    void notify()
    {
        boost::mutex::scoped_lock lock(mutex_);
        ++count_;
        condition_.notify_one();
    }
    
    void wait()
    {
        boost::mutex::scoped_lock lock(mutex_);
        while(!count_)
            condition_.wait(lock);
        --count_;
    }
};

// thread count and mutexes for accessing shared
// file descriptors
static int threadCount = 0;
static semaphore critical;

// mini custom exception handler
class generalException : exception {
private:
    string& exceptionContent;
public:
    // takes any custom string argument
    // and wraps in the exception class
    generalException(string &exceptionContent) :
    exceptionContent(exceptionContent){};
    const char* what() const noexcept { return exceptionContent.c_str(); };
};

// threaded method for reading files
// this method should not be modified to remove thread safety
void rwThread(int threadNumber, ifstream* readFile, ofstream* writeFile) {
    // set the current character
    char current = '\0';
    // as long as input is available
    while (true) {
        critical.notify();
        // and get its current character if not EOF
        if (readFile->good()) {
            current = readFile->get();
            // don't do this since it ignores newlines, spaces, and tabs
            // *readFile >> current;
            // write the character to the output file
            // *writeFile << current;
            // some odd errors occur with put() similar to the last lab,
            // which could be investigated *hint* *hint*
            // right now the insertion method above produces clean output
            writeFile->put(current);
        }
        else {
            // in the case that the file is EOF,
            // then break, since there is no more input
            break;
        }
        critical.wait();
    }
}

int main(int argc, const char * argv[])
{
    // program information
    cout << "--- Quick Brown Fox Program ---" << endl;
    cout << "By: Jon Fast & Luke Hines" << endl;
    
    // attempt to read the command line args
    try {
        
        // exception string decl
        string exceptionString;
        cout << "Reading command line arguments." << endl;
        
        // lack of three arguments implies failure to provide two file locations
        if (argc != 3) {
            
            // is an error...
            exceptionString = string("Invalid number of command line arguments.");
            throw generalException(exceptionString);
        }
        else {
            
            // attempt opening both file streams, one in, the other out
            ifstream* quickBrownIn = new ifstream(argv[1], ios::in | ios::binary);
            ofstream* quickBrownOut = new ofstream(argv[2], ios::out | ios::binary);
            
            // check to see if open and ready for r/w
            if (quickBrownIn->is_open() && quickBrownIn->good() &&
                quickBrownOut->is_open() && quickBrownOut->good()) {
                
                // now we do the reading and writing via threads, passing
                // passing references for each file to the spawned threads
                cout << "Open files passed. " << endl;
                
                threadCount++;
                thread quickBrownFox1(rwThread, threadCount, quickBrownIn, quickBrownOut);
                threadCount++;
                //wait until thread 1 has notified to create thread 2
                critical.wait();
                thread quickBrownFox2(rwThread, threadCount, quickBrownIn, quickBrownOut);
                
                // wait for both threads to exit
                quickBrownFox1.join();
                quickBrownFox2.join();
                cout << "Threads finished executing." << endl;
                
                // close both descriptors and delete refs
                quickBrownIn->close();
                quickBrownOut->close();
                delete quickBrownIn;
                delete quickBrownOut;
                
                // wait for the program to finish if necessary
                cout << "File descriptors closed." << endl;
                cout << "Program finished. Press any key to continue." << endl;
                
                // getchar();
                return 0;
            }
            else {
                
                // if files don't open, path values probably invalid
                cout << "Open files failed. Closing file descriptors." << endl;
                
                // if we need to clean up...
                if (quickBrownIn->is_open()) quickBrownIn->close();
                if (quickBrownOut->is_open()) quickBrownOut->close();
                delete quickBrownIn;
                delete quickBrownOut;
                
                // the error here
                exceptionString = string("Some file params were invalid.");
                throw generalException(exceptionString);
            }
        }
    }
    catch(generalException& e) {
        // general exception handling
        cout << e.what() << endl;
    }
    
    // end it now, falls through here
    return -1;
}

