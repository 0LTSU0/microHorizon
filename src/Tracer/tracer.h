#pragma once
#include <string>

enum traceLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Tracer {    
public:
    enum traceMode {
        CONSOLE,
        TRACEFILE //to be implemented
    };
    
    static void log(const std::string& message, traceLevel);
    static void changeTracingMode(Tracer::traceMode mode);

private:
    static Tracer::traceMode tracerMode;
};