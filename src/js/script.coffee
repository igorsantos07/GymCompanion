MINI = require 'minified'
_    = MINI._
$    = MINI.$
$$   = MINI.$$
EE   = MINI.EE
HTML = MINI.HTML

$root = $('form')
$tpl_exercise = $('#template_exercise')
$tpl_exercise.remove()
$tpl_workout  = $('#template_workout')
$tpl_workout.remove()

class Exercise
	constructor: (@name, @sets, @reps, @weight, @interval, @id = NaN) ->

	setRoot: (@root) ->
		$('[name*=name]', @root).set 'placeholder', (v) => v.replace(/#\d*/, "##{@id}")
		$('input', @root).set 'value', (v, i, obj) => if @[obj.name] then @[obj.name] else ''

	update: ->
		$('input', @root).each (e) =>
			@[e.name] = if e.type == 'number' then parseInt(e.value) else e.value

class Workout
	@letters: ['', 'A', 'B', 'C', 'D', 'E', 'F', 'G']
	@total: 0
	@list: []
	exercises: []

	constructor: (animated = true, workout = null) ->
		++Workout.total
		@root = $tpl_workout.clone()
		$('h2 em', @root).fill Workout.letters[Workout.total]
		Workout.list.push @

		@root.set '$$slide', 0
		$root.add @root
		@root.animate {$$slide: 1}, if animated then 300 else 1

	addExercise: (animated = true, exercise = null) ->
		if !exercise then exercise = new Exercise
		root = $tpl_exercise.clone()
		exercise.id = @exercises.length + 1
		exercise.setRoot root
		@exercises.push exercise

		# $('input', root).set 'name', (v) => "#{v}_#{@total_exercises}"
		root.set '$$slide', 0
		@root.add root
		root.animate {$$slide: 1}, if animated then 300 else 1

$('#newWorkout').on 'click', ->
	new Workout


# Sample code
w = new Workout
w.addExercise(false)
e = new Exercise('Abdominal', 4, 20, 10, 60)
w.addExercise(false, e)