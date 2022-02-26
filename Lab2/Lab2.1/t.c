/**************** t.c file **************/
#include "uart.c"

int v[10] = {1,2,3,4,5,6,7,8,9,10};
int sum;

int main()
{
  int i;
  char line[128];
  UART *up;
  
  uart_init();
  up = &uart[0];
  while(1){
    ufprintf(up, "enter a line : ");
    ugets(up, line);
    if (line[0]==0)
      break;
    //uprints(up, "  line = "); uprints(up, line); uprints(up, "\n\r");
    ufprintf(up, "  line = ");
    ufprintf(up, line);
    ufprintf(up, "\n\r");
  }
  
  ufprintf(up, "Compute sum of array\n\r");
  sum = 0;
  for (i=0; i<10; i++)
    sum += v[i];
  ufprintf(up, "sum = %d\n\r", sum);
  ufprintf(up, "negative sum = %d\n\r", sum * -1);
  ufprintf(up, "unsigned sum = %u\n\r", (unsigned int)sum);
  ufprintf(up, "sum as hex = %x\n\r", sum);
  ufprintf(up,"The sums are as follows: sum = %d, negative sum = %d, unsigned sum = %u, sum as hex = %x\r\n", sum, sum * -1, (unsigned int) sum, sum);
  //uputc(up, (sum/10)+'0'); uputc(up, (sum%10)+'0');
  //uprints(up, "\n\r");
  
  ufprintf(up, "END OF UART DEMO\n\r");
}