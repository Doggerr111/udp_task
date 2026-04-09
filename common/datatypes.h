#ifndef DATATYPES_H
#define DATATYPES_H

struct SensorData
{
    uint8_t x;
    int8_t y;
    uint8_t v;
    uint8_t m;
    uint8_t s;
    float a;      
    uint8_t p;
    
    //для тестирования
    bool operator==(const SensorData& other) const 
    {
        return x == other.x &&
               y == other.y &&
               v == other.v &&
               m == other.m &&
               s == other.s &&
               std::abs(a - other.a) < 0.01f &&  
               p == other.p;
    }
};

#endif // DATATYPES_H
