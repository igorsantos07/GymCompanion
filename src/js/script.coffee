MINI = require 'minified'
_    = MINI._
$    = MINI.$
$$   = MINI.$$
EE   = MINI.EE
HTML = MINI.HTML

##################### Global functions, objects and stuff ######################

window.isPebble = false
window.data = "{}"

window.close = (data)->
	url = "pebblejs://close##{encodeURIComponent(data)}"
	if window.isPebble
		window.location = url
	else
		console.log "Got this to submit: #{data}"
		console.log "Should go to #{url}"
		localStorage.setItem 'data', data
		console.log "Data saved to localStorage, since you're not on the Pebble app"

if window.isPebble
	$flash = $('#flash')
	$flash.on 'click', -> $flash.set '$opacity', 0
	console.log = (msg)->
		$flash.fill msg
		$flash.set '$opacity', 1

Array.prototype.toJSON = ->
	json = @map (e)-> if e.toJSON? then e.toJSON() else JSON.stringify(e)
	"[#{json.join()}]"


console.log(window.data)


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
		$('form > .save:last-child', $root).addBefore @root
		if animated
			@root.animate {$$slide: 1}, 300
			.then => @root.set '$height', 'auto'

		exercises.forEach (e)=> @addExercise(false, e)

	addExercise: (animated = true, exercise = null) ->
		if !exercise then exercise = new Exercise
		root = $tpl_exercise.clone()
		exercise.id = @exercises.length + 1
		exercise.setRoot root
		@exercises.push exercise

		# $('input', root).set 'name', (v) => "#{v}_#{@total_exercises}"
		root.set '$$slide', 0 if animated
		$('.newExercise', @root).addBefore root
		root.animate {$$slide: 1}, 300 if animated
		if animated
			console.log('should move to the top of this root') #TODO

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
	Workout.list.forEach (w)->
		w.exercises.forEach (e)->
			e.update()
	window.close Workout.list.toJSON()

##### Sample code
$('h1').fill window.data
e = new Exercise('Abdominal', 4, 20, 10, 60)
w = new Workout(false, 60)
w.addExercise(false, e)
w.addExercise(false)