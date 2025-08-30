#include <stdio.h>

// Global variables
int a;
int b;
int opt;
int res;

// Global Function
int calculator();
int logger(int RorW);

int main()
{
  printf("1 for addition \n2 for subtraction \n3 for multiplication \n4 for division \n5 for history  \n6 for exit \n");
  do{
  printf("Enter the option:");
  scanf("%d", &opt);
  if (opt < 1 || opt > 6)
  {
    printf("Error: Invalid operator\n");
    return 0;
  }
  else if (opt == 5)
  {
    logger(0);
    return 0;
  }
  else
  {

    printf("Enter the number one:");
    scanf("%d", &a);
    printf("Enter the number two:");
    scanf("%d", &b);
    calculator();
    logger(1);
  }}while(opt!=6);
  return 0;
}

int calculator()
{

  switch (opt)
  {
  case 1:
    res = a + b;
    printf("%d\n", res);
    break;
  case 2:
    res = a - b;
    printf("%d\n", res);
    break;
  case 3:
    res = a * b;
    printf("%d\n", res);
    break;
  case 4:
    if (b == 0)
    {
      printf("Error: Division by zero\n");
      break;
    }
    else
    {
      res = a / b;
      printf("%d\n", res);
      break;
    }
  }
}

int logger(int RorW)
{
  FILE *calcfile;
  calcfile = fopen("calc.txt", "a");
  if (RorW == 1)
  {
    fprintf(calcfile, "%d | %d | %d | %d\n", a, b, opt, res);
    fclose(calcfile);
  }
  else if (RorW == 0)
  {
    char ch;
    calcfile = fopen("calc.txt", "r");
    printf("num1  | num2 | opt | result\n");
    while ((ch = fgetc(calcfile)) != EOF)

      putchar(ch);
    fclose(calcfile);
  }

  return 0;
}