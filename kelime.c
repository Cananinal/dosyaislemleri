#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

long dosyaBoyutu(const char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "rb");
    if (dosya == NULL) {
        printf("Dosya acilamadi: %s\n", dosyaAdi);
        return -1;
    }
    
    fseek(dosya, 0, SEEK_END);
    long boyut = ftell(dosya);
    fclose(dosya);
    
    return boyut;
}

int zipDosyalari() {
    int sonuc = system("zip -j dosyalar.zip bilgiler.txt bilgiler2.txt fark.txt");
    
    if (sonuc == 0) {
        printf("ZIP işlemi başarili.\n");
    } else {
        printf("ZIP işlemi başarisiz oldu. Hata kodu: %d\n", sonuc);
    }
    
    return sonuc;
}

int main() {
    FILE *d1, *d2;
    char cumleler[255];
    char tcumleler[255];
    long oncekiBoyut1 = 0, oncekiBoyut2 = 0;

    while (1) {
        d1 = fopen("bilgiler.txt", "r");
        d2 = fopen("bilgiler2.txt", "a");

        if (d1 == NULL || d2 == NULL) {
            printf("Dosya okuma/yazma hatasi\n");
            return 1;
        }

        if (fgets(cumleler, sizeof(cumleler), d1) != NULL) {
            int karakters = strlen(cumleler);
            if (cumleler[karakters - 1] == '\n') {
                cumleler[karakters - 1] = '\0';
                karakters--;
            }

            for (int i = 0; i < karakters; i++) {
                tcumleler[i] = cumleler[karakters - 1 - i];
            }
            tcumleler[karakters] = '\0';

            fputs(tcumleler, d2);
            fputc('\n', d2);
            printf("İşlem başarili, bilgiler2.txt dosyasina yazildi.\n");
        }

        fclose(d1);
        fclose(d2);

        long boyut1 = dosyaBoyutu("bilgiler.txt");
        long boyut2 = dosyaBoyutu("bilgiler2.txt");

        if (boyut1 != -1 && boyut2 != -1) {
            long fark1 = boyut1 - oncekiBoyut1;
            long fark2 = boyut2 - oncekiBoyut2;

            if (fark1 != 0 || fark2 != 0) {
                printf("Değişiklik tespit edildi: bilgiler.txt fark: %ld bayt, bilgiler2.txt fark: %ld bayt\n", fark1, fark2);

                FILE *farkDosyasi = fopen("fark.txt", "a");
                if (farkDosyasi != NULL) {
                    fprintf(farkDosyasi, "bilgiler.txt boyut farki: %ld bayt\n", fark1);
                    fprintf(farkDosyasi, "bilgiler2.txt boyut farki: %ld bayt\n", fark2);
                    fclose(farkDosyasi);
                }

                zipDosyalari();
            }

            printf("bilgiler.txt boyutu: %ld bayt\n", boyut1);
            printf("bilgiler2.txt boyutu: %ld bayt\n", boyut2);

            oncekiBoyut1 = boyut1;
            oncekiBoyut2 = boyut2;
        }

        sleep(5);
    }

    return 0;
}