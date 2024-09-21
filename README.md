# Project Name: Advanced Robot Programming for Server Connection
# Project Description:
The Advanced Robot Programming for Server Connection project focuses on developing a robust multi-process system that enables communication between two computers within a local area network (LAN). This system facilitates the exchange of floating-point data tokens, simulating real-time interactions akin to robotic communication.

# Technical Implementation:
Architecture Overview:

The project is structured around a client-server model where each computer runs multiple processes (P, G, S, L) for token generation, logging, and signal processing.
Each process communicates via sockets, allowing for flexible and dynamic data exchange.
Key Components:

Process P:
Receives incoming tokens and performs computations based on the formula:
new token
=
received token
+
𝐷
𝑇
×
(
1
−
(
received token
)
2
2
)
×
2
𝜋
×
𝑅
𝐹
new token=received token+DT×(1− 
2
(received token) 
2
 
​
 )×2π×RF
This results in a sine wave pattern of tokens over time.
Process G:
Dispatches tokens to Process P and listens for incoming data from connected clients.
Process L:
Logs all data and events, ensuring that a record of token exchanges and signals is maintained.
Process S:
Listens for console commands via POSIX signals to start, stop, and manage the operations of other processes.
System Configuration:

Each machine utilizes a configuration file containing crucial parameters such as IP addresses, reference frequencies, and other settings required to initiate the processes.
The configuration file is user-editable before running the system, enhancing adaptability.
Implementation Steps:

# Setup:

Install necessary packages on Ubuntu 16.04 LTS or later.

Create the configuration file to define network parameters and token generation settings.

# Process Development:
Implement each process in C, ensuring proper use of POSIX system calls (fork(), exec(), pipe(), select(), etc.).
Code each process to handle data appropriately, ensuring they respond to signals and communicate effectively with one another.
Testing:
Conduct initial tests on a single machine to verify inter-process communication before deploying the system across multiple machines in the network.
Use logging to troubleshoot and ensure that the system operates as intended.
Final Integration:
After testing individual components, deploy the full system across multiple computers, establishing a connection via sockets.
Validate that the token exchange functions correctly across the network, simulating a robotic communication scenario.
Performance Monitoring:

Utilize log files to monitor the behavior of the system during operation, analyzing the timestamped data to ensure accurate token exchanges and processing.

## Technical Implementation

### Key Components
- **Process P**: Receives tokens, computes new tokens using a defined formula, and sends them back.
- **Process G**: Dispatches tokens to Process P and manages incoming data.
- **Process L**: Logs all events and token exchanges.
- **Process S**: Handles console commands for process management.

### System Configuration
Each machine utilizes a configuration file that includes:
- IP addresses
- Reference frequencies
- Other necessary parameters

## Setup Instructions

### Prerequisites
- Ubuntu 16.04 LTS or later
- GCC compiler

### Installation Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/SaiVinay023/Advanced-Robot-Programing.git
   cd Advanced-Robot-Programing

   Edit the Configuration File Open configurationfile.txt and set the necessary parameters:

   IP_ADDRESS=your_machine_ip
ADJACENT_IP=neighbor_machine_ip
REFERENCE_FREQUENCY=desired_frequency

Run the Processes Open multiple terminal windows to run each process:

./process_g &
./process_p &
./process_s &
./process_l &

Interact with the System Use console signals to control the processes:

Start: Send signal to start token generation.
Stop: Send signal to stop operations.
Dump Log: Outputs the contents of the log file.
Testing
After running the processes, you can monitor the log file (logfile.txt) to ensure correct token exchanges and system behavior.

Conclusion
This project demonstrates the capabilities of multi-process communication in a networked environment, simulating advanced robotic interactions. For any questions or contributions, feel free to open an issue or submit a pull request.

# Project Outcomes:
This project provides a comprehensive understanding of multi-process architecture, networking protocols, and the practical implementation of real-time data exchange systems. It equips students with the skills necessary to design and deploy complex systems in collaborative environments, simulating advanced robotic communications.
