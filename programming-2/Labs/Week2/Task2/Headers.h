//
// Created by giley on 2/8/2026.
//

#ifndef HEADERS_H
#define HEADERS_H

#define M_IN_M 1.0f
#define FT_IN_M 0.3048f
#define M_IN_KM 1000.00f
#define M_IN_YD 0.9144f
#define M_IN_IN 0.0254f
#define M_IN_DM 0.1f

enum Unit {
    UNIT_M,
    UNIT_FT,
    UNIT_KM,
    UNIT_YD,
    UNIT_IN,
    UNIT_DM,
    UNIT_UNKNOWN,
    UNIT_COUNT
};


double GetConvertedDistance(double value, enum Unit inputUnit, enum Unit destUnit);
char * ReturnPrintableUnit(enum Unit unitType);
enum Unit GetDistanceUnitType(char *unit);

#endif //HEADERS_H
