#ifndef _XMLCONFIG_DEF_H_
#define _XMLCONFIG_DEF_H_


#define PROC_CONFIG_XML_ROOT "ProcessConfigurationStore"

#define PROC_CONFIG_XML_NODE_GROUPS "Groups"
#define PROC_CONFIG_XML_NODE_GROUP "Group"
#define PROC_CONFIG_XML_NODE_PROCESS "Process"
#define PROC_CONFIG_XML_NODE_SHM "Shm"
#define PROC_CONFIG_XML_NODE_FLAGS "Flags"
#define PROC_CONFIG_XML_NODE_FLAG "Flag"
#define PROC_CONFIG_XML_NODE_VARIABLES "Variables"
#define PROC_CONFIG_XML_NODE_ARGS "Args"
#define PROC_CONFIG_XML_NODE_ARG "Arg"
#define PROC_CONFIG_XML_NODE_ENVS "Envs"
#define PROC_CONFIG_XML_NODE_ENV "Env"

#define PROC_CONFIG_XML_NODE_DEPENDENCY "Dependency"
#define PROC_CONFIG_XML_NODE_PD_PR "Process"
#define PROC_CONFIG_XML_NODE_PD_SM "Shm"


#define PROC_CONFIG_XML_ATTR_CONFIG_VERSION "configVersion"
#define PROC_CONFIG_XML_ATTR_ID "id"
#define PROC_CONFIG_XML_ATTR_DESCRIPTION "description"
#define PROC_CONFIG_XML_ATTR_NAME "name"
#define PROC_CONFIG_XML_ATTR_DESC "description"
#define PROC_CONFIG_XML_ATTR_VALUE "value"
#define PROC_CONFIG_XML_ATTR_IGNORE "ignore"
#define PROC_CONFIG_XML_ATTR_ENABLE "enable"
#define PROC_CONFIG_XML_ATTR_PATH "path"
#define PROC_CONFIG_XML_ATTR_DELAY "delay"
#define PROC_CONFIG_XML_ATTR_MAX_INSTANCE "maxInstance"

#define PROC_CONFIG_XML_ATTR_SHM_PATH "shmPath"
#define PROC_CONFIG_XML_ATTR_SHM_ID "shmId"

#define PROC_CONFIG_XML_FLAG_IS_SYSTEM_COMMAND "issystemcommand"
#define PROC_CONFIG_XML_FLAG_IS_KILL_MULTIPLE "iskillmultiple"
#define PROC_CONFIG_XML_FLAG_IS_RESTART_ALL "isrestartall"
#define PROC_CONFIG_XML_FLAG_IS_MONITOR "ismonitor"
#define PROC_CONFIG_XML_FLAG_IS_SHARED_PROCESS "issharedprocess"

//Server configuration def

#define SERVER_SETTING_CONFIG_XML_ROOT "ServerConfiguration"

#define SERVER_SETTING_CONFIG_ATTR_LOG "log"
#define SERVER_SETTING_CONFIG_ATTR_LOG_FILE "logFile"
#define SERVER_SETTING_CONFIG_ATTR_PORT "port"
#define SERVER_SETTING_CONFIG_ATTR_DEFAULT_POLL_INTERVAL "pollInterval"

//Client configuration def
#define CLIENT_SETTING_CONFIG_XML_ROOT "ClientConfiguration"

#define CLIENT_SETTING_CONFIG_ATTR_PORT "port"
#define CLIENT_SETTING_CONFIG_ATTR_LOG "log"
#define CLIENT_SETTING_CONFIG_ATTR_IP "serverIp"

#endif