This project demonstrates how to build and integrate a custom Nginx module that calculates and adds a `X-Cache-Key` header to HTTP responses. The custom module is built and tested using Docker.

## Prerequisites

- Docker
- Curl

## Building and Running

1. Clone the repository if you haven't already:
```sh
git clone https://github.com/simonastrecanska/nginx-cache-dns-wildcard.git
```

2. **Navigate to the `examples` directory**:
    ```sh
    cd nginx-cache-dns-wildcard
    cd nginx_cache_key/examples
    ```

2. **Run the example request script**:
    ```sh
    ./example_request.sh
    ```

This script will:

1. Build the Docker image.
2. Run the Docker container and expose it on port 8080.
3. Send several requests to the server and print the response headers, including checks for the `X-Cache-Key` header:
    - Root endpoint.
    - Root endpoint with a query string.
    - `/anything` endpoint.
    - `/anything` endpoint with a query string.
    - `/headers` endpoint.
    - `/status/200` endpoint.
4. Check and print the Docker logs.
5. Clean up the Docker container.
6. Stop and remove any existing container named `mynginx2`.
7. Run a new container instance on port 8081.
8. Repeat the tests on port 8081.
9. Check and print the Docker logs for the second container.
10. Clean up the second Docker container.

## Example Output

You should see HTTP response headers including the `X-Cache-Key` header with the expected values.

## Files

-`Dockerfile` : builds Nginx with the custom module and sets up the environment.

-`conf/nginx.conf` : the Nginx configuration file, which includes settings for proxy caching and adding the `X-Cache-Key` header.

-`ngx_http_cache_key_module/config` : configuration file for the custom module to be recognized by the Nginx build system.

-`ngx_http_cache_key_module/ngx_http_cache_key_module.c` : source code of the custom Nginx module that calculates and adds the `X-Cache-Key` header.

-`example/example_request.sh` : a script to build the Docker image, run the Nginx container, and make various HTTP requests to test the module.

#### Key Functions:
- `ngx_http_cache_key_header_filter`: Calculates the cache key and adds it to the response headers.
- `ngx_http_cache_key_filter_init`: Initializes the header filter.

