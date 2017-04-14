//
// kkarp.c
//
//
// Created by 90982966 and 90977369
//
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>


/* Function that runs KK algorithm on an an array of numbers
    Returns the residue of the partition
 */
long long int kk(long long int nums[], int size)
{
    // For testing
    for(int u = 0; u < size; u++)
    {
        printf("%lld ", nums[u]);
    }
    printf("\n");
    
    
    int greatestIndex = 0;
    long long int greatest = 0;
    int secgreatestIndex = 0;
    long long int secgreatest = 0;
    int i = 0;
    int j = 0;
    long long int magn = 0;
    
    for(int k = 0; k < size - 1; k++)
    {
        // Find greatest
        for(i = 0; i < size; i++)
        {
            if(nums[i] > greatest)
            {
                greatest = nums[i];
                greatestIndex  = i;
            }
        }
        // Change to 0
        nums[greatestIndex] = 0;
    
        // Find second greatest
        for(j = 0; j < size; j++)
        {
            if(nums[j] > secgreatest)
            {
                secgreatest = nums[i];
                secgreatestIndex  = i;
            }
        }
        // Calculate |ai - aj|
        magn = greatest - secgreatest;
        // Replace second greatest with 0 and the greatest with the difference
        nums[secgreatestIndex] = 0;
        nums[greatestIndex] = magn;
    
        // Reset for next iteration
        greatestIndex = 0;
        greatest = 0;
        secgreatestIndex = 0;
        secgreatest = 0;
        
        // for testing
        for(int u = 0; u < size; u++)
        {
            printf("%lld ", nums[u]);
        }
        printf("\n");
    }
    
    // Calculate residue
    long long int residue = 0;
    for(int l = 0; l < size; l++)
    {
        residue += nums[l];
    }
    return residue;
}

// MAIN FOR TESTING
int main()
{
    // Pdf example
    long long int arr[] = {10,8,7,6,5};
    int n = 5;
    printf("Residue is: %lld \n", kk(arr,n));
    
    
    return 0;
}
