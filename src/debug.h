#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

class debug{
private:
    bool _enable = false;
    int _verbosity = 0;
public:
    debug(bool enable, int verbosity){
        this->_enable = enable;
        this->_verbosity = verbosity;
    };
    debug(bool enable){
        this->_enable = enable;
    }

    template <typename T>
    void out(T o){
        if(this->_verbosity >= 0 && this->_enable)
            std::cout << "[DEBUG] " << o << std::endl;
    }
    template <typename T>
    void out(std::string label, T o){
        if(this->_verbosity >= 0 && this->_enable)
            std::cout << "[DEBUG] [" << label << "] "  << o << std::endl;
    }

    template <typename T>
    void warn(T o){
        if(this->_verbosity >= 1 && this->_enable)
            std::cout << "[WARN] " << o << std::endl;
    }

    template <typename T>
    void err(T o){
        if(this->_verbosity >= 2 && this->_enable)
            std::cout << "[ERR] " << o << std::endl;
    }

};

#endif // DEBUG_H
