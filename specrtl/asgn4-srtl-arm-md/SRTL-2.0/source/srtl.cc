#include <iostream>
#include "srtl-driver.hh"
#include <cstring>

int main(int argc, char* argv[]) {
    bool displayStatsFlag = false;
    srtl_driver driver;
    
    driver.setLogLevel (srtl_driver::err);
    
    if (argc == 1) {
        std::cerr << "Format: srtl [-s] [-o outputFileName] inputFilename" << std::endl;
        return 1;
    }

    if (!strcmp(argv[1],"-s")) {
        if (argc == 3 || argc == 5) {
            driver.initializeDotFile ();
            if (argc == 3) {
                string s (argv[2]);
                s.append (".md");
                driver.setOutFileName (&s);
                driver.initializeOutFile ();
                if(driver.parse (argv[2])) 
                    std::cout << driver.result <<std::endl;
            } else {
                driver.setOutFileName (new string (argv[3]));
                driver.initializeOutFile ();
                if(driver.parse (argv[4])) 
                    std::cout << driver.result <<std::endl;
            }
            displayStatsFlag = true;
        } else {
            std::cerr << "Invalid file name." << std::endl;
        }
    } else if (!strcmp (argv[1], "-o")) {
        if (argc == 4) {
            string s (argv[2]);
            driver.setOutFileName (&s);
            driver.initializeOutFile ();
                if(driver.parse (argv[3])) 
                    std::cout << driver.result <<std::endl;
        } else {
            std::cerr << "Invalid Flags." << std::endl;
            std::cerr << "Format: srtl [-s] [-o outputFileName] inputFilename" << std::endl;
        }
    } else {
        string s (argv[1]);
        s.append (".md");
        driver.setOutFileName (&s);
        std::cout << "Invalid file name :" << argv[1] << std::endl;
        driver.initializeOutFile ();
        if(driver.parse (argv[1]))
            std::cout << driver.result <<std::endl;
    }

    /*
     * Calculate the statistics 
     */
    if (displayStatsFlag)
        driver.calculateAndDisplayStats ();
  
    std::cout << std::endl;
}