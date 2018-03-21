#include <stdio.h>

#define RED 0
#define GREEN 1
#define BLUE 2

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


typedef struct pixelColor {
    unsigned short int red, green, blue;
} Pixel;

typedef struct imageScale {
    unsigned short int pixel[512][512][3];
    unsigned int width, height;
} Image;

Image escala_de_cinza(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int pixelAverage = img.pixel[i][j][RED] +
                               img.pixel[i][j][GREEN] +
                               img.pixel[i][j][BLUE];
            pixelAverage /= 3;

            img.pixel[i][j][RED] = pixelAverage;
            img.pixel[i][j][GREEN] = pixelAverage;
            img.pixel[i][j][BLUE] = pixelAverage;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3],
          int T, unsigned int width) {

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel average = {0, 0, 0};

            int lowerHeight = min(height - 1,i + T/2);
            int lowerWidth = min(width - 1,j + T/2);
            for(int x = max(0,i - T/2); x <= lowerHeight; ++x) {
                for(int y = max(0,j - T/2); y <= lowerWidth; ++y) {
                    average.red += pixel[x][y][RED];
                    average.green += pixel[x][y][GREEN];
                    average.blue += pixel[x][y][BLUE];
                }
            }

            average.red /= T * T;
            average.green /= T * T;
            average.blue /= T * T;

            pixel[i][j][RED] = average.red;
            pixel[i][j][GREEN] = average.green;
            pixel[i][j][BLUE] = average.blue;
        }
    }
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotacionada.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotacionada.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][RED] = 255 - pixel[i][j][RED];
            pixel[i][j][GREEN] = 255 - pixel[i][j][GREEN];
            pixel[i][j][BLUE] = 255 - pixel[i][j][BLUE];
        }
    }
}

Image cortar_imagem(Image img, int x, int y, int w, int h) {
    Image cortada;

    cortada.width = w;
    cortada.height = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            cortada.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            cortada.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return cortada;
}


int main() {
    Image img;

    char imageType[4];
    scanf("%s", imageType);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][RED],
                                 &img.pixel[i][j][GREEN],
                                 &img.pixel[i][j][BLUE]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.height; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[RED] = img.pixel[x][j][RED];
                        pixel[GREEN] = img.pixel[x][j][GREEN];
                        pixel[BLUE] = img.pixel[x][j][BLUE];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2]
                                * .189;

                        int menor_r = min(255,p);
                        img.pixel[x][j][RED] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = min(255,p);
                        img.pixel[x][j][GREEN] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = min(255,p);
                        img.pixel[x][j][BLUE] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.width, h = img.height;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i = 0; i < h; ++i) {
                    for (int j = 0; j < w; ++j) {
                        int x = i, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i;

                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cortar_imagem(img, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][RED],
                                   img.pixel[i][j][GREEN],
                                   img.pixel[i][j][BLUE]);

        }
        printf("\n");
    }
    return 0;
}
