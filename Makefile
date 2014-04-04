all:
	pebble build
web:
	./web_compile
clean:
	pebble clean
install:
	pebble install --logs --phone 192.168.43.1
desk-install:
	pebble install --logs --phone 192.168.0.14
bth-install:
	pebble install --logs --pebble_id 00:17:EC:51:A0:E5
deploy:
	npm install
	./web_compile
	cp build/src/js/complete.html /var/www/gymcompanion/index.html
	cp src/js/favicon* /var/www/gymcompanion/
	cp src/js/libs/bugsense.min.js /var/www/bugsense.min.js
	cp config/post-receive.githook /root/projects/gymcompanion.git/hooks/post-receive
	cp config/gymcompanion.host /etc/apache2/sites-available/gymcompanion.conf
	service apache2 reload