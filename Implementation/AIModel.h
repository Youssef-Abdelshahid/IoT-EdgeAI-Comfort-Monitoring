#ifndef DD360370-686D-4DD7-BC3C-222E3227CF15_H
#define DD360370-686D-4DD7-BC3C-222E3227CF15_H

#include <string.h>
void score(double * input, double * output) {
    double var0[3];
    if (input[1] <= 39.5) {
        if (input[0] <= 25.899999618530273) {
            { double tmp[] = {0.0, 1.0, 0.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
        } else {
            { double tmp[] = {0.0, 0.0, 1.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
        }
    } else {
        if (input[1] <= 60.5) {
            if (input[0] <= 19.949999809265137) {
                { double tmp[] = {0.0, 1.0, 0.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
            } else {
                if (input[0] <= 26.0) {
                    { double tmp[] = {1.0, 0.0, 0.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
                } else {
                    { double tmp[] = {0.0, 1.0, 0.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
                }
            }
        } else {
            { double tmp[] = {0.0, 1.0, 0.0}; memcpy(var0, tmp, 3 * sizeof(double)); }
        }
    }
    memcpy(output, var0, 3 * sizeof(double));
}


#endif // DD360370-686D-4DD7-BC3C-222E3227CF15_H
