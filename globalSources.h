#pragma once

std::mutex m;
#define Log(x) m.lock(); std::cout << x << std::endl; m.unlock();
#define Message(message, x) m.lock(); std::cout << message << " " << x << std::endl; m.unlock();
#define TimeLog(message, x) m.lock(); std::cout << message << " " << x << " seconds" << std::endl; m.unlock();

// check if the elevator has arrived , (for source)
Binary_Semaphore elevatorReached(0);

// check if you can edit either sourceQueue or destinationQueue
Binary_Semaphore data_enter(1);

// initially you can access the elevator, (Attempt to open the door of the elevator)
Primitive_Semaphore accessElevator(1);

// wait for a source, signal a source is added
Primitive_Semaphore Source(0);

// wait for a destination, signal a destination is added
Primitive_Semaphore Destination(0);

// define the souurce with the highest priority and go through it
Primitive_Semaphore goto_source(1);

// define the destination with the highest priority and go through it
Primitive_Semaphore goto_destination(1);

Primitive_Semaphore capacity(4);


// array of sources to which the elevator must go
std::deque <int> sourceQueue;

// array of destinations to which the elevator must go
std::deque <int> destinationQueue;

// the highest priority destination of the elevator
int GoTo;

// the time takes for the user to define the destination
int delay;

// the fifth floor has 2 times higher priority
int priority_count = 0;

// The current floor of the elevator
int current_floor_of_the_elevator = 0;
