#include "RapiCore"

#include "config.h"

/** Get library version number */
const char* RAPI_VERSION()
{
  return VERSION;
}

/** Get git build number */
const char* RAPI_BUILD()
{
  return BUILD_NUMBER;
}