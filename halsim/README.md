# HALSIM
Implements HAL for simulated enviroment.

## client
Linked with user code that requires the hal. It implements communication to the hal server.

## server
Implements the devices being simulated, multiple client can connect to server through hal ipc messages.
Simulated devices be implented as shared object which server can load dynamically./.