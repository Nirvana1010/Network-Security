# Network Security

Network Security course projects.

**Language:** C++

**Platform:** Visual Studio 2010

## Overview

### Project 1 - TCP communication based on DES encryption

1. Implement reliable, full duplex TCP communication based on socket programming.
2. Implement DES strings encryption and decryption operations.
3. Implement a TCP chat program with DES encryption by combining 1 and 2; the sender encrypts and the receiver decrypts with a pre-agreed key to ensure the confidentiality of the message during the network transmission.

**Demos:**

![](/Project1/result.png) 

See codes for *[project 1](/Project1)*. 

### Project 2 - TCP communication based on RSA encryption

1. Implement automatic key distribution encryption chat program based on RSA. 
2. The application maintains all the operations in Project 1 - "TCP communication based on DES encryption", and extends it to achieve automatic key generation and secret sharing based on RSA algorithm. 
3. Full duplex communication is required and the encryption process is completely transparent to the user.

**Demos:**

![](/Project2/result.png) 

See codes for *[project 2](/Project2)*. 

### Project 3 - MD5 message-digest algorithm

Write an application that correctly implements the MD5 algorithm. Requirements:
1. generate MD5 message-digest for a file of arbitrary length and size. 
2. verify the integrity of the file based on MD5 message-digest algorithm. 
3. Verify the integrity of the file in two ways.
    -  Manually input the MD5 digest of the file, calculate the MD5 digest of the file under test, and compare the two; if they are the same, the file is considered complete; otherwise, the file is corrupted.
    - Generate the MD5 digest of the file under test using Windows system tools, calculate the MD5 digest of the file under test, and compare the two to obtain the result.