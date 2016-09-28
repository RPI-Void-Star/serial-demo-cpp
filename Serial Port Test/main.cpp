//
//  main.cpp
//  Serial Port Test
//
//  Created by John Drogo on 9/27/16.
//  Copyright © 2016 John Drogo. All rights reserved.
//

#include "serial.hpp"

int main(int argc, char* argv[]){
    
    // on Unix POSIX based systems, turn off line buffering of input, so cin.get() returns after every keypress
    // On other systems, you'll need to look for an equivalent
#ifdef POSIX
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
    tcsetattr(0, TCSANOW, &new_settings);
#endif
    try {
        if (argc != 3) {
            cerr << "Usage: minicom <baud> <device>\n";
            return 1;
        }
        
        boost::asio::io_service io_service;
        minicom_client c(io_service, boost::lexical_cast<unsigned int>(argv[1]), argv[2]);
        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
        while (c.active()) {
            char ch;
            cin.get(ch); // blocking wait for standard input
            if (ch == 3) // ctrl-C to end program
                break;
            c.write(ch);
        }
        c.close(); // close the minicom client connection
        t.join(); // wait for the IO service thread to close
    }
    catch (exception& e){ cerr << "Exception: " << e.what() << "\n"; }
#ifdef POSIX // restore default buffering of standard input
    tcsetattr(0, TCSANOW, &stored_settings);
#endif
    return 0;
}