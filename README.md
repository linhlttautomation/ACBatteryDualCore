# Dual Core 2 CPU 2 CLA Code for FLC and TPC in AC Battery Systems ⚡️

This repository provides the source code for the **AC Battery** system, designed with a **Dual Core** and **2 CLA** architecture. It controls Four-leg Converters (FLC) and Three-port Converters (TPC), leveraging multi-core technology and Control Law Accelerators (CLA) to enhance performance, reduce latency, and improve accuracy.

---

## 🛠️ Contributors

- **Luu Linh** - Responsible for FLC
- **Tung Bui** - Responsible for TPC

---

## 🚀 Key Features

- **Dual Core CPU**: Employs two processors to execute tasks concurrently for parallel processing.
- **2 CLA (Control Law Accelerators)**: Optimized for real-time computation, improving efficiency and offloading processing tasks from the main CPU.

---

## 📂 AC Battery 1kW Board
![AC Battery 1kW Board](https://github.com/linhlttautomation/ACBatteryDualCore/blob/master/AC%20Battery%201kW%20Board.png)
![AC Battery 1kW Board](https://github.com/linhlttautomation/ACBatteryDualCore/blob/master/AC%20Battery%201kW%20Board%20Side.png)
## 📂 PINMAP For Dual-Core Project
![PINMAP For Dual-Core Project](https://github.com/linhlttautomation/ACBatteryDualCore/blob/master/PINMAP%20For%20Dual-Core%20Project.png)


---

## 📂 Project Structure

The project is organized into directories corresponding to CPU1, CPU2, and their associated CLAs to maximize modularity and control capabilities:

```plaintext
├── src/                         # Main source code directory
│   ├── cpu1/                    # Source code for CPU1
│   ├── cpu1_cla1/               # Source code for CLA1 of CPU1
│   ├── cpu2/                    # Source code for CPU2
│   └── cpu2_cla1/               # Source code for CLA1 of CPU2
│
├── docs/                        # Project documentation
├── examples/                    # Examples and demonstrations
└── README.md                    # Main project documentation
