# Task Relationship Analysis
In priority preemptive scheduling, higher-priority tasks will take over lower-priority tasks that are currently running. If a higher-priority task is ready to run, the scheduler will preempt the lower-priority task and execute the higher-priority task.

## a. Priority Settings
In the code found in this repository, each task in this project is assigned a different priority. In this program, the task for blinking the LED for 0.5 seconds using the `FlashRedLed()` function has a higher priority than the task for blinking the LED for 4 seconds using the `FlashGreenLed()` function. With preemptive scheduling, when the higher-priority task is ready to be executed, the currently running lower-priority task will be suspended and resumed only after the higher-priority task is finished or enters a `Blocked`/`Suspended` state.

For example, in the program, the following tasks exist:
- `FlashRedLedTask` (above normal priority)
- `FlashGreenLedTa` (normal priority)

When both tasks are ready to run, the RTOS will execute the `FlashRedLedTask` first. If the `FlashRedLedTask` is running and the `FlashGreenLedTa` becomes ready, the `FlashGreenLedTa` will only be executed once the `FlashRedLedTask` is completed or delayed (e.g., waiting for an event or `osDelay`). Below is the expected timing diagram for the LED output.

## b. Preemptive Scheduling
Preemptive scheduling means that a higher-priority task will interrupt a lower-priority task if an event makes the higher-priority task ready. This ensures high responsiveness to critical tasks, such as those related to real-time operations.
![timing diagram3](https://github.com/user-attachments/assets/bcbbced1-e355-4804-adf7-0a91be500bca)


# Results When Uploaded to the STM32 Board
When this program is uploaded and executed on an STM32 board with an RTOS (such as FreeRTOS), the expected outcomes are:
- Higher-priority tasks will receive CPU time more frequently than lower-priority tasks.
- If a lower-priority task is running and a higher-priority task becomes ready, the scheduler will stop the lower-priority task and execute the higher-priority task.
- Task switching will be handled automatically by the RTOS scheduler without direct intervention from the main program.
- There is one indicator light that stays on continuously while the light next to it Which should be blinking becomes steady, indicating that the blinking task has been suspended.


https://github.com/user-attachments/assets/e3643a67-c018-44f9-86c7-667bdf5661bc

## Conclusion
The relationship between tasks in this program is based on the priority system enforced by the RTOS. Using preemptive scheduling, the STM32 efficiently executes high-priority tasks when they are ready, while lower-priority tasks are delayed until no higher-priority tasks need to be run. The result is high responsiveness for important tasks with high priority, while lower-priority tasks still get executed when CPU resources are available.
