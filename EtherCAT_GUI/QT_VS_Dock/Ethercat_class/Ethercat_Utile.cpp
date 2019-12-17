#include "Ethercat_Utile.h"

int8_t Ethercat_Utile::readData_int8(char *baseAddr,int addr_offset){
    int8_t *data_ptr;
    data_ptr = (int8_t*)(baseAddr+addr_offset);
    return *data_ptr;
}

void Ethercat_Utile::writeData_int8(char *baseAddr,int addr_offset,int8_t data){
    int8_t *data_ptr;
    data_ptr = (int8_t*)(baseAddr+addr_offset);
    *data_ptr = data;
}

int16_t Ethercat_Utile::readData_int16(char *baseAddr,int addr_offset){
    int16_t *data_ptr;
    data_ptr = (int16_t*)(baseAddr+addr_offset);
    return *data_ptr;
}

void Ethercat_Utile::writeData_int16(char *baseAddr,int addr_offset,int16_t data){
    int16_t *data_ptr;
    data_ptr = (int16_t*)(baseAddr+addr_offset);
    *data_ptr = data;
}

int32_t Ethercat_Utile::readData_int32(char *baseAddr,int addr_offset){
    int32_t *data_ptr;
    data_ptr = (int32_t*)(baseAddr+addr_offset);
    return *data_ptr;
}

void Ethercat_Utile::writeData_int32(char *baseAddr,int addr_offset,int32_t data){
    int32_t *data_ptr;
    data_ptr = (int32_t*)(baseAddr+addr_offset);
    *data_ptr = data;
}
