#include "types.h"
#include "user.h"

int
main(void)
{
  int count, total_weight, period;
  int expected_period, expected_weight;
  int i;

  // Create several processes to increase the tree count
  for(i = 0; i < 40; i++){
    if(fork() == 0){
      for(int j = 0; j < 50000000; j++){
        asm volatile("nop");
      }
      exit();
    }
  }

  sleep(5);  // Allow processes to be inserted

  gettreeinfo(&count, &total_weight, &period);
  expected_period = (count > (32 / 2)) ? count * 2 : 32;
  expected_weight = 1024*count;

  printf(1, "Tree Info - Count: %d, Total Weight: %d, Period: %d\n", count, total_weight, period);
  printf(1, "Expected Period: %d\n", expected_period);

  if(period == expected_period){
    printf(1, "Test Passed: Period calculated correctly\n");
  } else {
    printf(1, "Test Failed: Period calculation incorrect\n");
  }
  if(total_weight == expected_weight){
    printf(1, "Test Passed: Total weight calculated correctly\n");
  } else {
    printf(1, "Test Failed: Total weight calculation incorrect\n");
  }

  // Clean up child processes
  for(i = 0; i < 40; i++)
    wait();

  gettreeinfo(&count, &total_weight, &period);
  printf(1, "Tree Info after cleanup - Count: %d, Total Weight: %d, Period: %d\n", count, total_weight, period);

  if (count == 0 && total_weight == 0 && period == 32) {
    printf(1, "Test Passed: Tree cleaned up correctly\n");
  } else {
    printf(1, "Test Failed: Tree not cleaned up correctly\n");
  }

  printf(1, "Test completed\n");
  exit();
}