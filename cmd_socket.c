/* *****************************************************************************
 * File:   cmd_socket.c
 * Author: Dimitar Lilov
 *
 * Created on 2022 06 18
 * 
 * Description: ...
 * 
 **************************************************************************** */

/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "cmd_socket.h"
#include "drv_socket.h"

#include <string.h>

#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"

#include "argtable3/argtable3.h"

#if CONFIG_DRV_CONSOLE_USE
#include "drv_console.h"
#endif

/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */
#define TAG "cmd_socket"

/* *****************************************************************************
 * Constants and Macros Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Enumeration Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Type Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Function-Like Macros
 **************************************************************************** */

/* *****************************************************************************
 * Variables Definitions
 **************************************************************************** */

static struct {
    struct arg_str *name;
    struct arg_str *command;
    struct arg_str *url;
    struct arg_str *ip_address;
    struct arg_end *end;
} socket_args;


/* *****************************************************************************
 * Prototype of functions definitions
 **************************************************************************** */

/* *****************************************************************************
 * Functions
 **************************************************************************** */
static int update_socket(int argc, char **argv)
{
    #if CONFIG_DRV_CONSOLE_USE
    #if CONFIG_DRV_CONSOLE_CUSTOM
    #if CONFIG_DRV_CONSOLE_CUSTOM_LOG_DISABLE_FIX
    drv_console_set_other_log_disabled();
    #endif
    #endif
    #endif

    ESP_LOGI(__func__, "argc=%d", argc);
    for (int i = 0; i < argc; i++)
    {
        ESP_LOGI(__func__, "argv[%d]=%s", i, argv[i]);
    }

    int nerrors = arg_parse(argc, argv, (void **)&socket_args);
    if (nerrors != ESP_OK)
    {
        arg_print_errors(stderr, socket_args.end, argv[0]);
        return ESP_FAIL;
    }

    const char* socket_name = NULL;
    socket_name = socket_args.name->sval[0];
    if (socket_args.name->count)
    {
        ESP_LOGD(TAG, "name[0]%s count:%d", socket_args.name->sval[0], socket_args.name->count);
    }
    const char* socket_command = NULL;
    socket_command = socket_args.command->sval[0];
    if (socket_args.command->count)
    {
        ESP_LOGD(TAG, "command[0]%s count:%d", socket_args.command->sval[0], socket_args.command->count);
    }
    const char* socket_url = NULL;
    socket_url = socket_args.url->sval[0];
    if (socket_args.url->count)
    {
        ESP_LOGD(TAG, "url[0]%s count:%d", socket_args.url->sval[0], socket_args.url->count);
    }
    const char* socket_ip_address = NULL;
    if (socket_args.ip_address->count)
    socket_ip_address = socket_args.ip_address->sval[0];
    {
        ESP_LOGD(TAG, "ip_address[0]%s count:%d", socket_args.ip_address->sval[0], socket_args.ip_address->count);
    }
    if (strcmp(socket_command,"list") == 0)
    {
        drv_socket_list();
    }    
    else
    if (strlen(socket_name) > 0)
    {
        int index = drv_socket_get_position(socket_name);
        if ( index >= 0 )
        {
            ESP_LOGW(TAG, "Found Socket[%d] Name %s", index, socket_name);
        }
        else
        {
            ESP_LOGE(TAG, "Error Socket %s not found", socket_name);
        }
        drv_socket_t* pSocket = drv_socket_get_handle(socket_name);
        if (pSocket != NULL)
        {
            if (socket_args.url->count)
            {
                drv_socket_url_set(pSocket, socket_url);
            }
            if (socket_args.ip_address->count)
            {
                drv_socket_ip_address_set(pSocket, socket_ip_address);
            }

            if (strcmp(socket_command,"reset") == 0)
            {
                drv_socket_disconnect(pSocket);
            }
            else
            if (strcmp(socket_command,"stop") == 0)
            {
                drv_socket_stop(pSocket);
            }
            else
            if (strcmp(socket_command,"start") == 0)
            {
                drv_socket_start(pSocket);
            }
        }
    }
    return 0;
}

static void register_socket(void)
{
    socket_args.ip_address = arg_strn("a", "ip", "<ip address>", 0, 1, "Command can be : socket -n socket_name -a 192.168.0.5");
    socket_args.url = arg_strn("u", "url", "<URL>", 0, 1, "Command can be : socket -n socket_name -u url_name");
    socket_args.name = arg_strn("n", "name", "<name>", 0, 1, "Command can be : socket [-n socket_name]");
    socket_args.command = arg_strn(NULL, NULL, "<command>", 0, 1, "Command can be : socket {reset|start|stop|list}");
    socket_args.end = arg_end(5);

    const esp_console_cmd_t cmd_socket = {
        .command = "socket",
        .help = "Socket Settings Manage Request",
        .hint = NULL,
        .func = &update_socket,
        .argtable = &socket_args,
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd_socket));
}


void cmd_socket_register(void)
{
    register_socket();
}