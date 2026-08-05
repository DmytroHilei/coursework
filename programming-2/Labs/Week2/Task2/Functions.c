#include "Headers.h"
#include <string.h>

double GetConvertedDistance(double value,
                            enum Unit inputUnit,
                            enum Unit destUnit)
{
    const double toMeters[UNIT_COUNT] = {
        [UNIT_M]  = M_IN_M,
        [UNIT_FT] = FT_IN_M,
        [UNIT_KM] = M_IN_KM,
        [UNIT_YD] = M_IN_YD,
        [UNIT_IN] = M_IN_IN,
        [UNIT_DM] = M_IN_DM,
        [UNIT_UNKNOWN] = 0.0
    };

    if (inputUnit == UNIT_UNKNOWN ||
        destUnit == UNIT_UNKNOWN)
        return -1;

    return value * toMeters[inputUnit] /
                   toMeters[destUnit];
}

char *ReturnPrintableUnit(enum Unit unitType)
{
    switch (unitType)
    {
        case UNIT_M:  return "m";
        case UNIT_FT: return "ft";
        case UNIT_KM: return "km";
        case UNIT_YD: return "yd";
        case UNIT_IN: return "in";
        case UNIT_DM: return "dm";
        default: return "unknown";
    }
}


enum Unit GetDistanceUnitType(char *unit)
{
    const char* units[] = {"m", "ft", "km", "yd", "in", "dm"};

    if (strcmp(unit, units[UNIT_M]) == 0)  return UNIT_M;
    if (strcmp(unit, units[UNIT_FT]) == 0) return UNIT_FT;
    if (strcmp(unit, units[UNIT_KM]) == 0) return UNIT_KM;
    if (strcmp(unit, units[UNIT_YD]) == 0) return UNIT_YD;
    if (strcmp(unit, units[UNIT_IN]) == 0) return UNIT_IN;
    if (strcmp(unit, units[UNIT_DM]) == 0) return UNIT_DM;

    return UNIT_UNKNOWN;
}
