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
    Pixel pixel[512][512];
    unsigned int width, height;
} Image;

Image grayScale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int pixelAverage = img.pixel[i][j].red +
                               img.pixel[i][j].green +
                               img.pixel[i][j].blue;
            pixelAverage /= 3;

            img.pixel[i][j].red = pixelAverage;
            img.pixel[i][j].green = pixelAverage;
            img.pixel[i][j].blue = pixelAverage;
        }
    }

    return img;
}

Image sepia(Image img) {
  int p; //pixel

  for (unsigned int x = 0; x < img.height; ++x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
          pixel[RED] = img.pixel[x][j].red;
          pixel[GREEN] = img.pixel[x][j].green;
          pixel[BLUE] = img.pixel[x][j].blue;

          p =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
          img.pixel[x][j].red = min(255,p);

          p =  pixel[RED] * .349 + pixel[BLUE] * .686 + pixel[GREEN] * .168;
          img.pixel[x][j].green = min(255,p);

          p =  pixel[RED] * .272 + pixel[BLUE] * .534 + pixel[GREEN] * .131;
          img.pixel[x][j].blue = min(255,p);
      }
  }

  return img;
}

Image blur(Image img, int size, Pixel pixel[512][512]) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel average = {0, 0, 0};

            int lowerHeight = min((img.height - 1),(i + size/2));
            int lowerWidth = min((img.width - 1),(j + size/2));

            for(int x = max(0, (i - size/2)); x <= lowerHeight; ++x) {
                for(int y = max(0, (j - size/2)); y <= lowerWidth; ++y) {
                    average.red += pixel[x][y].red;
                    average.green += pixel[x][y].green;
                    average.blue += pixel[x][y].blue;
                }
            }

            average.red /= size * size;
            average.green /= size * size;
            average.blue /= size * size;

            img.pixel[i][j].red = average.red;
            img.pixel[i][j].green = average.green;
            img.pixel[i][j].blue = average.blue;
        }
    }
    return img;
}

Image rightRotation(Image img) {
    Image aux;

    aux.width = img.height, aux.height = img.width;

    for (unsigned int i = 0, y = 0; i < aux.height; ++i, ++y) {
        for (int j = aux.width - 1, x = 0; j >= 0; --j, ++x) {
            aux.pixel[i][j].red = img.pixel[x][y].red;
            aux.pixel[i][j].green = img.pixel[x][y].green;
            aux.pixel[i][j].blue = img.pixel[x][y].blue;
        }
    }

    return aux;
}

Image invertColors(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].red = 255 - img.pixel[i][j].red;
            img.pixel[i][j].green = 255 - img.pixel[i][j].green;
            img.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
        }
    }

    return img;
}

Image cutImage(Image img, int x, int y, int width, int height) {
    img.width = width, img.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            img.pixel[i][j].red = img.pixel[i + y][j + x].red;
            img.pixel[i][j].green = img.pixel[i + y][j + x].green;
            img.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
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

          aux1.red = img.pixel[i][j].red;
          aux1.green = img.pixel[i][j].green;
          aux1.blue = img.pixel[i][j].blue;

          img.pixel[i][j].red = img.pixel[x][y].red;
          img.pixel[i][j].green = img.pixel[x][y].green;
          img.pixel[i][j].blue = img.pixel[x][y].blue;

          img.pixel[x][y].red = aux1.red;
          img.pixel[x][y].green = aux1.green;
          img.pixel[x][y].blue = aux1.blue;

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
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

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

                img = blur(img, size, img.pixel);
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
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

        }
        printf("\n");
    }
    return 0;
}
