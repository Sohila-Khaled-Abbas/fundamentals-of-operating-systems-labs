#!/usr/bin/env bash
# Lab 07: Shell / cURL Socket Testing Script
# Fundamentals of Operating Systems - Hussein Nasser

PORT=8080
HOST="127.0.0.1"

echo "=========================================================="
echo "  Lab 07: Socket & Web Server Test Automation (cURL/nc)   "
echo "  Target: http://${HOST}:${PORT}/                         "
echo "=========================================================="

echo -e "\n[1] Basic HTTP GET Request using curl:"
curl -i "http://${HOST}:${PORT}/" || echo "Server not reachable on port ${PORT}"

echo -e "\n[2] Checking Socket State & Queue with ss/netstat:"
if command -v ss &> /dev/null; then
    ss -lntp "sport = :${PORT}"
elif command -v netstat &> /dev/null; then
    netstat -an | grep "${PORT}"
fi

echo -e "\n[3] Testing Backlog Saturation (10 concurrent requests):"
for i in {1..10}; do
    curl -s "http://${HOST}:${PORT}/" > /dev/null &
done
wait
echo "Burst completed."
