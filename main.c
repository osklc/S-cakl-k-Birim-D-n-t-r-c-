#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int parse_unit(const char *line, char *out_unit) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p)) p++;
    if (!*p || *p == '\n') return 0; // boş giriş
    char u = toupper((unsigned char)*p);
    p++;
    while (*p && *p != '\n') {
        if (!isspace((unsigned char)*p)) return -1; // ekstra görünür karakter
        p++;
    }
    *out_unit = u;
    return 1;
}

int read_unit(char *out_unit) {
    char line[128];
    if (!fgets(line, sizeof line, stdin)) return 0; // EOF veya hata
    int r = parse_unit(line, out_unit);
    if (r == 1) return 1;
    if (r == 0) {
        printf("Bos giris. Lutfen C, F veya K girin.\n");
        return -1;
    }
    printf("Lutfen sadece tek bir birim harfi girin (ornek: C). Girdiniz: %s", line);
    return -1;
}

int read_double(const char *prompt, double *out) {
    char buf[128];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) return 0;
        char extra;
        double val;
        // " %lf %c" ile ekstra karakter kontrolü yapıyoruz: eger ekstra varsa sscanf 2 döner.
        if (sscanf(buf, " %lf %c", &val, &extra) == 1) {
            *out = val;
            return 1;
        }
        printf("Gecersiz sayi. Lutfen tekrar deneyin.\n");
    }
}

int main(void) {
    const double KELVIN_OFFSET = 273.15;
    char unit;
    while (1) {
        printf("Sicaklik birimini giriniz (C, F, K) Programi sonlandirmak icin \"q\": ");
        int ru = read_unit(&unit);
        if (ru == 0) break;      // EOF
        if (ru == -1) continue;  // tekrar sor

        if (unit == 'Q') break;

        double temp;
        if (!read_double("Sicaklik degerini giriniz: ", &temp)) break;

        if (unit == 'K' && temp < 0.0) {
            printf("Kelvin 0'dan kucuk olamaz. Tekrar deneyin.\n");
            continue;
        }

        double c,f,k;
        if (unit == 'C') {
            c = temp;
            f = c * 9.0 / 5.0 + 32.0;
            k = c + KELVIN_OFFSET;
        } else if (unit == 'F') {
            f = temp;
            c = (f - 32.0) * 5.0 / 9.0;
            k = c + KELVIN_OFFSET;
        } else { // K
            k = temp;
            c = k - KELVIN_OFFSET;
            f = c * 9.0 / 5.0 + 32.0;
        }

        printf("Sicaklik Degerleri:\nCelsius: %.2f\nFahrenheit: %.2f\nKelvin: %.2f\n", c, f, k);
    }

    printf("Program sonlandirildi.\n");
    return 0;
}
