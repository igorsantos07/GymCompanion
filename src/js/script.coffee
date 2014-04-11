MINI = require 'minified'
_    = MINI._
$    = MINI.$
$$   = MINI.$$
EE   = MINI.EE
HTML = MINI.HTML
$body= $('body')
$workoutJumper = $('nav select')


############################# Internationalization #############################

i18n =
	skipTags: ['SCRIPT', 'EM']
	skipWords: ['Gym', 'Companion', 'sec', 'kg/lb', ' :)']
	user: navigator.language.split('-')[0]
	source: 'en'
	Exception: (@tag, @string, @missingAt)->
		@name = 'i18nException'
		@toString = -> "#{@name}: Missing translation for #{@missingAt} lang: <#{@tag}> '#{@string}'"
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
		' New Exercise',
		'Donations',
		'Ideas',
		'Source code',
		'Contact me'
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
		'Doações',
		'Idéias',
		'Código-fonte',
		'Contato'
	]
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
					console.error new i18n.Exception node.parentNode.nodeName, node.nodeValue, error

i18n.translate $$('head title')
i18n.translate $$('body')

##################### Global functions, objects and stuff ######################

#### Don't change those, they're changed on runtime by pebble-integration script
window.isPebble = false
window.isPebble = true if window.location.hash.indexOf('pebble') >= 0
window.data = "{}"

window.close = (data)->
	console.log "Got this: #{data}"
	url = "pebblejs://close##{encodeURIComponent(data)}"
	if window.isPebble
		window.location = url
	else
#		console.log "Should go to #{url}"
		localStorage.data = data
		console.log "Data saved to localStorage, since you're not on the Pebble app"

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

if window.isPebble
	$flash = $('#flash')
	$flash.on 'click', -> $flash.set '$opacity', 0
	console.log = (msg)->
		msg = JSON.stringify(msg) if typeof msg == 'object'
		$flash.fill msg
		$flash.set '$opacity', 1
else
	window.data = localStorage.data || '{}'

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

	toJSON: ->
		JSON.stringify @getSecureValues()

	@fromJSON: (json)->
		new Exercise(json.name, json.sets, json.reps, json.weight, json.interval, json.id)

	getSecureValues: ->
		values = {}
		Exercise.properties.forEach (prop)=> values[prop] = @[prop]
		values

	toString: -> @toJSON()

class Workout
	@letters: ['A', 'B', 'C', 'D', 'E', 'F', 'G']
	@list: []
	exercises: undefined

	constructor: (animated = true, @interval, exercises = []) ->
		if Workout.list.length == Workout.letters.length-1
			$('.newWorkout').set disabled: true
		else if Workout.list.length >= Workout.letters.length
			return false

		@root      = $tpl_workout.clone()
		workout_id = Workout.list.length
		letter     = Workout.letters[workout_id]
		@root.set '@id', "workout_#{workout_id}"
		$('h2 em', @root).fill letter
		$('[name=workout_interval]', @root).set '@value', @interval
		$('.newExercise', @root).set '%workout', workout_id
		Workout.list.push @

		@root.set '$$slide', 0 if animated
		$('.buttons:last-child', $root).addBefore @root
		if animated
			@root.animate {$$slide: 1}, 300
			.then => @root.set '$height', 'auto'
			window.scrollToElement @root, => $('input', @root)[0]?.focus()
			# purposely out of the then(), scrolling while the element appears

		$workoutJumper.add EE('option', {'@value': workout_id}, "Workout #{letter}")

		@exercises = []
		exercises.forEach (e)=> @addExercise(false, e)

	addExercise: (animated = true, exercise = null) ->
		if !exercise then exercise = new Exercise
		root = $tpl_exercise.clone()
		exercise.id = @exercises.length + 1
		exercise.setRoot root
		@exercises.push exercise

		# $('input', root).set 'name', (v) => "#{v}_#{@total_exercises}"
		root.set '$$slide', 0 if animated
		$('.newExercise:last-child', @root).addBefore root
		root.animate {$$slide: 1}, 300 if animated
		if animated
			window.scrollToElement root, -> $('input', root)[0].focus()

	update: ->
		@interval = parseInt $('[name=workout_interval]', @root).get('value')
		@exercises.forEach (e)-> e.update()
		$('.badge', @root).set $display: 'none'

	toJSON: ->
		JSON.stringify
			interval: @interval
			exercises: JSON.parse @exercises.listToJSON()

	@rehydrate: (pojo)->
		exercises = []
		if Array.isNonEmptyArray pojo.exercises
			pojo.exercises.forEach (e)-> exercises.push Exercise.fromJSON(e)
		new Workout(false, pojo.interval, exercises)

	@fromJSON: (json)->
		Workout.rehydrate JSON.parse(json)

	@restore: (json)->
		data = JSON.parse json
		if Array.isNonEmptyArray data
			workouts = []
			data.forEach (single_data)-> workouts.push Workout.rehydrate(single_data)
			return workouts
		else
			return Workout.rehydrate(data)

	toString: -> @toJSON()


################################# Events and interactions #################################

$(window).on 'scroll', (->
	floatHeader = $('header.main').toggle 'floating'
	-> floatHeader($body.get('scrollTop') > 35) # partially hidden top buttons
)()

$workoutJumper.on 'change', ->
	window.scrollToElement $("#workout_#{@[0].value}")
	console.log "Should go to #{@[0].value}" # TODO: report that this.get('@value') does not work for selects

$('.newWorkout').on 'click', ->
	new Workout

$('form').on 'click', ->
	workout_id = this.get '%workout'
	Workout.list[workout_id].addExercise()
, 'fieldset .newExercise' #this is made this way to work like old jQuery's .live()

$('.save').on 'click', ->
	Workout.list.forEach (w)-> w.update()
	window.close Workout.list.listToJSON()


################################## Template boilerplating ##################################

$root = $('form')
$tpl_exercise = $('#template_exercise')
$tpl_exercise.remove()
$tpl_workout = $('#template_workout')
$tpl_workout.remove()

#### Restoring previous state
Workout.restore window.data