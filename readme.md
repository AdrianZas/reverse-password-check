# Obfuscated Password Verification in C

This project demonstrates a simple yet intentionally obfuscated password verification routine written in C. It focuses on **reverse engineering resistance**, **binary hardening**, and clean **low-level programming techniques**.

## Purpose

The goal of this project is to simulate how sensitive information (like passwords or license keys) can be protected from static analysis tools such as `strings`, `objdump`, or basic binary scanning.

Although not cryptographically secure, it showcases how **basic obfuscation and runtime generation** can improve binary opacity and serve as a starting point for understanding software protection techniques.

## Key Learning Objectives

- Implement password logic without exposing plaintext in the binary
- Generate secret values entirely at runtime
- Understand XOR-based obfuscation and its limitations
- Explore how attackers might reverse such binaries using `gdb`, `objdump`, etc.
- Practice low-level C with a focus on security awareness

## Technical Details

- No plaintext passwords exist in the binary (verified via `strings`)
- The target password is dynamically generated using math-based character selection
- XOR is used as a lightweight obfuscation mechanism (`^ 0x13`)
- No null-terminated string literals for secrets

## Example Usage

```bash
$ make
$ ./secret
Enter password: <your input>
Access granted
```

## Possible Improvements

- Add anti-debugging detection using ptrace()
- Strip symbols and section names (strip)
- Introduce fake backdoors to mislead static analysis
- Use inline assembly for advanced obfuscation
- Replace strcmp() with manual byte comparison to avoid null-terminated strings

