#ifndef LIMITSREADER_H
#define LIMITSREADER_H
#include <QString>
#include <optional>

class LimitsReader
{
public:
    struct Limits
    {
        int xMin, xMax;
        int yMin, yMax;
        int vMin, vMax;
        int mMin, mMax;
        int sMin, sMax;
        double aMin, aMax;
        int pMin, pMax;
    };
    static std::optional<Limits> read(const QString& filename);
private:
    LimitsReader() = delete;
};

#endif // LIMITSREADER_H
