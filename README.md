# ESP_SelectWifi
Utility project to select wifi network to connect ESP32 to from a list of available networks

## How does it work

ESP board does not connect to any wifi on start.
ESP board is set to access point + station.
Connecting to the board from a web-browser displays available networks.
User can select the network to connect to and provides the password for it.

## Why do you need it

Provides more flexibility to projects intended to be run in different locations.
Equally useful for distributing a board to e. g. customers where the network cannot be known in advance.
Password to network no longer needs to be hard coded in the source code.

## Todos

Provide password to network through a form.
Refactor project into a library.

