#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <iostream>
#include <iomanip>

class timer {
  friend std::ostream& operator<<(std::ostream& os, timer& t);

 private:
  bool running;
  std::chrono::time_point<std::chrono::system_clock> start_time;
  std::chrono::duration<double> acc_time;

  double elapsed_time_since_last_start();

 public:
  timer() : running(false), start_time(), acc_time(0.0) { }

  void start(const char* msg = 0);
  void restart(const char* msg = 0);
  void stop(const char* msg = 0);
  void check(const char* msg = 0);

  double elapsed_time();
};

//===========================================================================
// Return the total time that the timer has been in the "running"
// state since it was last "started" or "restarted".

inline double timer::elapsed_time_since_last_start() {
  return (std::chrono::system_clock::now() - start_time).count() / 1e6;
}

//===========================================================================
// Return the total time the timer has been in the "running" state.

inline double timer::elapsed_time() {
  return (acc_time.count() +
    (running ? elapsed_time_since_last_start() : 0.0));
}

//===========================================================================
// Start the timer if it is not already running.  Print an optional message.

inline void timer::start(const char* msg) {
  // Print an optional message, something like "Starting timer t";
  if (msg) {
    std::cout << msg << std::endl;
  }
  if (!running) {
    running = true;
    start_time = std::chrono::system_clock::now();
  }
}

//===========================================================================
// Reset the accumulated time and start the timer.
// Print an optional message.

inline void timer::restart(const char* msg) {
  // Print an optional message, something like "Restarting timer t";
  if (msg) {
    std::cout << msg << std::endl;
  }

  // Set timer status to running, reset accumulated time, and set start time.
  running = true;
  acc_time = std::chrono::duration<double>(0.0);
  start_time = std::chrono::system_clock::now();
}

//===========================================================================
// Stop the timer and print an optional message.

inline void timer::stop(const char* msg) {
  // Print an optional message, something like "Stopping timer t";
  if (msg) {
    std::cout << msg << std::endl;
  }

  // Compute accumulated running time and set timer status to not running
  if (running) {
    acc_time += std::chrono::duration<double>(elapsed_time_since_last_start());
    running = false;
  }
}

//===========================================================================
// Print out an optional message followed by the current timer timing.

inline void timer::check(const char* msg) {
  // Print an optional message, something like "Checking timer t";
  if (msg) std::cout << msg << " : ";

  std::cout << "Elapsed time [" << std::setiosflags(std::ios::fixed)
            << std::setprecision(2)
            << acc_time.count() +
               (running ? elapsed_time_since_last_start() : 0.0)
            << "] seconds\n";
}

//===========================================================================
// Allow timers to be printed to ostreams using the syntax 'os << t'
// for an ostream 'os' and a timer 't'.  For example, "cout << t" will
// print out the total amount of time 't' has been "running".

inline std::ostream& operator<<(std::ostream& os, timer& t) {
  os << std::setprecision(2) << std::setiosflags(std::ios::fixed)
    << t.elapsed_time();
  return os;
}

#endif  // TIMER_H_

