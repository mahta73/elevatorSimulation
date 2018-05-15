#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <time.h>
#include <cmath>
#include <queue>

#include "semaphore.h"

#include "globalSources.h"

#include "Func.h"

// people who want to use the elevator
void USER(int floor, int destination) {
  Log("***User***");

  pushSourceButton(floor);

  pushDestinationButton(destination);

}

// the elevator
void ELEVATOR() {
  Log("***Elevator***");

  // print the floor on which the elevator currently is
  Message("E| f|", current_floor_of_the_elevator);

  // the elevator is waiting for a customer
  Log(" | ..::...:::....:::: Elevator ::::....:::...::.. | ");

  // the elevator works infinitely
  while (true) {

    // the elevator is waiting for a source
    Source.wait();

    // if the sourceQueue is not empty
    if ( !sourceQueue.empty() ) {

      // The elevator trys to move
      goto_source.wait();

      // define the destination for the elevator with the highest priority
      GOTO(sourceQueue);

      // Message("GOTO THE SOURCE FLOOR -> -> ->", GoTo);

      // if the current floor of the elevator is grater than or equal to the destination of the elevator
      if (current_floor_of_the_elevator >= GoTo ) {

        while(current_floor_of_the_elevator >= GoTo ) {

          // check the entire elements existing int the sourceQueue
          checkSourceQueue(elevatorReached);

         // if there is any element is destinationQueue wich is equal to the current floor of the elevator
         checkDestinationQueue();

        // if the elevator has reached its destination
        if ( current_floor_of_the_elevator == GoTo) {
          // break the loop
          break;
        } else {

          moveDownWard();
        }
      }
      // elevator reached its destination
      goto_source.signal();
    }

    // if the current floor of the elevator is less than the destination of the elevator
    else if (GoTo > current_floor_of_the_elevator) {

      while(current_floor_of_the_elevator <= GoTo ) {

        // if there is any element is sourceQueue wich is equal to the current floor of the elevator
        checkSourceQueue(elevatorReached);

         // if there is any element is sourceQueue wich is equal to the current floor of the elevator
         checkDestinationQueue();

          // if the elevator has reached its destination
          if ( current_floor_of_the_elevator == GoTo) {
            // break the loop
            break;
          } else {

            moveUpward();
          }
        }
    // elevator reached its destination
    goto_source.signal();
    }
  }

  // the elevator is waiting for a destination
  Destination.wait();

  if ( !destinationQueue.empty() ) {


    // The elevator trys to move
    goto_destination.wait();


    // define the destination for the elevator with the highest priority
    GOTO(destinationQueue);

    // Message("GOTO THE SOURCE FLOOR -> -> ->", GoTo);

    // if the current floor of the elevator is grater than or equal to the destination of the elevator
    if (current_floor_of_the_elevator >= GoTo ) {

      while(current_floor_of_the_elevator >= GoTo ) {

          // check the entire elements existing int the destinationQueue
          checkDestinationQueue();

         // check the entire elements existing int the sourceQueue
         checkSourceQueue(elevatorReached);

        // if the elevator has reached its destination
        if ( current_floor_of_the_elevator == GoTo) {
          // break the loop
          break;
        } else {

          moveDownWard();
        }
      }
      // elevator reached its destination
      goto_destination.signal();
    }

    // if the current floor of the elevator is less than the destination of the elevator
    else if (GoTo > current_floor_of_the_elevator) {

      while(current_floor_of_the_elevator <= GoTo ) {

          // check the entire elements existing int the destinationQueue
          checkDestinationQueue();

         // check the entire elements existing int the sourceQueue
         checkSourceQueue(elevatorReached);

        // if the elevator has reached its destination
        if ( current_floor_of_the_elevator == GoTo) {
          // break the loop
          break;
        } else {

          moveUpward();
        }
      }
    // elevator reached its destination
    goto_destination.signal();
  }
}

  }
}

int main() {

  // random number of users want to utilize the elevator
  srand(time(NULL));
  const int numberOfTotalUsers = rand() % 11;
  Message("Number Of Total Users is equal to",numberOfTotalUsers);

  // if there won't be any users, exit from the program
  if (numberOfTotalUsers == 0) {
    Log("Unfortunately, there is no user");
    exit(0);
  }

  // list of user threads
  std::vector<std::thread> threadList;

  // the elevator thread
  std::thread Elevator(ELEVATOR);

  for(unsigned int i = 0; i < numberOfTotalUsers; i++)
  {
      // users randomly come to use the elevator
      const int timeToSleep = rand() % 5;
      TimeLog(" >>>ENTER: A user will come in", timeToSleep);
      sleep(timeToSleep);

      // add to the thread vector
      threadList.push_back( std::thread( USER, (rand() % 11 ), (rand() % 11) ) );
  }

  // join user threadds
  for(unsigned int i = 0; i < numberOfTotalUsers; i++)
  {
    threadList[i].join();
  }

  // join the elevator
  Elevator.join();
}
