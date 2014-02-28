if (typeof window.config_html == 'undefined')
	window.config_html = '<h1>Config file not set</h1>';

Pebble.addEventListener 'ready', (e) ->
  console.log('Starting PebbleKit JS')

Pebble.addEventListener 'showConfiguration', (e) ->
	Pebble.openURL('data:text/html,' + encodeURIComponent(window.config_html + '<!--.html'))

Pebble.addEventListener 'webviewclosed', (e) -> 
  console.log "Config window returned: #{e.response}"