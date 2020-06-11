#ifndef STORE_H
#define STORE_H

//Temperature values will be stored with 2 decimal point precision.
//consider the case where t > 100; cause 1 deciaml point percision.
//consider how to handle negative cases

//Convert back by:
//float x = get_value(i)
//x = x/100
unsigned int get_value(int n);


// 1 = n is in bounds 
// 0 = n is not in bounds
int check_bounds(int n);

//When storing temperature values,
//(int)(Temp*100)
void store_value(unsigned int val);

#endif