\# High Performance Server



\## 📌 Project Overview

This is a \*\*C++ High Performance Server\*\* project built to handle multiple clients simultaneously using \*\*multi-threading\*\* and \*\*socket programming\*\*.  



It is designed to demonstrate:

\- Networking fundamentals (TCP/IP Sockets)

\- Multi-threading in C++

\- Server-Client architecture

\- Error handling and client disconnects



---



\## ⚙️ Tech Stack

\- C++

\- Socket Programming (Winsock2)

\- Multi-threading (std::thread)



---



\## 🚀 Features

\- Handles multiple clients at the same time  

\- Each client handled by a separate thread  

\- Proper error handling and client disconnect handling  

\- Clients can send messages and receive responses  



---



\## ▶️ How to Run

1\. Compile the server:

&nbsp;  ```bash

&nbsp;  g++ server.cpp -o server.exe -lws2\_32



