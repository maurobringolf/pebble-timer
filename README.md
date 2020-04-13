# Pebble timer

* `vagrant up`
* `vagrant halt`
* `vagrant reload`
* `vagrant ssh`

## Inside the VM

* `TERM=xterm && sudo su - ubuntu`
* `cd timer`
* `pebble build`
* `pebble build && pebble install -v --phone <PHONE_IP>` (ip address shown in app on phone, only works in same WIFI)

