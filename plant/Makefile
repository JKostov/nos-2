####
#### Sample Makefile for building applications with the RIOT OS
####
#### The example file system layout is:
#### ./application Makefile
#### ../../RIOT
####

# Set the name of your application:
APPLICATION = plant

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../

# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS

# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

# Uncomment this to enable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP = 1

# Change this to 0 to show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:

ifneq (,$(DEFAULT_MQTT_CLIENT_ID))
  CFLAGS += -DDEFAULT_MQTT_CLIENT_ID=\"$(DEFAULT_MQTT_CLIENT_ID)\"
endif
ifneq (,$(DEFAULT_MQTT_USER))
  CFLAGS += -DDEFAULT_MQTT_USER=\"$(DEFAULT_MQTT_USER)\"
endif
ifneq (,$(DEFAULT_MQTT_PWD))
  CFLAGS += -DDEFAULT_MQTT_PWD=\"$(DEFAULT_MQTT_PWD)\"
endif

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += netdev_default
USEPKG += paho-mqtt
USEPKG += jsmn

USEMODULE += ipv6_addr
USEMODULE += lwip_ipv6_autoconfig

USEMODULE += lwip_netdev
USEMODULE += lwip

USEMODULE += sock_async_event
USEMODULE += sock_ip
USEMODULE += sock_udp
USEMODULE += sock_tcp

# If your application is very simple and doesn't use modules that use
# messaging, it can be disabled to save some memory:

#DISABLE_MODULE += core_msg

#INCLUDES += -Iapplication_include

# Specify custom dependencies for your application here ...
# APPDEPS = app_data.h config.h

include $(RIOTBASE)/Makefile.include

# ... and define them here (after including Makefile.include,
# otherwise you modify the standard target):
#proj_data.h: script.py data.tar.gz
#	./script.py

ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
endif

USEMODULE += plant_room
INCLUDES += -I$(CURDIR)/room
DIRS += $(CURDIR)/room

USEMODULE += plant_sensors
INCLUDES += -I$(CURDIR)/sensors
DIRS += $(CURDIR)/sensors

USEMODULE += broker_wrapper
INCLUDES += -I$(CURDIR)/broker
DIRS += $(CURDIR)/broker
