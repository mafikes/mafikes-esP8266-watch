#include <Helper.h>
#include <iostream>
#include <WString.h>
#include <stdlib.h>
#include "stdlib_noniso.h"

String Helper::getStringRounded(float value) 
{
    char buffer[10];
    String textValue = dtostrf(value, 5, 1, buffer);
    return textValue;
}