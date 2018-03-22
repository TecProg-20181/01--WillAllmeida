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
  int p; //pixel

  for (unsigned int x = 0; x < img.height; ++x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
          pixel[RED] = img.pixel[x][j][RED];
          pixel[GREEN] = img.pixel[x][j][GREEN];
          pixel[BLUE] = img.pixel[x][j][BLUE];

          p =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
          img.pixel[x][j][RED] = min(255,p);

          p =  pixel[RED] * .349 + pixel[BLUE] * .686 + pixel[GREEN] * .168;
          img.pixel[x][j][GREEN] = min(255,p);

          p =  pixel[RED] * .272 + pixel[BLUE] * .534 + pixel[GREEN] * .131;
          img.pixel[x][j][BLUE] = min(255,p);
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

    aux.width = img.height, aux.height = img.width;

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
    img.width = width, img.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            img.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            img.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            img.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return img;
}

Image mirrorImage(int horizontal, Image img) {
  int width = img.width, height = img.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
          int x = i, y = j;

          if (horizontal == 1) y = img.width - 1 - j;
          else x = img.height - 1 - i;

          Pixel aux1;

          aux1.red = img.pixel[i][j][RED];
          aux1.green = img.pixel[i][j][GREEN];
          aux1.blue = img.pixel[i][j][BLUE];

          img.pixel[i][j][RED] = img.pixel[x][y][RED];
          img.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
          img.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];

          img.pixel[x][y][RED] = aux1.red;
          img.pixel[x][y][GREEN] = aux1.green;
          img.pixel[x][y][BLUE] = aux1.blue;

      }
  }

  return img;
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

    int optionsNumber;
    scanf("%d", &optionsNumber);

    for(int i = 0; i < optionsNumber; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: {
                img = grayScale(img);
                break;
            }
            case 2: {
                img = sepia(img);
                break;
            }
            case 3: {
                int size;
                scanf("%d", &size);

                blur(img.height, img.pixel, size, img.width);
                break;
            }
            case 4: {
                int rotations;
                scanf("%d", &rotations);
                rotations %= 4;

                for (int j = 0; j < rotations; ++j) {
                    img = rightRotation(img);
                }
                break;
            }
            case 5: {
              int horizontal = 0;
              scanf("%d", &horizontal);

              img = mirrorImage(horizontal, img);
              break;
            }
            case 6: {
                img = invertColors(img);
                break;
            }
            case 7: {
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
