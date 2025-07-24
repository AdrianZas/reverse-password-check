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

# Reverse Engineering the Obfuscated Password

This section explains how I reverse-engineered a binary that performs password verification using XOR obfuscation. The goal was to extract the correct password **without having access to the source code**.

## Used Code:

<pre><code>```c #include &lt;stdio.h&gt; #include &lt;string.h&gt; void encrypt_password(char *out) { char get_char(int index) { if (index < 26) return 'A' + index; else if (index < 52) return 'a' + (index - 26); else return '1' + (index - 52); } for (int i = 0; i < 5; i++) { out[i] = get_char((i * 7 + 3) % 62) ^ 0x13; } out[5] = '\0'; } int main() { char input[20]; char pwd[20]; printf("Enter password: "); scanf("%s", input); for(int i = 0; i < strlen(input); i++) { input[i] ^= 0x13; } encrypt_password(pwd); if (strcmp(input, pwd) == 0) { printf("Access granted\n"); } else { printf("Access denied\n"); } } ``` </code></pre>

## Objective

The binary verifies a password without storing it in plaintext. Instead, it uses a runtime-generated secret combined with XOR encoding.

I wanted to:

- Analyze the binary using only **GDB and objdump**
- Understand how the password is generated
- Reconstruct the password from memory
- Explain each step with screenshots

---

##  Tools Used

- `gdb` (GNU Debugger)
- `objdump`
- `strings` (for initial verification)
- `gcc -g` to keep debug symbols during compilation
- Manual memory inspection using `x/s` and `x/6bx`
- Hex-to-ASCII reasoning

---


## Setup

Compile the binary **with debug info** to preserve symbol names:

```bash
gcc -g -o secret_debug secret.c
gdb ./secret_debug
```

### Step by Step RE Process
1. Break at main()
```bash
break main
run
```
Once paused i used list to inspect the code and step through the logic:

```bash
list
next
```
2. Step into the password generator

```bash 
break encrypt_password generator
continue
```
The function generates the password byte by byte using XOR logic.

3. Read password from memory
Once inside `encrypt_password` used:
```bash
x/6bx out

Output: 0x7ffffffff250: 0x57    0x58    0x41    0x4a    0x75    0x55
```

This gives us the XOR-encoded bytes of the password.

To decode them, we apply `^ 0x13` (XOR with hex 13):

| Byte | XOR 0x13 | ASCII |
|------|----------|--------|
| 0x57 | 0x44     | D      |
| 0x58 | 0x4B     | K      |
| 0x41 | 0x52     | R      |
| 0x4A | 0x59     | Y      |
| 0x75 | 0x66     | f      |
| 0x55 | 0x46     | F      |

Note that the last byte needs to be ignored because `\0` is the null terminator in C

**Final password: `DKRYf`**


## Key Takeaways

- XOR obfuscation is not secure, but can hide values from static tools like strings.
- Debugging with symbols (gcc -g) enables step-by-step RE.
- Memory inspection (x/6bx) is critical for recovering runtime values.
- You must skip the null terminator when decoding passwords in C binaries.

## Next Step

- Try stripping the binary (strip secret_debug) and repeat the process without symbols.1~Try stripping the binary (strip secret_debug) and repeat the process without symbols.

