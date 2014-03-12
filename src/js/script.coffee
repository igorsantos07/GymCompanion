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
  @letters: ['A', 'B', 'C', 'D', 'E', 'F', 'G']
  @list: []
  exercises: []

  constructor: (animated = true, workout = null) ->
    @root = $tpl_workout.clone()
    workout_id = Workout.list.length
    $('h2 em', @root).fill Workout.letters[workout_id]
    $('.newExercise', @root).set '%workout', workout_id
    Workout.list.push @

    @root.set '$$slide', 0 if animated
    $('.save:last-child', $root).addBefore @root
    if animated
      @root.animate {$$slide: 1}, 300
      .then => @root.set '$height', 'auto'

  addExercise: (animated = true, exercise = null) ->
    if !exercise then exercise = new Exercise
    root = $tpl_exercise.clone()
    exercise.id = @exercises.length + 1
    exercise.setRoot root
    @exercises.push exercise

    # $('input', root).set 'name', (v) => "#{v}_#{@total_exercises}"
    root.set '$$slide', 0 if animated
    @root.add root
    root.animate {$$slide: 1}, 300 if animated
    if animated
      console.log('should move to the top of this root') #TODO

$('#newWorkout').on 'click', ->
  new Workout
  
$('form').on 'click', ->
  workout_id = $(this).get('%workout')
  Workout.list[workout_id].addExercise()
, 'fieldset .newExercise' #this is made this way to work like old jQuery's .live()

# Sample code
e = new Exercise('Abdominal', 4, 20, 10, 60)
w = new Workout(false)
w.addExercise(false, e)
w.addExercise(false)