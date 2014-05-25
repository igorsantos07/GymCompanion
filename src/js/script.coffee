MINI = require 'minified'
_    = MINI._
$    = MINI.$
$$   = MINI.$$
EE   = MINI.EE
HTML = MINI.HTML

$body          = $('body')
$form          = $('form')
$workoutJumper = $('nav select')

############################# Internationalization #############################

throwError = (exception)->
	if Bugsense?.config.apiKey?
		Bugsense.notify exception
	else
		console.error "Uncaught #{exception.name}: #{exception.message}"

i18n =
	skipTags: ['SCRIPT', 'EM']
	skipWords: ['Gym', 'Companion', 'sec', 'kg/lb', ' :)']
	user: navigator.language.split('-')[0]
	source: 'en'
	Exception: (@tag, @string, @lang)->
		@name     = 'i18nException'
		@toString = -> "Missing translation for #{@lang} lang: <#{@tag}> '#{@string}'"
		@message  = @toString()
		this
	en: [
		'GymCompanion - Workout buddy'
		'Jump to...'
		' Add new Workout'
		'You have up to 7 workouts'
		' Done'
		'Workout '
		'Unsaved'
		'Seconds between exercises:'
		'Sets:'
		'Reps:'
		'Weight:'
		'Interval:'
		' New Exercise'
		'Buy me a beer!'
		'Ideas / Bugs'
		'Contact me'
		'Your workout was saved!'
		"Yeah, I'm awesome!"
		"<strong>Thank for the help!</strong><br />You're making GymCompanion happen :D"
		'Whoops! Did you give up on donating?<br/>Think again, protein bars are expensive! :('
		'Donor VIP status is over...<br/>How about a small donation again this month? (:'
		'OK'
		'Cancel'
	]
	pt: [
		'GymCompanion - Parceiro de academia'
		'Ir para...'
		' Criar nova Série'
		'Você tem até 7 séries'
		' Feito'
		'Série '
		'Não salvo'
		'Segundos entre os exercícios:'
		'Séries:'
		'Reps.:'
		'Peso:'
		'A cada:'
		' Novo exercício',
		'Me dê uma cerveja!'
		'Idéias / Bugs'
		'Contato'
		'Sua série foi salva!'
		"Valeu, eu sou foda!"
		"<strong>Obrigado pela ajuda!</strong><br />Pessoas como você fazem o GymCompanion acontecer :D"
		'Oops! Desistiu de doar?<br/>Pensa de novo, as barrinhas de proteína são caras! :('
		'Seu status VIP de doador acabou...<br/>Que tal outra doação pequena esse mês? (:'
		'OK'
		'Cancelar'
	]

	###
	# Simply translates a string
	# @param string v The value to be translated
	# @return string
	###
	t: (v)->
		if (index = @[@source].indexOf(v)) != -1
			if @[@user][index]?
				return @[@user][index]
			else
				error = 'user'
		else
			error = 'source'

		if typeof error == 'string'
			throwError new i18n.Exception '{embed}', v, error
		return v

	###
	# Translates an entire element and all it's child nodes
	# @param HTMLElement e The element to be translated
	# @return void
	# @throws i18n.Exception
	###
	translate: (e)->
		if @user == @source then return
		for node in e.childNodes
			if node.nodeType == 1 && @skipTags.indexOf(node.nodeName) == -1
				@translate node
			else if node.nodeType == 3 && @skipWords.indexOf(node.nodeValue) == -1
				error = false
				if (index = @[@source].indexOf(node.nodeValue)) != -1
					if @[@user][index]?
						node.nodeValue = @[@user][index]
					else
						error = 'user'
				else if !/^\s*$/.test(node.nodeValue)
					error = 'source'

				if typeof error == 'string'
					throwError new i18n.Exception node.parentNode.nodeName, node.nodeValue, error

i18n.translate $$('head title')
i18n.translate $$('body')

##################### Global functions, objects and stuff ######################

configureAlertify = ->
	alertify.set
		delay: 1500
		labels:
			ok: i18n.t 'OK'
			cancel: i18n.t 'Cancel'

#### Don't change those, they're changed on runtime by pebble-integration script
window.isPebble = false
window.isPebble = true if window.location.hash.indexOf('pebble') >= 0
window.data = "{}"

window.close = (data)->
	#console.log "Got this: #{data}"
	url = "pebblejs://close##{encodeURIComponent(data)}"
	if window.isPebble
		window.location = url
	else
		localStorage.data = data
		console.log "#{data.length} bytes of data saved to localStorage, since you're not on the Pebble app"

window.scrollToElement = ($element, callback)->
	windowTop    = $body.get 'scrollTop'
	topDiff      = $element[0].getBoundingClientRect().top - 15
	headerHeight = $('header')[0].getBoundingClientRect().height
	color        = $element.get '$background-color'
	altColor     = '#BBB'
	blinkTime    = 350
	scrollTime   = if topDiff != 0 then 500 else 0

	$body.animate { scrollTop: windowTop + topDiff - headerHeight }, scrollTime
	.then -> callback?()
	.then =>
		$element.animate({'$background-color': altColor}, blinkTime)
		.then => $element.animate({'$background-color': color}, blinkTime)

window.configureLabels = (root, id, inputModifier = null)->
	$('input', root).per (input)->
		final_id = "#{input.get('name')}_#{id}"
		input.set '@id': final_id
		$('label', input.get('parentElement')).set '@for': final_id
		inputModifier?(input)

if window.isPebble
	$flash = $('#flash')
	$flash.on 'click', -> $flash.set '$opacity', 0
	console.log = (msg)->
		msg = JSON.stringify(msg) if typeof msg == 'object'
		$flash.fill msg
		$flash.set '$opacity', 1
else
	window.data = localStorage.data || '{}'

Array.prototype.each = Array.prototype.forEach
Array.prototype.listToJSON = ->
	json = @map (e)->
		if e.toJSON?			then e.toJSON()
		else if e.listToJSON?	then e.listToJSON()
		else					     JSON.stringify(e)
	"[#{json.join()}]"

if !Array.isArray
	Array.isArray = (stuff)-> Object.prototype.toString.call(stuff) == '[object Array]'

Array.isNonEmptyArray = (stuff)->
	Array.isArray(stuff) && stuff.length > 0

############################## Classes definition ##############################

class Exercise
	@properties: ['id','name','sets','reps','weight','interval']

	constructor: (@name, @sets, @reps, @weight, @interval, @id = NaN) ->

	setRoot: (@root) ->
		$('[name*=name]', @root).set 'placeholder', (v) => v.replace(/#\d*/, "##{@id}")
		$('input', @root).set 'value', (v, i, obj) => if @[obj.name] then @[obj.name] else ''

	update: ->
		$('input', @root).each (e) =>
			@[e.name] = (if e.type == 'number' then parseInt(e.value) else e.value) || undefined
			e.changed = false

	toJSON: ->
		JSON.stringify @getSecureValues()

	@fromJSON: (json)->
		new Exercise(json.name, json.sets, json.reps, json.weight, json.interval, json.id)

	getSecureValues: ->
		values = {}
		Exercise.properties.each (prop)=> values[prop] = @[prop]
		values

	toString: -> @toJSON()

class Workout
	@letters: ['A', 'B', 'C', 'D', 'E', 'F', 'G']
	@list: []
	exercises: undefined
	id: -1
	dirty: true

	constructor: (animated = true, @interval, exercises = []) ->
		if Workout.list.length == Workout.letters.length-1
			$('.newWorkout').set disabled: true
		else if Workout.list.length >= Workout.letters.length
			return false

		@root  = $tpl_workout.clone()
		@id    = Workout.list.length
		letter = Workout.letters[@id]
		Workout.list.push @

		@root.set '@id': "workout_#{@id}"
		$('h2 em', @root).fill letter
		$('[name=workout_interval]', @root).set '@value': @interval, 'workout': @id
		$('.newExercise', @root).set workout: @id
		configureLabels(@root, @id)

		@root.set '$$slide', 0 if animated
		$('.buttons:last-child', $root).addBefore @root
		if animated
			@root.animate {$$slide: 1}, 300
			.then => @root.set '$height', 'auto'
			window.scrollToElement @root, => $('input', @root)[0]?.focus()
			# purposely out of the then(), scrolling while the element appears

		$workoutJumper.add EE('option', {'@value': @id}, "Workout #{letter}")

		@exercises = []
		exercises.each (e)=> @addExercise(false, e)

	addExercise: (animated = true, exercise = null) ->
		# TODO: where the heck are the exercise fields filled???
		# TODO: when you find that, please set the default interval to be the one from the workout
		if !exercise then exercise = new Exercise
		root = $tpl_exercise.clone()
		exercise.id = @exercises.length + 1
		exercise.setRoot root
		@exercises.push exercise
		configureLabels(root, "#{@id}_#{exercise.id}", (input)-> input.set workout: @id)

		root.set '$$slide', 0 if animated
		$('.newExercise:last-child', @root).addBefore root
		root.animate {$$slide: 1}, 300 if animated
		if animated
			window.scrollToElement root, -> $('input', root)[0].focus()

	setDirty: (@dirty)->
		# TODO: the encapsulation strategy used on window.scroll is not working here for some reason
		$('.badge', @root).set $visibility: (if @dirty then 'visible' else 'hidden')

	update: ->
		@interval = parseInt $('[name=workout_interval]', @root).get('value')
		@exercises.each (e)-> e.update()

	toJSON: ->
		@setDirty false
		JSON.stringify
			interval: @interval
			exercises: JSON.parse @exercises.listToJSON()

	@rehydrate: (pojo)->
		exercises = []
		if Array.isNonEmptyArray pojo.exercises
			pojo.exercises.each (e)-> exercises.push Exercise.fromJSON(e)
		new Workout(false, pojo.interval, exercises)

	@fromJSON: (json)->
		Workout.rehydrate JSON.parse(json)

	@restore: (json)->
		data = JSON.parse json
		if Array.isNonEmptyArray data
			workouts = []
			data.each (single_data)->
				w = Workout.rehydrate single_data
				w.setDirty false
				workouts.push w
			return workouts
		else
			return Workout.rehydrate data

	toString: -> @toJSON()


################################# Events and interactions #################################

$.ready ->
	now   = new Date()
	month = "#{now.getMonth()}"
	year  = "#{now.getFullYear()}"

	setDonor = (donated, persist = true)->
		if donated
			$('header h1').set '+vip'
			if persist then localStorage.donatedOn = "#{month}-#{year}"
		else
			$('header h1').set '-vip'
			if persist then localStorage.removeItem 'donatedOn'

	if localStorage.donatedOn?
		donateDate = localStorage.donatedOn.split('-')
		if donateDate[1] < year || (donateDate[1] == year && donateDate[0] < month)
			setDonor false
			alertify.set delay: 5000
			alertify.log i18n.t('Donor VIP status is over...<br/>How about a small donation again this month? (:')
		else
			setDonor true, false

	if location.hash.indexOf('#donation-') == 0
		# TODO: This will only work when paypal goes into HTTP or GymCompanion into HTTPS
		# if document.referrer.indexOf('paypal') != -1
		switch location.hash
			when '#donation-success'
					alertify.set { labels: { ok: i18n.t("Yeah, I'm awesome!") } }
					alertify.alert i18n.t("<strong>Thank for the help!</strong><br />You're making GymCompanion happen :D")
					setDonor true
			when '#donation-gaveup'
				location.hash = ''
				alertify.set delay: 10000
				alertify.error i18n.t('Whoops! Did you give up on donating?<br/>Think again, protein bars are expensive! :(')
		location.hash = ''

	configureAlertify()

$(window).on 'scroll', (->
	floatHeader = $('header.main').toggle 'floating'
	-> floatHeader($body.get('scrollTop') > 35) # partially hidden top buttons
)()

$workoutJumper.on 'change', ->
	window.scrollToElement $("#workout_#{@get('value')}")

$('.newWorkout').on 'click', ->
	new Workout

$form.on 'click', ->
	workout_id = this.get 'workout'
	Workout.list[workout_id].addExercise()
, 'fieldset .newExercise' #this is made this way to work like old jQuery's .live()

$form.onChange ->
	if !@get 'changed'
		@set changed: true
		Workout.list[@get('workout')].setDirty true
, 'input'

#TODO: replace both events by onFocus when this gets closed: https://github.com/timjansen/minified.js/issues/40
#TODO: not even the plain .on() method is working! https://github.com/timjansen/minified.js/issues/41
$form.on 'focus', ->
	@up('fieldset').set('+focused')
, 'input'

$form.on 'blur', ->
	@up('fieldset').set('-focused')
, 'input'

$('.save').on 'click', ->
	Workout.list.each (w)-> w.update()
	window.close Workout.list.listToJSON()
	alertify.success $$('#success-save').innerHTML


################################## Template boilerplating ##################################

$root = $('form')
$tpl_exercise = $('#template_exercise')
$tpl_exercise.remove()
$tpl_workout = $('#template_workout')
$tpl_workout.remove()

#### Restoring previous state
Workout.restore window.data