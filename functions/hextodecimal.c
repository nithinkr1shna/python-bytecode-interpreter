#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int htod(char *hex)
{

  int len;
  len = strlen(hex);
  len--;
  int decimal = 0, val, i = 0;

  for (i = 0; hex[i] != '\0'; i++){
    if (hex[i] >= '0' && hex[i] <= '9'){
      val = hex[i] - 48;
    }else if (hex[i] >= 'a' && hex[i] <= 'f'){
      val = hex[i] - 97 + 10;
    }else if (hex[i] >= 'A' && hex[i] <= 'F'){
      val = hex[i] - 65 + 10;
    }

    decimal += val * pow(16, len);
    len--;
  }

  return decimal;
}
