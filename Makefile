all:
	pebble build
clean:
	pebble clean
install:
	pebble install --logs --phone 192.168.43.1
desk-install:
	pebble install --logs --phone 192.168.0.14
bth-install:
	pebble install --logs --pebble_id 00:17:EC:51:A0:E5
