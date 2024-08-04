#!/bin/bash

# Navigate to the project root directory
cd "$(dirname "$0")/.."

echo "Building Docker image..."
docker build -t mynginx .

# Ensure the container is running
echo "Running Docker container..."
docker run --name mynginx -p 8080:80 -d mynginx

sleep 2

# Send a request to the root endpoint
echo "Requesting root endpoint..."
curl -I http://localhost:8080

# Send a request with a query string
echo "Requesting with query string..."
curl -I "http://localhost:8080/?foo=bar"

# Send a request to a different URI
echo "Requesting /anything endpoint..."
curl -I http://localhost:8080/anything

# Send a request to a different URI with a query string
echo "Requesting /anything endpoint with query string..."
curl -I "http://localhost:8080/anything?foo=bar&baz=qux"

# Send a request to the /headers endpoint
echo "Requesting /headers endpoint..."
curl -I http://localhost:8080/headers

# Send a request to the /status/200 endpoint
echo "Requesting /status/200 endpoint..."
curl -I http://localhost:8080/status/200

# Cleanup: stop and remove the container
echo "Cleaning up..."
docker stop mynginx
docker rm mynginx

# Test with a different port
echo "Testing with a different port..."

docker run --name mynginx2 -p 8081:80 -d mynginx
sleep 2

echo "Requesting root endpoint on port 8081..."
curl -I http://localhost:8081

echo "Requesting /anything endpoint on port 8081..."
curl -I http://localhost:8081/anything

# Cleanup: stop and remove the container
echo "Cleaning up..."
docker stop mynginx2
docker rm mynginx2

