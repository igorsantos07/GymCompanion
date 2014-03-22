MINI = require 'minified'
_    = MINI._
$    = MINI.$
$$   = MINI.$$
EE   = MINI.EE
HTML = MINI.HTML
$body= $('body')

##################### Global functions, objects and stuff ######################

window.isPebble = false
window.isPebble = true if window.location.hash.indexOf('pebble') >= 0
window.data = "{}"

window.close = (data)->
	console.log "Got this to submit: #{data}"
	url = "pebblejs://close##{encodeURIComponent(data)}"
	if window.isPebble
		window.location = url
	else
		console.log "Should go to #{url}"
		localStorage.setItem 'data', data
		console.log "Data saved to localStorage, since you're not on the Pebble app"

window.scrollToElement = ($element)->
	windowTop  = $body.get 'scrollTop'
	topDiff    = $element[0].getBoundingClientRect().top - 15
	color      = $element.get '$background-color'
	altColor   = '#BBB'
	blinkTime  = 350
	scrollTime = if topDiff != 0 then 500 else 0

	$body.animate { scrollTop: windowTop + topDiff }, scrollTime
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

Array.prototype.toJSON = ->
	json = @map (e)-> if e.toJSON? then e.toJSON() else JSON.stringify(e)
	"[#{json.join()}]"

############################## Classes definition ##############################

class Exercise
	constructor: (@name, @sets, @reps, @weight, @interval, @id = NaN) ->

	setRoot: (@root) ->
		$('[name*=name]', @root).set 'placeholder', (v) => v.replace(/#\d*/, "##{@id}")
		$('input', @root).set 'value', (v, i, obj) => if @[obj.name] then @[obj.name] else ''

	update: ->
		$('input', @root).each (e) =>
			@[e.name] = (if e.type == 'number' then parseInt(e.value) else e.value) || undefined

	toJSON: ->
		data = {}
		@getSecureProperties().forEach (prop)=> data[prop] = @[prop]
		JSON.stringify data

	@fromJSON: (json)->
		new Exercise(json.name, json.sets, json.reps, json.weight, json.interval, json.id)

	getSecureProperties: ->
		['id','name','sets','reps','weight','interval']

	toString: -> @toJSON()

class Workout
	@letters: ['A', 'B', 'C', 'D', 'E', 'F', 'G']
	@list: []
	exercises: []

	constructor: (animated = true, @interval, exercises = []) ->
		@root = $tpl_workout.clone()
		workout_id = Workout.list.length
		$('h2 em', @root).fill Workout.letters[workout_id]
		$('[name=workout_interval]', @root).set '@value', @interval
		$('.newExercise', @root).set '%workout', workout_id
		Workout.list.push @

		@root.set '$$slide', 0 if animated
		$('.buttons:last-child', $root).addBefore @root
		if animated
			@root.animate {$$slide: 1}, 300
			.then => @root.set '$height', 'auto'
			window.scrollToElement @root
			# purposely out of the then(), scrolling while the element appears

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
			window.scrollToElement root

	update: ->
		@interval = parseInt $('[name=workout_interval]', @root).get('@value')

	toJSON: ->
		JSON.stringify
			interval: @interval
			exercises: JSON.parse @exercises.toJSON()

	@fromJSON: (json)->
		data = JSON.parse json
		exercises = []
		data.exercises.forEach (e)-> exercises.push Exercise.fromJSON(e)
		new Workout(false, data.interval, exercises)

	toString: -> @toJSON()


################################# Working code #################################

if isPebble
	window.data = JSON.parse window.data
else
	window.data = JSON.parse(localStorage.getItem('data') || '{}')

$root = $('form')
$tpl_exercise = $('#template_exercise')
$tpl_exercise.remove()
$tpl_workout = $('#template_workout')
$tpl_workout.remove()

$('#newWorkout').on 'click', ->
	new Workout

$('form').on 'click', ->
	workout_id = $(this).get('%workout')
	Workout.list[workout_id].addExercise()
, 'fieldset .newExercise' #this is made this way to work like old jQuery's .live()

$('.save').on 'click', ->
	Workout.list.forEach (w)-> w.exercises.forEach (e)-> e.update()
	window.close Workout.list.toJSON()