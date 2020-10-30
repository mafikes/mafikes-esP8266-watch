#include <Helper.h>
#include <iostream>
#include <WString.h>
#include <stdlib.h>
#include "stdlib_noniso.h"

String Helper::getStringRounded(float value, int width = 5, int prec = 1) 
{
    char buffer[10];
    String textValue = dtostrf(value, width, prec, buffer);
    return textValue;
}