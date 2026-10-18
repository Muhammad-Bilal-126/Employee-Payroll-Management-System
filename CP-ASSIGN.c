// #include <stdio.h>
// int main()
// {   float miles, cpg, mpg, park, tolls, dailyCost;

//     printf("Enter total miles driven per day: ");
//     scanf("%f", &miles);

//     printf("Enter cost per gallon of gasoline: ");
//     scanf("%f", &cpg);

//     printf("Enter average miles per gallon: ");
//     scanf("%f", &mpg);

//     printf("Enter parking fees per day: ");
//     scanf("%f", &park);

//     printf("Enter tolls per day: ");
//     scanf("%f", &tolls);

//     dailyCost = (miles / mpg) * cpg + park + tolls;
//     printf("\nYour daily driving cost is: $%.2f\n", dailyCost);

//     return 0;
// }




// #include <stdio.h>
// int main()
// {   float weight, height, bmi;
//     printf("This is a Body Mass Index Calculator\n");
//     printf("Enter your weight in pounds: ");
//     scanf("%f", &weight);

//     printf("Enter your height in inches: ");
//     scanf("%f", &height);

//     bmi = (weight * 703) / (height * height);

//     printf("\nYour BMI is: %.2f\n\n", bmi);

//     printf("BMI VALUES\n");
//     printf("Underweight: less than 18.5\n");
//     printf("Healthy:      18.5 - 24.9\n");
//     printf("Overweight:   25.0 - 29.9\n");
//     printf("Obesity:      30.0 or greater\n");

//     return 0;
// }



// #include <stdio.h>
// int main()
// {   int num;
//     printf("Number\tSquare\tCube\n");

//     num = 0;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 1;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 2;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 3;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 4;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 5;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 6;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 7;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 8;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 9;    printf("%d\t%d\t%d\n", num, num * num, num * num * num);
//     num = 10;   printf("%d\t%d\t%d\n", num, num * num, num * num * num);

//     return 0;
// }



// #include <stdio.h>
// int main()
// {
//     int num, d1, d2, d3, d4, d5;

//     printf("Enter a five-digit number: ");
//     scanf("%d", &num);

//     d1 = num / 10000;
//     d2 = (num / 1000) % 10;
//     d3 = (num / 100) % 10;
//     d4 = (num / 10) % 10;
//     d5 = num % 10;

//     printf("%d   %d   %d   %d   %d\n", d1, d2, d3, d4, d5);

//     return 0;
// }




// #include <stdio.h>
// int main()
// {   printf("A = %d\n", 'A');
//     printf("B = %d\n", 'B');
//     printf("C = %d\n", 'C');

//     printf("a = %d\n", 'a');
//     printf("b = %d\n", 'b');
//     printf("c = %d\n", 'c');

//     printf("0 = %d\n", '0');
//     printf("1 = %d\n", '1');
//     printf("2 = %d\n", '2');

//     printf("$ = %d\n", '$');
//     printf("* = %d\n", '*');
//     printf("+ = %d\n", '+');
//     printf("/ = %d\n", '/');

//     printf("Blank character = %d\n", ' ');

//     return 0;
// }




// #include <stdio.h>
// int main()
// {  //Eight printfs are used
//     printf("* * * * * * * *\n");
//     printf(" * * * * * * * *\n");
//     printf("* * * * * * * *\n");
//     printf(" * * * * * * * *\n");
//     printf("* * * * * * * *\n");
//     printf(" * * * * * * * *\n");
//     printf("* * * * * * * *\n");
//     printf(" * * * * * * * *\n");
//     printf("\n");
//    //Single Printf is used
//    printf("* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n* * * * * * * *\n * * * * * * * *\n");
//     return 0;
// }



// #include <stdio.h>

// int main()
// {
//     int num1, num2;

//     printf("Enter two integers: ");
//     scanf("%d %d", &num1, &num2);

//     if (num1 % num2 == 0)
//         printf("%d is a multiple of %d.\n", num1, num2);

//     if (num1 % num2 != 0)
//         printf("%d is not a multiple of %d.\n", num1, num2);

//     return 0;
// }



// #include <stdio.h>
// int main()
// {
//     /* M rotated sideways */
//     printf("MMMMMMMMMM\n");
//     printf("      M\n");
//     printf("     M\n");
//     printf("    M\n");
//     printf("     M\n");
//     printf("      M  \n");
//     printf("MMMMMMMMMM\n");
//     printf("\n");

//     /* B rotated sideways */
//     printf("BBBBBBBBBBBB\n");
//     printf("B     B    B\n");
//     printf("B     B    B\n");
//     printf("B  B     B B\n");

//     return 0;
// }



// #include <stdio.h>

// int main()
// {
//     int num;

//     printf("Enter an integer: ");
//     scanf("%d", &num);

//     if (num % 2 == 0)
//         printf("%d is even.\n", num);

//     if (num % 2 != 0)
//         printf("%d is odd.\n", num);

//     return 0;
// }




// #include <stdio.h>
// int main()
// {
//     int num1, num2, num3, largest, smallest;

//     printf("Enter three integers: ");
//     scanf("%d %d %d", &num1, &num2, &num3);

//     largest = num1;
//     smallest = num1;

//     if (num2 > largest)
//         largest = num2;

//     if (num3 > largest)
//         largest = num3;

//     if (num2 < smallest)
//         smallest = num2;

//     if (num3 < smallest)
//         smallest = num3;

//     printf("Largest: %d\n", largest);
//     printf("Smallest: %d\n", smallest);

//     return 0;
// }




// #include <stdio.h>
// int main()
// {
//     printf("   *********       ***        *            *      \n");
//     printf("   *       *     *     *     ***         *  *     \n");
//     printf("   *       *    *       *   *****       *     *    \n");
//     printf("   *       *    *       *     *        *       *   \n");
//     printf("   *       *    *       *     *       *        *   \n");
//     printf("   *       *    *       *     *        *      *    \n");
//     printf("   *       *    *       *     *         *    *     \n");
//     printf("   *       *     *     *      *          *  *      \n");
//     printf("   *********       ***        *            *       \n");
    
// }




// #include <stdio.h>

// int main()
// {
//     int totalSeconds, hours, minutes, seconds;

//     printf("Enter the total time elapsed in seconds: ");
//     scanf("%d", &totalSeconds);

//     hours = totalSeconds / 3600;
//     minutes = (totalSeconds % 3600) / 60;
//     seconds = totalSeconds % 60;

//     printf("Time: %02d:%02d:%02d\n", hours, minutes, seconds);

//     return 0;
// }




// //Write a program that inputs three different integers from the keyboard, then prints the sum, the average, the product, the smallest and the largest of these numbers. Use only the single-selection form of the if statement you learned in this chapter. The screen dialogue should appear as follows: 
// #include<stdio.h>
// int main()
// {
//     int n1, n2, n3, smallest , largest;
//     printf("Enter Three Integer : \n");
//     scanf("%d %d %d", &n1, &n2, &n3);

//     printf("Sum is %d\n", n1+n2+n3);
//     printf("Average is %f\n", (n1+n2+n3)/3.0);
//     printf("Product is %d\n", n1*n2*n3);
    
//     smallest = n1;
//     largest = n1;

//     if (n2 < smallest)
//         smallest = n2;

//     if (n3 < smallest)
//         smallest = n3;

//     if (n2 > largest)
//         largest = n2;

//     if (n3 > largest)
//         largest = n3;
//     printf("Smallest is %d\n", smallest);
//     printf("Largest is %d\n", largest);
//     return 0;
    
// }


// // Write a program that asks the user to enter the highest rainfall ever in one season for a country, and the rainfall in the current year for that country, obtains the values from the user, checks if the current rainfall exceed the highest rainfall and prints an appropriate message on the screen. If the current rainfall is higher, it assigns that value as the highest rainfall ever. Use only the single-selection form of the if statement you learned in this chapter.
// #include <stdio.h>

// int main() {
//     float highestRainfall, currentRainfall;

//     printf("Enter the highest rainfall ever in one season: ");
//     scanf("%f", &highestRainfall);

//     printf("Enter the rainfall in the current year: ");
//     scanf("%f", &currentRainfall);

//     if (currentRainfall > highestRainfall) {
//         printf("The current rainfall exceeds the highest rainfall ever.\n");
//         highestRainfall = currentRainfall;
//         printf("The new highest rainfall is %.2f\n", highestRainfall);
//     }
//     else{
//         printf("Normal Rainfall\n");
//     }

//     return 0;
//}




// #include<stdio.h>
// int main()
// {
//     float u, a, t, v, s;

//     printf("Enter initial velocity (u): \n");
//     scanf("%f", &u);

//     printf("Enter acceleration (a): \n");
//     scanf("%f", &a);

//     printf("Enter time elapsed (t): \n");
//     scanf("%f", &t);

//     v = u + a*t;
//     printf("Final Velocity is %f\n", v );

//     s = u*t + (a*t*t)/2;
//     printf("Distance covered is %f\n", s);
//     return 0;

//}



// //Write a program that asks the user to enter two numbers, obtains them from the user and prints their sum, product, difference, quotient and remainder.
// #include <stdio.h>
// int main()
// {
//     int a , b;
//     printf("Enter two numbers");
//     scanf("%d %d", &a, &b);

//     printf("SUM is %d\n", a+b);
//     printf("Product is %d\n",  a*b);
//     printf("Difference is %d\n", a-b);
//     printf("Quotient is %d\n", a/b);
//     printf("Remainder is %d\n",  a%b);
// }

/* Write a program than asks the user to enter the initial velocity and acceleration of an object, and the time that has elapsed, places them in the variables u, a, and t, and prints the final velocity, v, and distance traversed, s, using the following equations. 
a)  v = u + at 
b) s = ut + ½at²*/ 









