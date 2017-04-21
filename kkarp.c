//
// kkarp.c
//
//
// Created by 90982966 and 90977369
//
// usage: ./kk inputfile
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


// Calculates residue for pre-partition representation
long long int preResidue(long long int* nums, int* s, int size)
{
    long long int* numsP = calloc(size, sizeof(long long int));
    for(int i = 0; i < size; i++)
    {
        // Iterate through all ps
        for(int j = 0; j < size; j++)
        {
            if(s[j] == i)
            {
                // a′pj =a′pj +aj
                numsP[j] = numsP[j] + nums[i];
            }
        }
    }
    // Run kk on A'
    long long int res = kk(numsP, size);
    free(numsP);
    return res;
}


// Function that implements Repeated Random for sequence representation
long long int repRand(long long int* nums, int size)
{
    // Declare and assign random values to S
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


// Function that generates random solution P in pre-partition representation
int* getrandP(int* outP, int size)
{
    for(int i = 0; i < size; i++)
    {
        // Generate random number [0,n-1]
        outP[i] = rand() % size;
    }
    return outP;
}


// Function that implements Repeated Random for pre-partition representation
long long int prerepRand(long long int* nums, int size)
{
    // Declare and assign values to S
    int* p = malloc(sizeof(int) * size);
    p = getrandP(p, size);
    
    for(int i = 0; i < MAX_ITER; i++)
    {
        // Declare and assign random values to P'
        int* pP = malloc(sizeof(int) * size);
        pP = getrandP(pP, size);
        
        if(preResidue(nums, pP, size) < preResidue(nums, p, size))
        {
            free(p);
            p = pP;
        }
        else
        {
            free(pP);
        }
    }
    long long int finalRes = preResidue(nums, p, size);
    free(p);
    return finalRes;
}


// Function that generates a random neighbor in sequence representation
int* seqNeighbor(int* signs, int size)
{
    int* signsP = malloc(sizeof(int) * size); // S'
    // Declare and assign original values to S'
    for(int i = 0; i < size; i++)
    {
        signsP[i] = signs[i];
    }
    // Select random indices
    int i = rand() % size;
    int j = rand() % size;
    while(i == j) // i =/= j
    {
        j = rand() % size;
    }
    
    // Change S' to be a neighbor of S
    signsP[i] *= -1;
    if(rand()/RAND_MAX < .5)
    {
        signsP[j] *= -1;
    }
    
    return signsP;
}


// Function that generates a random neighbor in pre-partition representation
int* preNeighbor(int* p, int size)
{
    int* pP = malloc(sizeof(int) * size); // P'
    for(int i = 0; i < size; i++)
    {
        pP[i] = p[i];
    }
    // Select random indices
    int i = rand() % size;
    int j = rand() % size;
    while(p[i] == j) // p_i =/= j
    {
        j = rand() % size;
    }
    // Change P' to be a neighbor of P
    pP[i] = j;
    
    return pP;
}


// Function that implements Hill Climbing in sequence representation
long long int hillClimb(long long int* nums, int size)
{
    // Declare and assign random values to S
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    
    for(int k = 0; k < MAX_ITER; k++)
    {
        int* signsP = seqNeighbor(signs, size);
        // Check if neighbor is better
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


// Function that implements Hill Climbing in pre-partition representation
long long int prehillClimb(long long int* nums, int size)
{
    // Declare and assign random values to P
    int* p = malloc(sizeof(int) * size);
    p = getrandP(p, size);
    
    for(int i = 0; i < MAX_ITER; i++)
    {
        int* pP = preNeighbor(p, size);
        // Check if neighbor is better
        if(preResidue(nums, pP, size) < preResidue(nums, p, size))
        {
            free(p);
            p = pP;
        }
        else
        {
            free(pP);
        }
    }
    long long int finalRes = preResidue(nums, p, size);
    free(p);
    return finalRes;
}



// Function that implements cooling schedule for simulated annealing
double coolSched(int iter)
{
    // T(iter) = 10^10 * (0.8)^(⌊iter/300⌋)
    return pow(10, 10) * pow(0.8, iter/300);
}


// Function that implements simulated annealing in sequence representation
long long int simAnn(long long int* nums, int size)
{
    // Declare and assign random values to S
    int* signs = malloc(sizeof(int) * size);
    signs = getrandSigns(signs, size);
    
    int* signsPP = malloc(sizeof(int) * size); // S''
    for(int j = 0; j < size; j++)
    {
        signsPP[j] = signs[j];
    }
    int i = 0;
    for(int k = 0; k < MAX_ITER; k++)
    {
        int* signsP = seqNeighbor(signs, size);
        
        // Check if S' is better or if cooling schedule allows for switch regardless
        if(seqResidue(nums, signsP, size) < seqResidue(nums, signs, size) || (rand() / RAND_MAX) <
           exp(-1*(seqResidue(nums, signs, size) - seqResidue(nums, signsP, size))/coolSched(k)))
        {
            free(signs);
            signs = signsP;
        }
        else
        {
            free(signsP);
        }
        // Check if new or old S is better than S''
        if(seqResidue(nums, signs, size) < seqResidue(nums, signsPP, size))
        {
            for(i = 0; i < size; i++)
            {
                signsPP[i] = signs[i];
            }
        }
    }
    long long int finalRes = seqResidue(nums, signsPP, size);
    free(signs);
    free(signsPP);
    return finalRes;
}


// Function that implements simulated annealing in pre-partitioning representation
long long int presimAnn(long long int* nums, int size)
{
    // Declare and assign random values to P
    int* p = malloc(sizeof(int) * size);
    p = getrandP(p, size);
    
    int* pPP = malloc(sizeof(int) * size); // P''
    for(int i = 0; i < size; i++)
    {
        pPP[i] = p[i];
    }
    
    int i = 0;
    for(int k = 0; k < MAX_ITER; k++)
    {
        int* pP = preNeighbor(p, size);
        
        // Check if P' is better or if cooling schedule allows for switch regardless
        if(preResidue(nums, pP, size) < preResidue(nums, p, size) || (rand() / RAND_MAX) <
           exp(-1*(preResidue(nums, p, size) - preResidue(nums, pP, size))/coolSched(k)))
        {
            free(p);
            p = pP;
        }
        else
        {
            free(pP);
        }
        // Check if new or old S is better than S''
        if(preResidue(nums, p, size) < preResidue(nums, pPP, size))
        {
            for(i = 0; i < size; i++)
            {
                pPP[i] = p[i];
            }
        }
    }
    long long int finalRes = preResidue(nums, pPP, size);
    free(p);
    free(pPP);
    return finalRes;
}


// Function that prints array of size size
void printArray(long long int* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%lld \n", arr[i]);
    }
}



/*
 Real main that reads in file and then does 100 trials with 
 25000 iterations on each heuristic using both representations
 
 usage: ./kk inputfile
 */
int main(int argc, char* argv[])
{
    int size = 100;
    
    // PART 1 ---------------------
    // Read in text file
    char const* const filename = argv[1];
    FILE* file = fopen(filename, "r");
    char curr_line[64];
    long long int* inputNums = malloc(sizeof(long long int) * size);
    
    // Fill array with input
    for(int h = 0; h < size; h++)
    {
        fgets(curr_line, sizeof curr_line, file);
        inputNums[h] = atoi(&(curr_line[0]));
    }
    
    long long int kkarp = kk(inputNums, size);
    printf("%lld \n", kkarp);
    fclose(file);
    
    // PART 2 ---------------------
    long long int prep;
    long long int phill;
    long long int psim;
    long long int rep;
    long long int hill;
    long long int sim;
    long long int kkres;
    
    // Generate 100 instances of random sequences/prepartitions and run all 7s algorithms
    for(int j = 0; j < size; j++)
    {
        long long int* nums = malloc(sizeof(long long int) * size);
        nums = getrandNums(nums, size);
        
        // Run algorithms
        rep = repRand(nums, size);
        hill = hillClimb(nums, size);
        sim = simAnn(nums, size);
        prep = prerepRand(nums, size);
        phill = prehillClimb(nums, size);
        psim = presimAnn(nums, size);
        kkres = kk(nums, size);
        
        // Print algorithms
        printf("%lld\n", kkres); // kk
        printf("%lld\n", rep); // Repeated Random sequence
        printf("%lld\n", hill); // Hill Climbing sequence
        printf("%lld\n", sim); // Simulated Annealing sequence
        printf("%lld\n", prep); // Repeated Random pre-partition
        printf("%lld\n", phill); // Hill Climbing pre-partition
        printf("%lld\n", psim); // Simulated Annealing pre-partition
        free(nums);
    }
    return 0;
}


/*
// MAIN FOR TESTING
int main()
{
    
    int size = 100;
    int seed = time(NULL);
    srand(seed);
    */
    /*
     // Pdf example
     long long int array[] = {10,8,7,6,5};
     long long int* arr = array;
     int n = 5;
     
     printf("Residue is: %lld \n", kk(arr,n));*/
    
    
    /*
     // Declare and assign random values to S and A
     long long int* nums = malloc(sizeof(long long int) * size);
     nums = getrandNums(nums, size);
     long long int rep = repRand(nums, size);
     long long int hill = hillClimb(nums, size);
     long long int sim = simAnn(nums, size);
     
     
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
*/
    
   /*
    int n = 5;
    long long int* nums = malloc(sizeof(long long int) * n);
    long long int kkarp;
    long long int rand;
    long long int hill;
    long long int ann;
    long long int randP;
    long long int hillP;
    long long int annP;
    
    // Iterate and get results, storing them by index
    for(int i = 0; i < n; i++)
    {
        // Re-assign random numbers
        nums = getrandNums(nums, n);
        
        // Print
        printf("%lld \n", kk(nums, n));
        printf("%lld \n", repRand(nums, n));
        printf("%lld \n", hillClimb(nums, n));
        printf("%lld \n", simAnn(nums, n));
        printf("%lld \n", prerepRand(nums, n));
        printf("%lld \n", prehillClimb(nums, n));
        printf("%lld \n", presimAnn(nums, n));
    }
    */
    
    /*
    long long int* nums = malloc(sizeof(long long int) * size);
    nums = getrandNums(nums, size);
    long long int prep = prerepRand(nums, size);
    long long int phill = prehillClimb(nums, size);
    long long int psim = presimAnn(nums, size);
    long long int rep = repRand(nums, size);
    long long int hill = hillClimb(nums, size);
    long long int sim = simAnn(nums, size);
    printf("Repeated Random Residue is: %lld \n", rep);
    printf("Hill Climbing Residue is: %lld \n", hill);
    printf("Simulated Annealing Residue is: %lld \n", sim);
    printf("Pre-Partition Repeated Random Residue is: %lld \n", prep);
    printf("Pre-Partition Hill Climbing Residue is: %lld \n", phill);
    printf("Pre-Partition Simulated Annealing Residue is: %lld \n", psim);
    */
    
    /*
    long long int prep;
    long long int phill;
    long long int psim;
    long long int rep;
    long long int hill;
    long long int sim;
    
    int prepTime = 0;
    int phillTime = 0;
    int psimTime = 0;
    int repTime = 0;
    int hillTime = 0;
    int simTime = 0;
    

    
    for(int j = 0; j < size; j++)
    {
        long long int* nums = malloc(sizeof(long long int) * size);
        nums = getrandNums(nums, size);
        
        //For timing
        clock_t start1 = clock(), diff1;
        
        prep = prerepRand(nums, size);
        
        // For timing
        diff1 = clock() - start1;
        prepTime += diff1 * 1000 / CLOCKS_PER_SEC;
    
        //For timing
        clock_t start2 = clock(), diff2;
        
        phill = prehillClimb(nums, size);
        
        // For timing
        diff2 = clock() - start2;
        phillTime += diff2 * 1000 / CLOCKS_PER_SEC;
    
        //For timing
        clock_t start3 = clock(), diff3;
        
        psim = presimAnn(nums, size);
        
        // For timing
        diff3 = clock() - start3;
        psimTime += diff3 * 1000 / CLOCKS_PER_SEC;
        
        //For timing
        clock_t start4 = clock(), diff4;
        
        rep = repRand(nums, size);
        
        // For timing
        diff4 = clock() - start4;
        repTime += diff4 * 1000 / CLOCKS_PER_SEC;
        
        //For timing
        clock_t start5 = clock(), diff5;
        
        hill = hillClimb(nums, size);
        
        // For timing
        diff5 = clock() - start5;
        hillTime += diff5 * 1000 / CLOCKS_PER_SEC;
        
        //For timing
        clock_t start6 = clock(), diff6;
        
        sim = simAnn(nums, size);
        
        // For timing
        diff6 = clock() - start6;
        simTime += diff6 * 1000 / CLOCKS_PER_SEC;
        
    
     
        printf("%lld\n", hill);
        printf("%lld\n", sim);
        printf("%lld\n", prep);
        printf("%lld\n", phill);
        printf("%lld\n", psim);
        free(nums);
    }

    prepTime = prepTime/size;
    phillTime = phillTime/size;
    psimTime = psimTime/size;
    repTime = repTime/size;
    hillTime = hillTime/size;
    simTime = simTime/size;
    
    printf("Average miliseconds: %i\n", repTime);
    printf("Average miliseconds: %i\n", hillTime);
    printf("Average miliseconds: %i\n", simTime);
    printf("Average miliseconds: %i\n", prepTime);
    printf("Average miliseconds: %i\n", phillTime);
    printf("Average miliseconds: %i\n", psimTime);
    */

    /*
    long long int kkres;
    int kkTime = 0;
    
    for(int j = 0; j < size; j++)
    {
        long long int* nums = malloc(sizeof(long long int) * size);
        nums = getrandNums(nums, size);
        
        //For timing
        clock_t startk = clock(), diffk;
        
        kkres = kk(nums, size);
        
        // For timing
        diffk = clock() - startk;
        kkTime += diffk * 1000 / CLOCKS_PER_SEC;
        
        printf("%lld\n", kkres);
        
        free(nums);
    }
    
    kkTime = kkTime/size;
    printf("Average miliseconds: %i\n", kkTime);
    
    
    return 0;
}
*/
