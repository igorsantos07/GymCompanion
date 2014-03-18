if (typeof window.config_html == 'undefined')
	window.config_html = '<h1>Config file not set</h1>';

Pebble.addEventListener 'ready', (e) ->
	return true

Pebble.addEventListener 'showConfiguration', (e) ->
	data = localStorage.getItem('data') || "{}"
	console.log "Got this from the local storage: #{data}"
	data = data.replace(/\\/g, '\\\\').replace(/"/g, "\\\"")
	window.config_html = window.config_html.replace 'window.data = "{}"', "window.data = \"#{data}\""
	window.config_html = window.config_html.replace 'isPebble = false', 'isPebble = true'
	Pebble.openURL('data:text/html,' + encodeURIComponent(window.config_html + '<!--.html'))

Pebble.addEventListener 'webviewclosed', (e) ->
	return false if e.response == 'CANCELLED'
	data = decodeURIComponent(e.response)
	console.log "Back from config. Here follows the data: #{data}"
	localStorage.setItem('data', data)