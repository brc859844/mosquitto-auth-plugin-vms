mosquitto-auth-plugin-vms
=========================

#Overview
A simple plugin for the Mosquitto MQTT broker (http://mosquitto.org) that
provides the ability for the broker to authenticate users via standard OpenVMS
means (namely checking the supplied credentials against those in SYSUAF). In
its present form the plugin verifies only that valid username and password
credentials have been supplied; it does nothing to control what authenticated
users can and cannot do once they are connected to the broker. Clearly in order
to use this plugin you need to have Mosquitto running on an OpenVMS system;
fortunately it is reasonably easy to port to OpenVMS; let me know if you need
a hand!

#Building and configuring the plugin
Note that in order to compile the plugin you will require a copy of the
Mosquitto source code. Assuming this to be the case, building the plugin should
be straightforward. If you do not have git for OpenVMS then it will be
necessary to clone the sources to a Linux or Windows system that does have
git installed and then copy them over to your OpenVMS system.

    $ git clone https://github.com/brc859844/mosquitto-auth-plugin-vms
    $ set default mosquitto-auth-plugin-vms

Now edit `build.com` and modify the include path specified in the CC command as
appropriate so that the compiler can find the Mosquitto header files.

    $ @build

You should now have `mosquitto_auth_plugin_vms.exe`.

To configure the plugin, copy the sharable image
(`mosquitto_auth_plugin_vms.exe`) to somewhere sensible, define a system-level
logical name `mosquitto_auth_plugin_vms` that equates to the shareable image,
and edit your Mosquitto configuration file to include the following details:

    # Authentication plugin (specify path to OpenVMS shareable image)
    auth_plugin mosquitto_auth_plugin_vms

You might also wish to set `allow_anonymous` to `false` in your configuration
file to disable anonymous (unathenticated) logins. It is probably also a good
idea for testing purposes to initially compile the plugin with `MQAP_DEBUG`
defined, as this will cause messages to be written to the brokers' `stderr`
about the operation of the plugin.

#Versions 
The latest tagged version of the code has been tested with Mosquitto
1.1.3 on OpenVMS Integrity (Itanium) 8.3; however it should work with Mosquitto
1.2 and OpenVMS 8.4.

