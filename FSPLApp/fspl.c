#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Simple command-line free-space path loss calculator.
 * Compiles with: gcc -o fspl fspl.c -lm
 * Run with ./fspl (or fspl.exe on Windows).
 * Enter the distance in meters and frequency in hertz when prompted.
 */

double calculate_fspl(double distance_m, double frequency_hz) {
    const double c = 299792458.0; /* speed of light in m/s */
    double ratio = 4.0 * M_PI * distance_m * frequency_hz / c;
    return 20.0 * log10(ratio);
}

int main(void) {
    double d, f;
    printf("Free-space path loss calculator\n");
    printf("Distance (meters): ");
    if (scanf("%lf", &d) != 1 || d <= 0) {
        fprintf(stderr, "Invalid distance.\n");
        return EXIT_FAILURE;
    }
    printf("Frequency (hertz): ");
    if (scanf("%lf", &f) != 1 || f <= 0) {
        fprintf(stderr, "Invalid frequency.\n");
        return EXIT_FAILURE;
    }
    double loss = calculate_fspl(d, f);
    printf("FSPL = %.2f dB\n", loss);
    return EXIT_SUCCESS;
}
