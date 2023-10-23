English | [中文](专业版_CN.md)

# Professional Version of Task Creation

## Task Management

Understanding task management in FreeRTOS is fundamental to mastering task creation and scheduling.

### Processing Time Allocation:
FreeRTOS is a preemptive real-time operating system, managing the allocation of processing time through time slicing and task priority. Each task is assigned a priority, and among multiple tasks of the same priority, processor time can be shared through time slicing.

- Time Slicing:
If time slicing is enabled, tasks of the same priority will alternate during each time slice. Each task runs within a time slice, then control is passed to the next task of the same priority. This ensures all tasks of the same priority receive processing time.

### Task Selection and Execution:
The FreeRTOS Scheduler decides which task should run at any given moment. It usually employs a priority scheduling algorithm to determine the order of task execution.

- Priority Scheduling:
Each task has an associated priority. The scheduler always selects the highest priority ready task to run. If there are multiple highest priority ready tasks, the scheduler will share processor time among them based on the selected scheduling strategy (such as time slicing).

### Impact of Task Priority on System Behavior:
The way task priority affects system behavior mainly manifests in the following points:

- Task Preemption:
High priority tasks can preempt low priority tasks. That is, when a high priority task becomes ready, it can run immediately, even if a low priority task is running.

- Priority Inversion:
Priority inversion is a common problem in real-time systems, occurring when a low priority task holds a resource needed by a high priority task. FreeRTOS provides a priority inheritance protocol to solve the priority inversion problem.

### Task States:
The state of a task describes the condition or stage a task is currently in. In FreeRTOS, tasks can exist in several states:

- Ready:
In this state, the task is ready to run but is waiting for the scheduler to schedule it.

- Running:
The task is currently executing its code.

- Blocked:
The task is waiting for some condition to be met, such as waiting for a period or an event to occur, it does not consume processor time.

- Suspended:
The task is temporarily stopped and will not be scheduled by the scheduler until explicitly resumed.

- Deleted:
The task is deleted and no longer exists in the system.

Each state corresponds to different stages of a task in the system, understanding the states of tasks is crucial to understanding the lifecycle of tasks and the behavior of the system.

## Queue Management

Applications based on FreeRTOS consist of a set of independent tasks — each task is a small program with independent authority. These independent tasks are likely to communicate with each other to provide useful system functions. All communication and synchronization mechanisms in FreeRTOS are based on queues.

### Creating Queues

In FreeRTOS, queues are created by calling the xQueueCreate() function. When creating a queue, you need to specify the number of elements the queue can hold and the size of each element.
~~~c
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
~~~
- uxQueueLength: This parameter specifies the maximum number of elements the queue can hold.
- uxItemSize: This parameter specifies the size (in bytes) of each element.
### Queue Data Management
Queues internally use the First-In-First-Out (FIFO) principle to manage data. This means that the first data sent to the queue will be the first data received from the queue.

### Sending Data to Queue

You can use the `xQueueSend()` or `xQueueSendToFront()` functions to send data to a queue.
~~~c
BaseType_t xQueueSend( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );
~~~
- xQueue: The queue handle, specifying the queue to send data to.
- pvItemToQueue: A pointer to the data to be sent.
- xTicksToWait: If the queue is full, this parameter specifies how long the task should block waiting for space in the queue to become available.

### Receiving Data from Queue

You can use the `xQueueReceive()` function to receive data from a queue.
~~~c
BaseType_t xQueueReceive( QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait );
~~~
- xQueue: The queue handle, specifying the queue to receive data from.
- pvBuffer: A pointer to the buffer to receive data.
- xTicksToWait: If the queue is empty, this parameter specifies how long the task should block waiting for data to become available.

### Queue Blocking
Queue blocking refers to the scenario when a task attempts to retrieve data from an empty queue or send data to a full queue, causing the task to enter a blocked state as it waits for the queue to become available.

### Impact of Task Priority
When multiple tasks attempt to access the same queue, higher priority tasks will be granted access first. Additionally, when sending data to and receiving data from a queue, higher priority tasks may preempt lower priority tasks. By setting task priorities sensibly and utilizing queues, effective communication and synchronization among tasks can be achieved.

## Interrupt Management

### FreeRTOS API Usable in Interrupt Service Routines
FreeRTOS provides some APIs designed specifically for Interrupt Service Routines (ISR). These functions typically end with "FromISR", such as `xQueueSendFromISR()`, `xSemaphoreGiveFromISR()`, etc. Utilizing these APIs ensures safe interaction with FreeRTOS objects like queues and semaphores within ISR.

### Delayed Interrupt Handling
Delayed interrupt handling refers to setting a flag within the ISR, and then handling the interrupt in lower priority code. This scheme minimizes interrupt processing time, thus improving system real-time performance. In FreeRTOS, binary semaphores or direct task notifications are commonly used to implement delayed interrupt handling.

### Creation and Usage of Binary and Counting Semaphores
- Binary Semaphore: Created using `xSemaphoreCreateBinary()`, primarily used for synchronization and mutual exclusion.
- Counting Semaphore: Created using `xSemaphoreCreateCounting()`, used for tracking resource counts.

### Difference Between Binary and Counting Semaphores
Binary semaphores can only have two states (0 or 1), while counting semaphores can have more states, used for tracking the quantity of available resources.

### Utilizing Queues to Pass Data in Interrupt Service Routines
By using `xQueueSendFromISR()` and `xQueueReceiveFromISR()` functions, data can be sent to and retrieved from queues within ISR, facilitating communication between ISR and tasks.

### FreeRTOS Interrupt Nesting Model
FreeRTOS supports interrupt nesting, meaning high-priority interrupts can preempt currently executing low-priority interrupts. The implementation of the interrupt nesting model depends on specific hardware and compilers.

## Resource Management

There's a potential risk in multi-tasking systems. When a task is utilizing a resource but hasn't completely finished accessing it, it might be preempted, leaving the resource in an inconsistent or incomplete state. If another task or interrupt accesses the resource at this time, it could lead to data corruption or similar errors.

### Necessity of Resource Management
In a multi-tasking system, resource management is a crucial component to ensure stable system operation. When multiple tasks attempt to access shared resources, some mechanism is needed to control access to the resources, preventing data conflicts and inconsistencies.

### Critical Section
A critical section refers to temporarily disabling interrupts in the code to protect access to shared resources. Interrupts are disabled before entering the critical section and re-enabled upon exit. FreeRTOS provides `taskENTER_CRITICAL()` and `taskEXIT_CRITICAL()` functions to implement critical section protection.

### Mutual Exclusion (Mutex)
Mutual exclusion is a synchronization mechanism to ensure that only one task can access shared resources at any time. It is usually implemented through a mutex.

### Suspending the Scheduler
Suspending the scheduler implies temporarily stopping task switching, typically used to protect short critical sections. FreeRTOS provides `vTaskSuspendAll()` and `xTaskResumeAll()` functions to suspend and resume the scheduler.

### Using Mutexes
FreeRTOS provides `xSemaphoreCreateMutex()` function to create a mutex, and `xSemaphoreTake()` and `xSemaphoreGive()` functions to acquire and release a mutex.

### Creation and Usage of Daemon Tasks

Daemon tasks are typically used to perform background processing or cleanup tasks within the system. The `xTaskCreate()` function can be used to create daemon tasks, and a lower priority can be set to ensure they don't block other important tasks.

### Priority Inversion and Priority Inheritance

- Priority inversion occurs when a lower-priority task holds a shared resource, preventing a higher-priority task from executing, which could potentially lead to system response delays.
- Priority inheritance is a technique to mitigate priority inversion issues. When a higher-priority task waits for a lower-priority task to release a resource, the lower-priority task temporarily inherits the priority of the higher-priority task to expedite resource release.

## Memory Management

### Timing of Memory Allocation in FreeRTOS
Memory allocation in FreeRTOS primarily occurs at the following times:

- Task creation: When creating a new task, FreeRTOS allocates the necessary memory for that task. This includes memory allocation for the Task Control Block (TCB) and the task stack.
- Creation of queues, semaphores, and mutexes: When creating queues, semaphores, or mutexes, FreeRTOS allocates the necessary memory to store management information for these objects.
- Dynamic memory allocation: If using FreeRTOS's dynamic memory management features, such as `pvPortMalloc()` and `vPortFree()`, memory can be dynamically allocated and released during runtime.
- Delayed allocation: For instance, in some cases, memory allocation may be delayed until it is indeed needed.
- Interrupt Service Routines (ISR): In some cases, ISRs might also need to allocate memory, for instance, when sending data items to queues or semaphores.
