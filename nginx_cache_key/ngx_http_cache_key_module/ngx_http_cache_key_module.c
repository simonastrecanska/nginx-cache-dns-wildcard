#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// Function prototypes
static ngx_int_t ngx_http_cache_key_header_filter(ngx_http_request_t *r);
static ngx_http_output_header_filter_pt ngx_http_next_header_filter;
static ngx_int_t ngx_http_cache_key_filter_init(ngx_conf_t *cf);

// Module context
static ngx_http_module_t ngx_http_cache_key_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_cache_key_filter_init,        /* postconfiguration */
    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */
    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */
    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};

// Module directives
static ngx_command_t ngx_http_cache_key_commands[] = {
    ngx_null_command
};

// Module definition
ngx_module_t ngx_http_cache_key_module = {
    NGX_MODULE_V1,
    &ngx_http_cache_key_module_ctx,        /* module context */
    ngx_http_cache_key_commands,           /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

// Header filter function to add X-Cache-Key header
static ngx_int_t
ngx_http_cache_key_header_filter(ngx_http_request_t *r)
{
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Entering ngx_http_cache_key_header_filter");

    ngx_table_elt_t  *h;
    ngx_str_t         cache_key;

    // Calculate cache key
    ngx_str_t scheme = ngx_string("http");
#if (NGX_HTTP_SSL)
    if (r->connection->ssl) {
        scheme = ngx_string("https");
    }
#endif
    ngx_str_t host = r->headers_in.host->value;
    ngx_str_t uri = r->uri;
    ngx_str_t args = r->args;

    // Determine if we need to include the port
    ngx_uint_t port = ngx_inet_get_port(r->connection->sockaddr);
    ngx_str_t port_str = ngx_null_string;
    if (port != 80 && port != 443) {
        port_str.data = ngx_pnalloc(r->pool, NGX_INT_T_LEN);
        if (port_str.data == NULL) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate memory for port string");
            return NGX_ERROR;
        }
        port_str.len = ngx_sprintf(port_str.data, ":%ui", port) - port_str.data;
    }

    // Log the individual components
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Scheme: %V", &scheme);
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Host: %V", &host);
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Port: %V", &port_str);
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "URI: %V", &uri);
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Args: %V", &args);

    // Create cache key
    size_t len = scheme.len + sizeof("://") - 1 + host.len + port_str.len + uri.len + (args.len ? args.len + 1 : 0);
    cache_key.data = ngx_pnalloc(r->pool, len);
    if (cache_key.data == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate memory for cache key");
        return NGX_ERROR;
    }
    if (args.len) {
        cache_key.len = ngx_sprintf(cache_key.data, "%V://%V%V%V?%V", &scheme, &host, &port_str, &uri, &args) - cache_key.data;
    } else {
        cache_key.len = ngx_sprintf(cache_key.data, "%V://%V%V%V", &scheme, &host, &port_str, &uri) - cache_key.data;
    }

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Calculated cache key: %V", &cache_key);

    // Add X-Cache-Key header
    h = ngx_list_push(&r->headers_out.headers);
    if (h == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to add X-Cache-Key header");
        return NGX_ERROR;
    }

    h->hash = 1;
    ngx_str_set(&h->key, "X-Cache-Key");
    h->value.len = cache_key.len;
    h->value.data = cache_key.data;

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Added X-Cache-Key header: %V", &cache_key);

    return ngx_http_next_header_filter(r);
}

// Initialization function for the header filter
static ngx_int_t
ngx_http_cache_key_filter_init(ngx_conf_t *cf)
{
    ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0, "Initializing ngx_http_cache_key_filter_init");

    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_cache_key_header_filter;

    ngx_log_error(NGX_LOG_ERR, ngx_cycle->log, 0, "ngx_http_cache_key_header_filter added to the filter chain");

    return NGX_OK;
}
