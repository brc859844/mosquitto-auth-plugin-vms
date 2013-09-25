$ set verify
$
$ cc/nolist/names=(as_is,shortened)/include=([-.mosquitto-113.src],[-.mosquitto-113.lib]) mosquitto_auth_plugin_vms.c
$ link/share=mosquitto_auth_plugin_vms.exe mosquitto_auth_plugin_vms.obj,sys$input/opt
!
CASE_SENSITIVE=YES
SYMBOL_VECTOR=( -
	mosquitto_auth_plugin_version   = PROCEDURE, -
	mosquitto_auth_plugin_init      = PROCEDURE, -
	mosquitto_auth_plugin_cleanup   = PROCEDURE, -
	mosquitto_auth_security_init    = PROCEDURE, -
	mosquitto_auth_security_cleanup = PROCEDURE, -
	mosquitto_auth_acl_check        = PROCEDURE, -
	mosquitto_auth_unpwd_check      = PROCEDURE, -
	mosquitto_auth_psk_key_get      = PROCEDURE  -
)
!
$ purge/log
$ exit

