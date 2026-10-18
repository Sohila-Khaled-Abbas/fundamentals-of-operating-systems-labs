#!/usr/bin/env python3
"""
Lab 07: Socket & Web Server Test Clients
Fundamentals of Operating Systems - Hussein Nasser

Simulates various client network conditions:
1. Normal HTTP client
2. Slow connection / Slowloris (sends headers byte-by-byte with delays)
3. Slow receiver / Zero TCP Window simulation (small receive buffer)
4. Concurrent connection burst (testing OS accept queue backlog)
"""

import sys
import socket
import time
import threading

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 8080

def test_normal_client():
    print("\n--- [1] Testing Normal Client ---")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((SERVER_HOST, SERVER_PORT))
        request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"
        s.sendall(request.encode())
        response = s.recv(4096)
        print("Response received from server:\n" + response.decode(errors='replace'))
    finally:
        s.close()

def test_slow_request():
    print("\n--- [2] Testing Slow Client (Byte-by-Byte Request) ---")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((SERVER_HOST, SERVER_PORT))
        request = "GET / HTTP/1.1\r\nHost: localhost\r\nX-Custom: slow\r\n\r\n"
        print("Sending request slowly (1 char per 0.5s)...")
        for char in request:
            s.send(char.encode())
            time.sleep(0.1)
        response = s.recv(4096)
        print("Response received:\n" + response.decode(errors='replace'))
    finally:
        s.close()

def test_backlog_burst(num_conns=15):
    print(f"\n--- [3] Testing Accept Queue Overflow (Burst of {num_conns} connections) ---")
    sockets = []
    for i in range(num_conns):
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(2.0)
            s.connect((SERVER_HOST, SERVER_PORT))
            sockets.append(s)
            print(f"  Connected client #{i+1}")
        except Exception as e:
            print(f"  Client #{i+1} failed to connect: {e}")
    
    print(f"Established {len(sockets)} open connections in OS accept queue/ESTABLISHED state.")
    time.sleep(2)
    for s in sockets:
        s.close()
    print("All burst connections closed.")

def test_zero_window():
    print("\n--- [4] Testing Zero TCP Window Simulation ---")
    print("Configuring tiny socket receive buffer (SO_RCVBUF)...")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        # Set OS receive buffer to minimum size to trigger TCP zero-window faster
        s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024)
        s.connect((SERVER_HOST, SERVER_PORT))
        s.sendall(b"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")
        print("Connected. Sleeping without reading from socket to let OS receive buffer fill up...")
        time.sleep(3)
        # Read available bytes
        chunk = s.recv(256)
        print(f"Read small chunk: {chunk[:30]}...")
    finally:
        s.close()

if __name__ == "__main__":
    print("==================================================")
    print("  Lab 07: OS Socket & Networking Test Suite")
    print(f"  Target: {SERVER_HOST}:{SERVER_PORT}")
    print("==================================================")
    print("Select test mode:")
    print("1. Normal Request")
    print("2. Slow Client / Slowloris simulation")
    print("3. Accept Queue / Backlog Burst test")
    print("4. Zero TCP Window test")
    print("5. Run all tests sequentially")
    
    choice = sys.argv[1] if len(sys.argv) > 1 else "1"
    
    try:
        if choice == "1":
            test_normal_client()
        elif choice == "2":
            test_slow_request()
        elif choice == "3":
            test_backlog_burst()
        elif choice == "4":
            test_zero_window()
        elif choice == "5":
            test_normal_client()
            time.sleep(1)
            test_slow_request()
            time.sleep(1)
            test_backlog_burst()
            time.sleep(1)
            test_zero_window()
        else:
            print(f"Unknown test mode: {choice}")
    except ConnectionRefusedError:
        print(f"\n[ERROR] Connection refused! Is the server running on port {SERVER_PORT}?")
        print("Start one of the web servers first (e.g., ./cwebserver)")
