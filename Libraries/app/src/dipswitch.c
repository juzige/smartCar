#include "dipswitch.h"

void bomakaiguan()
{ 
  mode_flag=gpio_get(E10)+gpio_get(E8)*2; 
}

