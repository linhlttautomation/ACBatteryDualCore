# Code Dual Core 2 CPU 2 CLA cho FLC và TPC của hệ thống AC Battery ⚡️

Dự án này cung cấp mã nguồn cho hệ thống **AC Battery** với kiến trúc **Dual Core** và **2 CLA** nhằm điều khiển các bộ biến đổi Four-leg Converter (FLC) và Three-port Converter (TPC). Hệ thống tận dụng cấu trúc đa nhân và các bộ tăng tốc CLA để tối ưu hóa hiệu suất, giảm độ trễ và tăng độ chính xác.

---

## 🛠️ Các Thành Viên Tham Gia

- **Luu Linh** - Phụ trách FLC
- **Tung Bui** - Phụ trách TPC
---

## 🚀 Tính Năng Nổi Bật

- **Dual Core CPU**: Sử dụng hai bộ xử lý giúp thực thi các nhiệm vụ song song.
- **2 CLA (Control Law Accelerators)**: Tối ưu cho xử lý thời gian thực, giúp cải thiện hiệu quả và giảm tải cho CPU chính.

---

## 📂 Cấu Trúc Dự Án

Dự án được phân chia thành các thư mục theo kiến trúc CPU1, CPU2 và các CLA tương ứng để tối ưu hóa khả năng điều khiển:

```plaintext
├── src/                         # Thư mục chứa mã nguồn chính
│   ├── cpu1/                    # Mã nguồn cho CPU1
│   ├── cpu1_cla1/               # Mã nguồn cho CLA1 thuộc CPU1
│   ├── cpu2/                    # Mã nguồn cho CPU2
│   └── cpu2_cla1/               # Mã nguồn cho CLA1 thuộc CPU2
│
├── docs/                        # Tài liệu dự án
├── examples/                    # Ví dụ và demo
└── README.md                    # Tài liệu chính của dự án
