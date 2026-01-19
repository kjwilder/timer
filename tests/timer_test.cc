#include <chrono>
#include <iostream>
#include <sstream>
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

TEST(Timer, IsRunning) {
  timer t;

  // Timer should not be running initially
  EXPECT_FALSE(t.is_running());

  // Start the timer
  t.start();
  EXPECT_TRUE(t.is_running());

  // Stop the timer
  t.stop();
  EXPECT_FALSE(t.is_running());

  // Restart the timer
  t.restart();
  EXPECT_TRUE(t.is_running());

  // Reset the timer
  t.reset();
  EXPECT_FALSE(t.is_running());
}

TEST(Timer, Reset) {
  timer t;

  // Start timer and accumulate time
  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  t.stop();
  double elapsed = t.elapsed_time();
  EXPECT_GT(elapsed, 0.0);

  // Reset should clear accumulated time and stop timer
  t.reset();
  EXPECT_FALSE(t.is_running());
  EXPECT_EQ(t.elapsed_time(), 0.0);
}

TEST(Timer, DoubleStart) {
  timer t;

  // Start the timer
  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Starting again should have no effect
  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  double elapsed = t.elapsed_time();
  // Should be around 1 second total
  EXPECT_GT(elapsed, 1.0);
  EXPECT_LT(elapsed, 1.1);
}

TEST(Timer, DoubleStop) {
  timer t;

  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  t.stop();

  double first_stop = t.elapsed_time();

  // Stopping again should have no effect
  t.stop();
  EXPECT_EQ(t.elapsed_time(), first_stop);
}

TEST(Timer, ConstCorrectness) {
  timer t;
  t.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test const methods work with const reference
  const timer& const_ref = t;

  EXPECT_TRUE(const_ref.is_running());
  EXPECT_GT(const_ref.elapsed_time(), 0.0);

  // check() should work on const reference
  const_ref.check("Const check");

  // Stream operator should work with const reference
  std::ostringstream oss;
  oss << const_ref;
  EXPECT_FALSE(oss.str().empty());
}

}  // namespace
