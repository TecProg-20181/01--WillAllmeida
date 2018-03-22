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

Image grayScale(Image img) {

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

Image sepia(Image img) {
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

  return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3],
          int size, unsigned int width) {

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel average = {0, 0, 0};

            int lowerHeight = min(height - 1,i + size/2);
            int lowerWidth = min(width - 1,j + size/2);

            for(int x = max(0,i - size/2); x <= lowerHeight; ++x) {
                for(int y = max(0,j - size/2); y <= lowerWidth; ++y) {
                    average.red += pixel[x][y][RED];
                    average.green += pixel[x][y][GREEN];
                    average.blue += pixel[x][y][BLUE];
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            pixel[i][j][RED] = average.red;
            pixel[i][j][GREEN] = average.green;
            pixel[i][j][BLUE] = average.blue;
        }
    }
}

Image rightRotation(Image img) {
    Image aux;

    aux.width = img.height;
    aux.height = img.width;

    for (unsigned int i = 0, y = 0; i < aux.height; ++i, ++y) {
        for (int j = aux.width - 1, x = 0; j >= 0; --j, ++x) {
            aux.pixel[i][j][RED] = img.pixel[x][y][RED];
            aux.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            aux.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return aux;
}

Image invertColors(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j][RED] = 255 - img.pixel[i][j][RED];
            img.pixel[i][j][GREEN] = 255 - img.pixel[i][j][GREEN];
            img.pixel[i][j][BLUE] = 255 - img.pixel[i][j][BLUE];
        }
    }

    return img;
}

Image cutImage(Image img, int x, int y, int width, int height) {
    img.width = width;
    img.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            img.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            img.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            img.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return img;
}

/*Image mirrorImage(Image img, int horizontal) {
    int width = img.width, height = img.height;

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
          int x = i, y = j;

          if (horizontal == 1) y = img.width - 1 - j;
          else x = img.height - 1 - i;

      }
    }

  return img;
}
*/

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

    int optionsNumber;
    scanf("%d", &optionsNumber);

    for(int i = 0; i < optionsNumber; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                img = grayScale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                int size;
                scanf("%d", &size);
                blur(img.height, img.pixel, size, img.width);
                break;
            }
            case 4: { // Rotacao
                int rotations;
                scanf("%d", &rotations);
                rotations %= 4;
                for (int j = 0; j < rotations; ++j) {
                    img = rightRotation(img);
                }
                break;
            }
            case 5: { // Espelhamento
              int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.width, h = img.height;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i2;

                    }
              }
                break;
            }
            case 6: { // Inversao de Cores
                img = invertColors(img);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cutImage(img, x, y, width, height);
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
