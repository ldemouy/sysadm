.. _network:

network
*******

The network class is used to manage and retrieve information from Ethernet and wireless network devices.

Every network class request contains the following parameters:

+---------------------------------+---------------+----------------------------------------------------------------------------------------------------------------------+
| **Parameter**                   | **Value**     | **Description**                                                                                                      |
|                                 |               |                                                                                                                      |
+=================================+===============+======================================================================================================================+
| id                              |               | any unique value for the request; examples include a hash, checksum, or uuid                                         |
|                                 |               |                                                                                                                      |
+---------------------------------+---------------+----------------------------------------------------------------------------------------------------------------------+
| name                            | sysadm        |                                                                                                                      |
|                                 |               |                                                                                                                      |
+---------------------------------+---------------+----------------------------------------------------------------------------------------------------------------------+
| namespace                       | rpc           |                                                                                                                      |
|                                 |               |                                                                                                                      |
+---------------------------------+---------------+----------------------------------------------------------------------------------------------------------------------+
| args                            |               | values vary by type of request                                                                                       |
|                                 |               |                                                                                                                      |
+---------------------------------+---------------+----------------------------------------------------------------------------------------------------------------------+

The rest of this section provides examples of the available *args* for each type of request, along with their responses.

.. _Network Devices:

Network Devices
===============

The requests in this section work for both Ethernet and wireless devices. When specifying the *nic*, use the FreeBSD Ethernet device name, or
*wlan#* for wireless devices (where
*#* is the wireless device number).

.. _Get Interface Information:

Get Interface Information
-------------------------

**Request**

.. code-block:: json

 {
  "id": "someUniqueID",
  "name": "sysadm",
  "namespace": "rpc",
  "args": {
      "getNicSettings": {
          nic: "em0"
      }
   }
 }


**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "getNicSettings": {
          nic: "em0",
          nic_desc: "Intel(R) Pro/100 Connection 7.4.2",
          nic_mac: "<foo>"
          nic_status: "active"
          nic_media: "1000baseT <full-duplex>",
          is_wired: true,
          ip4_addr: "192.168.0.50",
          ip4_netmask: "255.255.255.0",
          ip4_dhcp: false,
          ip6_addr: "<foo>",
          ip6_auto: false,
      }
   }
 }

.. _Set Interface Configuration:

Set Interface Configuration
---------------------------

**Request**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "sysadm",
   "namespace": "rpc",
   "args": {
      "saveNicSettings": {
          nic: "em0",
          nic_mac: "<foo>"
          ip4_addr: "192.168.0.50",
          ip4_netmask: "255.255.255.0",
          ip4_dhcp: false,
          ip6_addr: "<foo>",
          ip6_auto: false
      }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "setNicSettings": {
          nic: "em0",
          nic_desc: "Intel(R) Pro/100 Connection 7.4.2",
          nic_mac: "<foo>"
          nic_status: "active"
          nic_media: "1000baseT <full-duplex>",
          is_wired: true,
          ip4_addr: "192.168.0.50",
          ip4_netmask: "255.255.255.0",
          ip4_dhcp: false,
          ip6_addr: "<foo>",
          ip6_auto: false,
      }
   }
 }

.. _Get System Network Configuration:

Get System Network Configuration
--------------------------------

**Request**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "sysadm",
   "namespace": "rpc",
   "args": {
      "getSystemNetworkConfig": ""
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "getSystemNetworkConfig": {
          ip4_gateway: "192.168.0.1",
          ip4_dns1: "208.26.222.222",
          ip4_dns2: "8.8.8.8",
          ip6_gateway: "",
          ip6_dns1: "",
          ip6_dns2: "",
          domain: "example.org",
          hostname: "mydesktop"
          proxy: false,
          proxy_addr: "fooproxy.org",
          proxy_port: 8080
          proxy_type: "(basic/digest)"
          proxy_user: "myusername",
          proxy_pass: "mypass"
      }
   }
 }

.. _Set System Network Configuration:

Set System Network Configuration
--------------------------------

**Request**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "sysadm",
   "namespace": "rpc",
   "args": {
      "setSystemNetworkConfig": {
          ip4_gateway: "192.168.0.1",
          ip4_dns1: "208.26.222.222",
          ip4_dns2: "8.8.8.8",
          ip6_gateway: "",
          ip6_dns1: "",
          ip6_dns2: "",
          domain: "example.org",
          hostname: "mydesktop"
          proxy: false,
          proxy_addr: "fooproxy.org",
          proxy_port: 8080
          proxy_type: "(basic/digest)"
          proxy_user: "myusername",
          proxy_pass: "mypass"
      }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "setSystemNetworkConfig": {
          ip4_gateway: "192.168.0.1",
          ip4_dns1: "208.26.222.222",
          ip4_dns2: "8.8.8.8",
          ip6_gateway: "",
          ip6_dns1: "",
          ip6_dns2: "",
          domain: "example.org",
          hostname: "mydesktop"
          proxy: false,
          proxy_addr: "fooproxy.org",
          proxy_port: 8080
          proxy_type: "(basic/digest)"
          proxy_user: "myusername",
          proxy_pass: "mypass"
      }
   }
 }

.. _Wireless Settings:

Wireless Settings
=================

This section describes the additional requests available for wireless devices.

.. _List Available APs:

List Available APs
------------------

**Request**

.. code-block:: json

 {
 "id": "someUniqueID",
 "name": "sysadm",
 "namespace": "rpc",
 "args": {
      "getWifiAPs": {
      }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "getWifiAPs": {
          "myap": {
          ssid: "myap",
          bssid: "",
          security: "(wep|wpa|wpae)",
          key: ""         
         },
          "workap": {
          ssid: "workap",
          bssid: "",
          security: "(wep|wpa|wpae)",
          key: ""         
         },
       }
   }
 }

.. _List APs Within Range:

List APs Within Range
---------------------

**Request**

.. code-block:: json

 {
 "id": "someUniqueID",
 "name": "sysadm",
 "namespace": "rpc",
 "args": {
      "scanWifiAPs": {
      }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "scanWifiAPs": {
          "myap": {
          ssid: "myap",
          bssid: "",
          security: "(wep|wpa|wpae)",
          rate: "",
          signoise: "",
          caps: "",
          channel: "",
          inte: "",
          caps: "",
             },
          "workap": {
          ssid: "workap",
          bssid: "",
          security: "(wep|wpa|wpae)",
          rate: "",
          signoise: "",
          caps: "",
          channel: "",
          inte: "",
          caps: "",
         },
       }
   }
 }

.. _Connect to Specified AP:

Connect to Specified AP
-----------------------

**Request**

.. code-block:: json

 {
 "id": "someUniqueID",
 "name": "sysadm",
 "namespace": "rpc",
 "args": {
      "addWifiAP": {
        ssid: "myap",
        bssid: "",
        security: "(wep|wpa|wpae)",
        key: "mywifikey",
        wephex: false
        }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "addWifiAP": {
            ssid: "myap",
        bssid: "",
        security: "(wep|wpa|wpae)",
        key: "mywifikey",
        wephex: false     
       }
   }
 }

.. _Remove Specified AP:

Remove Specified AP
-------------------

**Request**

.. code-block:: json

 {
 "id": "someUniqueID",
 "name": "sysadm",
 "namespace": "rpc",
 "args": {
      "delWifiAP": {
        ssid: "myap",
        bssid: ""
        }
   }
 }

**Response**

.. code-block:: json

 {
   "id": "someUniqueID",
   "name": "response",
   "namespace": "rpc",
   "args": {
      "delWifiAP": {
            ssid: "myap",
        bssid: ""
       }
   }
 }