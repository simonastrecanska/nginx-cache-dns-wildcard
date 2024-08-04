FROM debian:bullseye

# Install necessary packages for building Nginx
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    libpcre3 \
    libpcre3-dev \
    zlib1g \
    zlib1g-dev \
    libssl-dev \
    curl

# Copy the Nginx source code and custom module into the container
COPY nginx-1.24.0 /usr/local/src/nginx-1.24.0
COPY ngx_http_cache_key_module /usr/local/src/ngx_http_cache_key_module

# Set the working directory
WORKDIR /usr/local/src/nginx-1.24.0

# Configure, build, and install Nginx with the custom module
RUN ./configure --add-module=/usr/local/src/ngx_http_cache_key_module && \
    make && \
    make install

# Copy custom Nginx configuration
COPY conf/nginx.conf /usr/local/nginx/conf/nginx.conf

EXPOSE 80

CMD ["/usr/local/nginx/sbin/nginx", "-g", "daemon off;"]
