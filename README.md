## Project Title: Word Separation Program

### Introduction
Process management is a fundamental aspect of any operating system. Understanding how processes are created and the different mechanisms through which they can communicate is crucial. This project explores the basic tools for interprocess communication in LINUX/UNIX systems, utilizing them to synchronize multiple processes working on word separation tasks.

### Objective
The main objective of this project is to demonstrate the mechanisms provided by UNIX systems for process creation, management, and interprocess communication.

### Problem to Solve
In this project, we will work with a set of words from the Royal Spanish Academy (RAE). The goal is to create multiple synchronized processes that separate words starting with different letters of the Spanish alphabet. You will be provided with a file containing all the RAE words categorized by their starting letter.

The implemented C program (written in Spanish) creates a process for each letter of the alphabet. Each process is responsible for gathering words corresponding to a specific letter and creating a file named after that letter.

### Main Process Tasks:
- Create child processes. Once all processes are created, send a communication using pipes and signals to indicate that they can start their tasks.
- Wait for each child process to finish. Once all processes have finished, display the names of the files generated by the child processes.

### Child Process Tasks:
- Upon creation, configure process-specific data in shared memory to facilitate interaction with the main process and other child processes, if necessary. The child processes should not start generating their file until they receive the corresponding communication from the main process.
- Upon receiving the notification, generate the file corresponding to the assigned letter.
- Before terminating, send a message to all active processes at that moment. This communication will be done using pipes and signals. The process will send its process identifier through the pipe and use the signal to ensure immediate attention from the receiving process. Once the message is sent, the process will terminate successfully.
- Upon receiving the notification, each child process will display the following message: "I am process XX, and child process YY has finished."

### Conclusion
This project provides an opportunity to explore the foundational concepts of process management and interprocess communication in UNIX systems. By implementing the word separation program (written in Spanish), you will gain hands-on experience with creating processes, managing their execution, and establishing communication channels between them.

Note: Please ensure that the necessary file containing the RAE words is provided before running the program.

Feel free to modify and enhance this project according to your needs and creativity. Happy coding!
