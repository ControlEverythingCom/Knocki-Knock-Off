#include "spark_wiring_string.h"

class SegmentLEDDriver{
public:
    bool init();
    bool displayWriteInt(int val);
    bool displayWriteInt(String val);
    
private:
    bool regWrite(int reg, int val);
};