SET( HEADERS
  Crc8.h
  i2c-api.h
  i2c-io-api.h
  i2c-api.h
  Log.h
  DumpMem.h
  i2c.h
  Config.h
)

SET( SOURCES
  Crc8.c
  i2c-api.h
  i2c-io-api.c
  i2c-api.c
  Log.c
  DumpMem.c
)

# build a shared library
ADD_LIBRARY( i2c STATIC ${SOURCES} )