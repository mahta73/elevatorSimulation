void pushSourceButton(int& floor) {
  // the user trys to modify the source queue
  data_enter.wait();

  if (capacity.count > 0) {

  // One source (the current floor of the user) is added to the sourceQueue
  sourceQueue.push_back(floor);

  Message(" * FLOOR --->", floor);

  // Message("source queue size", sourceQueue.size());
  // Message("queue source element front ", sourceQueue.front());

  data_enter.signal();

  // the user is signalling to the elevator that he or she has already determined the floor
  Source.signal();

  } else {
      data_enter.signal();
      Message("Sorry, There is not enough room in the elevator; capacity is", capacity.count);
      srand(time(NULL));
      sleep((rand() % 11 ) + 10);
      pushSourceButton(floor);
  }
}

void pushDestinationButton(int& destination) {
  // the user is waiting for the elevator to reach his or her floor
  elevatorReached.wait();

  // user trys to lock the elevator (The elevator door will get opened)
  accessElevator.wait();
  Log(" ] [ ");

  // Now the door of the elevator is opened and the user must check if there is enough space for him or her

    // the users entered the elevator
    Log(" ] --> :) <-- [");


    capacity.wait();

    Message("Capacity is", capacity.count);

    // capacity--;
    //Message(" -> /_ / capacity", capacity.count);

    srand(time(NULL));
    delay = rand() % 11;

    if ( delay <= 5) {
    sleep(delay);
    // the user trys to modify the destination queue
    data_enter.wait();

    // One destination (the current user's destination) is added to the destinationQueue
    destinationQueue.push_back(destination);

    Message(" # FLOOR ---> ", destination);

    // Message("destination queue size", destinationQueue.size());
    // Message("queue destination element front ", destinationQueue.front());

    data_enter.signal();

    // the user is signalling to the elevator that he or she has already determined the destination
    Destination.signal();

    // the use unlocks the elevator
    Log(" [] ");

    accessElevator.signal();
  } else {
    sleep(5);

    Log("Time out");
    capacity.signal();

    Log(" [] ");
    accessElevator.signal();

    Destination.signal();
  }
}

void useleaves() {
  // the elevator is trying to lock itself
  accessElevator.wait();
  Log(" ] [ ");

  // One user leaved the elevator
  capacity.signal();

  Log(" ] [ ->-> :D ");

  // the elevator unlocked itself
  Log(" [] ");

  accessElevator.signal();
}

int GOTO(std::deque <int> Q) {
  std::mutex mtx1;
  std::unique_lock<std::mutex> locker(mtx1);
  GoTo = Q.front();
  for (unsigned int i = 0; i < Q.size(); i++) {
    if (Q[i] == 5) {
      if (priority_count < 2) {
          GoTo = Q[i];
          priority_count++;
          break;
        break;
        } else {
          priority_count--;
          break;
      }
    }
  }
  return GoTo;
}

void checkSourceQueue(Binary_Semaphore& BS) {
  std::mutex mtx2;
  std::unique_lock<std::mutex> locker(mtx2);
  for (unsigned int i = 0; i < sourceQueue.size(); i++) {

    // if there is any element is sourceQueue wich is equal to the current floor of the elevator
    if (sourceQueue[i] == current_floor_of_the_elevator) {

      Message("This element will be eliminated from the sourceQueue", sourceQueue[i]);

      // eliminate that element form the sourceQueue
      data_enter.wait();
      sourceQueue.erase(sourceQueue.begin() + i);
      data_enter.signal();

      i--;

      // signal a user that the elevator has arrived
      BS.signal();

   }
 }
}

void checkDestinationQueue() {
  std::mutex mtx3;
  std::unique_lock<std::mutex> locker(mtx3);

  for (unsigned int i = 0; i < destinationQueue.size(); i++) {

    if (destinationQueue[i] == current_floor_of_the_elevator) {

      Message("This element will be eliminated from the destinationQueue", destinationQueue[i]);

      // eliminate that element form the destinationQueue
      data_enter.wait();
      destinationQueue.erase(destinationQueue.begin() + i);
      data_enter.signal();

      i--;

      // the user has reched its destination
      useleaves();

   }
 }
}

void moveDownWard() {
  std::mutex mtx4;
  std::unique_lock<std::mutex> locker(mtx4);
  // it takes one second
  sleep(1);

  // get one floor closer to the elevator's destination
  accessElevator.wait();
  current_floor_of_the_elevator--;
  accessElevator.signal();

  // print the floor on which the elevator currently is
  Message("E| f|", current_floor_of_the_elevator);
}

void moveUpward() {
  std::mutex mtx5;
  std::unique_lock<std::mutex> locker(mtx5);
  // it takes one seocnd
  sleep(1);
  // get one floor closer to the elevator's destination
  accessElevator.wait();
  current_floor_of_the_elevator++;
  accessElevator.signal();

  // print the floor on which the elevator currently is
  Message("E| f|", current_floor_of_the_elevator);
}
