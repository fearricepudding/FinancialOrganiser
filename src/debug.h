#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

class debug{
private:
    bool m_enable = false;
    int m_verbosity = 0;
    std::string prefix = "[DEBUG] ";
public:
    debug(bool enable, int verbosity){
        m_enable = enable;
        m_verbosity = verbosity;
    };

    void out(int i){
        std::cout << prefix <<  i << std::endl;
    };
    void out(std::string s){
        std::cout << prefix << s << std::endl;
    };
    void out(char c){
        std::cout << prefix << c << std::endl;
    };

};

#endif // DEBUG_H
