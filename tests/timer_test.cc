#include <chrono>
#include <iostream>
#include <thread>

#include "timer.h"
#include "gtest/gtest.h"

namespace {

TEST(Timer, All) {
  timer t;
  double elapsed_seconds = 0.0;

  // Start the timer and check it after 1.5 seconds.
  // The string argument to start() is optional.
  t.start("Timer started");
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  elapsed_seconds = t.elapsed_time();
  EXPECT_GT(elapsed_seconds, 1.5);
  EXPECT_LT(elapsed_seconds, 1.6);
  std::cout << "Start: " << t << std::endl;

  // Check again after another 1.5 seconds.
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  elapsed_seconds = t.elapsed_time();
  EXPECT_GT(elapsed_seconds, 3.0);
  EXPECT_LT(elapsed_seconds, 3.1);
  // Use 'check' member function for a default printing method.
  t.check("Wait");

  // Check again after restarting and waiting another 1.5 seconds.
  t.restart("Timer restarted");
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  elapsed_seconds = t.elapsed_time();
  EXPECT_GT(elapsed_seconds, 1.5);
  EXPECT_LT(elapsed_seconds, 1.6);
  t.check("Restart");

  // Stop the timer and wait another 1.5 seconds.
  // The elapsed time should not change.
  t.stop("Timer stopped");
  double initial_time = t.elapsed_time();
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  elapsed_seconds = t.elapsed_time();
  EXPECT_EQ(initial_time, elapsed_seconds);
  EXPECT_GT(elapsed_seconds, 1.5);
  EXPECT_LT(elapsed_seconds, 1.6);
  t.check("Stop");

  // Start the timer again.  Since there is no restart, the timer
  // will start from where it left off the last time it was stopped.
  t.start("Timer started, not restarted");
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  elapsed_seconds = t.elapsed_time();
  EXPECT_GT(elapsed_seconds, 3.0);
  EXPECT_LT(elapsed_seconds, 3.1);
  t.check("Start without restart");
}

}  // namespace
