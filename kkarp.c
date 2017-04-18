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
#include <time.h>//
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

#define MAX_ITER 25000


/* Function that runs KK algorithm on an an array of numbers
    Returns the residue of the partition
 */
long long int kk(long long int* nums, int size)
{
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
        //printf("greatest: %lld\n", greatest);
        // Change to 0
        nums[greatestIndex] = 0;
        
        // Find second greatest
        for(j = 0; j < size; j++)
        {
            if(nums[j] > secgreatest)
            {
                secgreatest = nums[j];
                secgreatestIndex  = j;
            }
        }
        //printf("second greatest: %lld\n", secgreatest);
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
    }
    
    // Calculate residue
    long long int residue = 0;
    for(int l = 0; l < size; l++)
    {
        residue += nums[l];
    }
    return residue;
}


// Function that generates 64-bit random number
long long int getrand()
{
    long long int r = (long long int) rand() << 32 | rand();
    return (r % ((long long int) pow(10,12) + 1));
}


// Function that generates entirely random sequence of numbers
long long int* getrandNums(long long int* outnums, int size)
{
    for(int i = 0; i < size; i++)
    {
        outnums[i] = getrand();
    }
    return outnums;
}

// Function that generates random signs
int* getrandSigns(int* outsigns, int size)
{
    long long int num = 0;
    for(int i = 0; i < size; i++)
    {
        num = getrand();
        // Divide probability in half by evens and odds
        if(num % 2 == 0)
        {
            outsigns[i] = 1;
        }
        else
        {
            outsigns[i] = -1;
        }
    }
    
    return outsigns;
}


// Calculate residue for sequence representation
long long int seqResidue(long long int* nums, int* s, int size)
{
    int long long cum = 0;
    for(int i = 0; i < size; i++)
    {
        cum += nums[i] * s[i];
    }
    return llabs(cum);
}



// Function that implements Repeated Random
long long int repRand(int size)
{
    // Declare and assign random values to S
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    
    
    
    for(int j = 0; j < MAX_ITER; j++)
    {
        // Declare and assign random values to S'
        int* signsP = malloc(sizeof(int) * size);
        signsP = getrandSigns(signsP, size);
        
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size))
        {
            free(signs);
            signs = signsP;
        }
        else
        {
            free(signsP);
        }
    }
    long long int finalRes = seqResidue(nums, signs, size);
    free(signs);
    return finalRes;
}


// Function that implements Hill Climbing
long long int hillClimb(int size)
{
    // Declare and assign random values to S and A
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    long long int i,j;
    int* signsP = malloc(sizeof(int) * size); // S'
    
    for(int k = 0; k < MAX_ITER; k++)
    {
        // Declare and assign original values to S'
        signsP = signs;
        
        // Select random indeces
        i = getrand() % (size+1);
        j = getrand() % (size+1);
        while(i == j) // i =/= j
        {
            j = getrand() % (size+1);
        }
        
        // Change S' to be a neighbor of S
        signsP[i] = -1 * signsP[i];
        if(getrand() % 2 == 0)
        {
            signsP[j] = -1 * signsP[j];
        }
        
        // Check if neighbor is better
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size))
        {
            signs = signsP;
        }
    }
    long long int finalRes = seqResidue(nums, signs, size);
    free(signs);
    return finalRes;
}


// Function that implements cooling schedule for simulated annealing
double coolSched(int iter)
{
    // T(iter) = 10^10 * (0.8)^(⌊iter/300⌋)
    return pow(10, 10) * pow(0.8, iter/300);
}


// Function that implements simulated annealing
long long int simAnn(int size)
{
    // Declare and assign random values to S and A
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    
    long long int i,j;
    int* signsP = malloc(sizeof(int) * size); // S'
    int* signsPP = malloc(sizeof(int) * size); // S''
    signsPP = signs;
    
    for(int k = 0; k < MAX_ITER; k++)
    {
        // Declare and assign original values to S'
        signsP = signs;
        
        // Select random indeces
        i = getrand() % (size+1);
        j = getrand() % (size+1);
        while(i == j) // i =/= j
        {
            j = getrand() % (size+1);
        }
        
        // Change S' to be a neighbor of S
        signsP[i] = -1 * signsP[i];
        if(getrand() % 2 == 0)
        {
            signsP[j] = -1 * signsP[j];
        }
        
        // Check if S' is better or if cooling schedule allows for switch regardless
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size) ||(rand() / RAND_MAX) <
           exp(-1*(seqResidue(nums, signs, size) - seqResidue(nums, signsP, size))/coolSched(k)))
        {
            signs = signsP;
        }
        // Check if new or old S is better than S''
        if(seqResidue(nums, signs, size) < seqResidue(nums, signsPP, size))
        {
            signsPP = signs;
        }
    }
    long long int finalRes = seqResidue(nums, signsPP, size);
    free(signsPP);
    return finalRes;
}


// MAIN FOR TESTING
int main()
{
    int seed = time(NULL);
    srand(seed);

    // Pdf example
    long long int array[] = {10,8,7,6,5};
    long long int* arr = array;
    int n = 5;
    
    printf("Residue is: %lld \n", kk(arr,n));
    
    long long int rep = repRand(100);
    long long int hill = hillClimb(100);
    long long int sim = simAnn(100);
    
    printf("Repeated Random Residue is: %lld \n", rep);
    
    printf("Hill Climbing Residue is: %lld \n", hill);
    
    printf("Simulated Annealing Residue is: %lld \n", sim);
    
    
    long long int bestRes;
    char* bestSol;
    
    if(rep < hill)
    {
        bestRes = rep;
        bestSol = "Repeated Rand";
    }
    else
    {
        bestRes = hill;
        bestSol = "Hill Climbing";
    }
    
    if(sim < bestRes)
    {
        bestRes = sim;
        bestSol = "Simulated Annealing";
    }
    
    printf("Best solution is %s with %lld residue \n", bestSol, bestRes);
    
    long long int* a = malloc(sizeof(long long int) * 100);
    a = getrandNums(a,100);
    printf("KK on rand is %lld \n", kk(a,100));
    
    return 0;
}

#include <limits.h>
#include <ctype.h>

#define MAX_ITER 25000


/* Function that runs KK algorithm on an an array of numbers
    Returns the residue of the partition
 */
long long int kk(long long int* nums, int size)
{
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
        //printf("greatest: %lld\n", greatest);
        // Change to 0
        nums[greatestIndex] = 0;
        
        // Find second greatest
        for(j = 0; j < size; j++)
        {
            if(nums[j] > secgreatest)
            {
                secgreatest = nums[j];
                secgreatestIndex  = j;
            }
        }
        //printf("second greatest: %lld\n", secgreatest);
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
    }
    
    // Calculate residue
    long long int residue = 0;
    for(int l = 0; l < size; l++)
    {
        residue += nums[l];
    }
    return residue;
}


// Function that generates 64-bit random number
long long int getrand()
{
    long long int r = (long long int) rand() << 32 | rand();
    return (r % ((long long int) pow(10,12) + 1));
}


// Function that generates entirely random sequence of numbers
long long int* getrandNums(long long int* outnums, int size)
{
    for(int i = 0; i < size; i++)
    {
        outnums[i] = getrand();
    }
    return outnums;
}

// Function that generates random signs
int* getrandSigns(int* outsigns, int size)
{
    long long int num = 0;
    for(int i = 0; i < size; i++)
    {
        num = getrand();
        // Divide probability in half by evens and odds
        if(num % 2 == 0)
        {
            outsigns[i] = 1;
        }
        else
        {
            outsigns[i] = -1;
        }
    }
    
    return outsigns;
}


// Calculate residue for sequence representation
long long int seqResidue(long long int* nums, int* s, int size)
{
    int long long cum = 0;
    for(int i = 0; i < size; i++)
    {
        cum += nums[i] * s[i];
    }
    return llabs(cum);
}



// Function that implements Repeated Random
long long int repRand(int size)
{
    // Declare and assign random values to S
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    int* signsP = malloc(sizeof(int) * size);
    
    
    for(int j = 0; j < MAX_ITER; j++)
    {
        // Declare and assign random values to S'
        signsP = getrandSigns(signsP, size);
        
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size))
        {
            signs = signsP;
        }
    }
    long long int finalRes = seqResidue(nums, signs, size);
    free(signsP);
    return finalRes;
}


// Function that implements Hill Climbing
long long int hillClimb(int size)
{
    // Declare and assign random values to S and A
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    long long int i,j;
    int* signsP = malloc(sizeof(int) * size); // S'
    
    for(int k = 0; k < MAX_ITER; k++)
    {
        // Declare and assign original values to S'
        signsP = signs;
        
        // Select random indeces
        i = getrand() % (size+1);
        j = getrand() % (size+1);
        while(i == j) // i =/= j
        {
            j = getrand() % (size+1);
        }
        
        // Change S' to be a neighbor of S
        signsP[i] = -1 * signsP[i];
        if(getrand() % 2 == 0)
        {
            signsP[j] = -1 * signsP[j];
        }
        // Check if neighbor is better
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size))
        {
            signs = signsP;
        }
    }
    long long int finalRes = seqResidue(nums, signs, size);
    free(signsP);
    return finalRes;
}


// Function that implements cooling schedule for simulated annealing
double coolSched(int iter)
{
    // T(iter) = 10^10 * (0.8)^(⌊iter/300⌋)
    return pow(10, 10) * pow(0.8, iter/300);
}


// Function that implements simulated annealing
long long int simAnn(int size)
{
    // Declare and assign random values to S and A
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    
    long long int i,j;
    int* signsP = malloc(sizeof(int) * size); // S'
    int* signsPP = malloc(sizeof(int) * size); // S''
    signsPP = signs;
    
    for(int k = 0; k < MAX_ITER; k++)
    {
        // Declare and assign original values to S'
        signsP = signs;
        
        // Select random indeces
        i = getrand() % (size+1);
        j = getrand() % (size+1);
        while(i == j) // i =/= j
        {
            j = getrand() % (size+1);
        }
        
        // Change S' to be a neighbor of S
        signsP[i] = -1 * signsP[i];
        if(getrand() % 2 == 0)
        {
            signsP[j] = -1 * signsP[j];
        }
        
        // Check if S' is better or if cooling schedule allows for switch regardless
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size) ||(rand() / RAND_MAX) <
           exp(-1*(seqResidue(nums, signs, size) - seqResidue(nums, signsP, size))/coolSched(k)))
        {
            signs = signsP;
        }
        // Check if new or old S is better than S''
        if(seqResidue(nums, signs, size) < seqResidue(nums, signsPP, size))
        {
            signsPP = signs;
        }
    }
    long long int finalRes = seqResidue(nums, signsPP, size);
    free(signsPP);
    return finalRes;
}


// MAIN FOR TESTING
int main()
{
    int seed = time(NULL);
    srand(seed);

    // Pdf example
    long long int array[] = {10,8,7,6,5};
    long long int* arr = array;
    int n = 5;
    
    printf("Residue is: %lld \n", kk(arr,n));
    
    long long int rep = repRand(100);
    long long int hill = hillClimb(100);
    long long int sim = simAnn(100);
    
    printf("Repeated Random Residue is: %lld \n", rep);
    
    printf("Hill Climbing Residue is: %lld \n", hill);
    
    printf("Simulated Annealing Residue is: %lld \n", sim);
    
    long long int bestRes;
    char* bestSol;
    
    if(rep < hill)
    {
        bestRes = rep;
        bestSol = "Repeated Rand";
    }
    else
    {
        bestRes = hill;
        bestSol = "Hill Climbing";
    }
    
    if(sim < bestRes)
    {
        bestRes = sim;
        bestSol = "Simulated Annealing";
    }
    
    printf("Best solution is %s with %lld residue \n", bestSol, bestRes);
    
    long long int* a = malloc(sizeof(long long int) * 100);
    a = getrandNums(a,100);
    printf("KK on rand is %lld \n", kk(a,100));
    
    return 0;
}
