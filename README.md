# Ventura Bank Management System (Refactored)

This is a refactored, safer version of the Ventura Bank Management System.
Changes made:
- Modularized C++ source into `src/` (main, auth, bank, util).
- Replaced plaintext password storage with **salted SHA-256** hashing (requires OpenSSL).
- Added atomic file writes, basic input validation, and removed `system()` calls.
- Added `.gitignore`, `LICENSE` (MIT), and a `Makefile`.
- Removed binary `main.exe` from the repo.

## Requirements
- g++ (with C++11 support)
- OpenSSL development libraries (for password hashing): install `libssl-dev` (Debian/Ubuntu) or equivalent.

## Build
From the project root:
```bash
make
```

This builds the `ventura_bank` binary in `build/`.

If OpenSSL isn't available, you can still compile by removing `-lcrypto` from the linker flags in the Makefile,
but password hashing will fail to compile — it's strongly recommended to install OpenSSL.

## Run
```bash
./build/ventura_bank
```

## Data files format
- `data/customer_credentials.txt` — stores credentials one per line: `username:salt:hex_hash`
- `data/customer_accounts.txt` — CSV: `account_number,username,balance,currency`

## Security notes
- Passwords are never stored in plaintext. Use a strong password.
- For production use, migrate storage from text files to an encrypted database (SQLite with file encryption or an external DB).
- This project is educational and not intended for real banking use.

## What I changed
See source files in `src/`. The `auth` module handles registration/login and uses OpenSSL EVP SHA-256 for hashing.
Atomic writes are used when updating data files.

