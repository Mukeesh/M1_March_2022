/**
 * @file quiz_operations.h
 * @author Mukeesh.K (mmuki651@gmail.com)
 * @brief Header file for Quiz Game 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _QUIZ_GAME_OPERATIONS_H_
#define _QUIZ_GAME_OPERATIONS_H_

/**
 * @brief First Intialize an array and then allocate memory for that array
 * @return Initialize array
 */
void initArray();

/**
 * @brief Starting the array and it goes to infinity size 
 * @return Array Initialized
 */
void insertArray();
/**
 * @brief Array will be Null 
 * @return Zero
 */
void freeArray();

/**
 * @brief First Initialize an array and then allocate memory for that array 
 * @return Array Initialized
 */
void initArrayRound();
/**
 * @brief Starting the array and it goes to infinity size
 * @return Array Initialized
 */
void insertArrayRound();

/**
 * @brief Array will be Null  
 * @return Zero
 */
void freeArrayRound();

/**
 * @brief Client Server Request  
 * @return Connection
 */
void waitForClients();

/**
 * @brief Play game 
 * @return Points
 */
void game();

/**
 * @brief Questions from a file
 * @return Gives correct answer
 */
void XMLParser();

/**
 * @brief Used to find the correct answer
 * @return Gives correct answer
 */
void printRound();

/**
 * @brief Used to show points 
 * @return Result
 */
int printResult();