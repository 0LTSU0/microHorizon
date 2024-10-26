#pragma once
#include <string>

class Tracer {
    
public:
    enum traceMode {
        CONSOLE,
        TRACEFILE //to be implemented
    };
    
    static void log(const std::string& message);
    static void changeTracingMode(Tracer::traceMode mode);

private:
    static Tracer::traceMode tracerMode;
};