if (typeof window.config_html == 'undefined')
	window.config_html = '<h1>Config file not set</h1>';

Pebble.addEventListener 'ready', (e) ->
  console.log('Starting PebbleKit JS')

Pebble.addEventListener 'showConfiguration', (e) ->
  data = window.localStorage.getItem('data') || "{}"
  console.log "Got this from the local storage: #{data}"
  window.config = window.config_html.replace 'DUMMY_DATA', data
  Pebble.openURL('data:text/html,' + encodeURIComponent(window.config_html + '<!--.html'))

Pebble.addEventListener 'webviewclosed', (e) ->
  return false if e.response == 'CANCELLED'
  data = decodeURIComponent(e.response)
  console.log "Back from config. Here follows the data: #{data}"
  window.localStorage.addItem('data', data)
  console.log "What's stored now: #{window.localStorage.getItem('data')}"