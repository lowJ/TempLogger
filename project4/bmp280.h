#ifndef BMP280_H
#define BMP280_H



int bmp280_verify(); //1 = has correct ID, 0 = incorrect ID
int bmp280_init();
void bmp280_get_coefficents(); //Assigns values to digT1,digT2, digT3
void bmp280_set_config();
float bmp280_calc_temp();
float convert_C_to_F(float c);

#endif