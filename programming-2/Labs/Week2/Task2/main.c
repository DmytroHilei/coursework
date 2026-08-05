#include <stdio.h>

#include "Headers.h"

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: program file unit\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }

    enum Unit destUnit = GetDistanceUnitType((char*)argv[2]);

    double value;
    char uniStr[16];


    double total = 0;
    int count = 0;

    while (fscanf(f, "%lf %s", &value, uniStr) == 2) {
        enum Unit unitType = GetDistanceUnitType(uniStr);

        double converted = GetConvertedDistance(value, unitType, destUnit);

        printf("%.2f %s-> %.2f %s\n", value, uniStr,
        converted,
        ReturnPrintableUnit(destUnit));

        total += converted;
        count++;
    }
    fclose(f);
    printf("\nTotal: %.2f %s\n",
       total,
       ReturnPrintableUnit(destUnit));

    if (count == 0) {
        return 0;
    }

    printf("Average: %.2f %s\n",
           total / count,
           ReturnPrintableUnit(destUnit));

    return 0;
}



