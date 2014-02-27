if (typeof Pebble == 'undefined')
	Pebble = {
		addEventListener: function(event_name, callback) {},
		openURL: function(url) {}
	}

if (typeof config_html == 'undefined')
	config_html = '<h1>Config file not set</h1>';

Pebble.addEventListener('ready', function(e) { console.log('Starting PebbleKit JS') })

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL('data:text/html,' + encodeURIComponent(config_html + '<!--.html'))
})

Pebble.addEventListener('webviewclosed', function(e) { console.log('Config window returned: '+e.response); })
