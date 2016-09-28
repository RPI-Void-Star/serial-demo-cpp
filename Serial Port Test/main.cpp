//
//  main.cpp
//  Serial Port Test
//
//  Created by John Drogo on 9/27/16.
//  Copyright © 2016 John Drogo. All rights reserved.
//

#include "serial.hpp"
char ch;

int main(int argc, char* argv[]){
    try {
        boost::asio::io_service io_service;
        minicom_client c(io_service, 9600, "/dev/ttys000", &cout);
        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
        
        while (c.active()) {
            cin.get(ch); // blocking wait for standard input
            c.write(ch);
            if (ch == '\n'){ c.write('\r'); }
        }
        c.close(); // close the minicom client connection
        t.join(); // wait for the IO service thread to close
    }
    catch (exception& e){ cerr << "Exception: " << e.what() << "\n"; }
    return 0;
}