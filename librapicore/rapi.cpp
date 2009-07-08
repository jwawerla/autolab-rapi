#include "RapiCore"

#include "config.h"

/** Get library version number */
const char* RAPI_VERSION()
{
  return VERSION;
}

/** Get build number */
const char* RAPI_BUILD()
{
  return BUILD_NUMBER;
}

/** Get git version */
const char* RAPI_GIT_VERSION()
{
  return GIT_VERSION;
}

/** Get build time */
const char* RAPI_BUILD_TIME()
{
  return __TIME__;
}

/** Get build date */
const char* RAPI_BUILD_DATE()
{
  return __DATE__;
}