#ifndef ETHERCAT_UTILE_H
#define ETHERCAT_UTILE_H

#include <QObject>

///
/// \brief The Ethercat_Utile class(静态类)
///
class Ethercat_Utile : public QObject
{
    Q_OBJECT
public:

    static int8_t readData_int8(char *baseAddr, int addr_offset);
    static void writeData_int8(char *baseAddr, int addr_offset, int8_t data);
    static int16_t readData_int16(char *baseAddr, int addr_offset);
    static void writeData_int16(char *baseAddr, int addr_offset, int16_t data);
    static int32_t readData_int32(char *baseAddr, int addr_offset);
    static void writeData_int32(char *baseAddr, int addr_offset, int32_t data);
    //Todo: bool struct
};

#endif // ETHERCAT_UTILE_H
