🛒 Supermarket Management System (C++)

A console-based Supermarket Management System built in C++ that allows administrators to manage products and users, while customers (buyers) can browse items and generate purchase receipts.

⸻

📌 Features

👨‍💼 Administrator
	•	Add new products
	•	Modify existing products
	•	Delete products
	•	Manage user accounts (username & password)

🛍️ Buyer
	•	View available products
	•	Purchase products
	•	Generate detailed receipt with discounts

🔐 Authentication
	•	Secure login system using hashed passwords
	•	User credentials stored in users.txt

🔊 Sound Effects
	•	Feedback sounds for actions (success, error, cash) using .wav files


📂 Project Structure
project/
│
├── main.cpp            # Main source code
├── database.txt        # Product database
├── users.txt           # User credentials (hashed)
├── *.wav               # Sound effects (error, accept, cash)
├── README.md           # Project documentation

⚙️ Requirements
	•	C++ Compiler (GCC / MinGW / Clang / MSVC)
	•	OS:
	•	Windows (uses PlaySound)
	•	macOS (uses afplay)
	•	Linux (requires modification for sound playback)

▶️ How to Compile & Run

🔹 On Windows
g++ main.cpp -o app -lwinmm
app.exe

🔹 On macOS / Linux
g++ main.cpp -o app
./app

🔑 Default Login
Username: admin
Password: admin

📊 Data Storage

	•	database.txt → stores product details:
    product_code  name  price  discount

	•	users.txt → stores:
    username  hashed_password