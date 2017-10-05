# transceiver #

### IMPORTANT ###

Rx for the transceiver does NOT use a ring buffer...in other words, things will break if we receive a message thats larger than the max size defined in `hc12.h`. That should be fine for now but we may need to revisit later.