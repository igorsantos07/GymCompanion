import { Record, List } from 'immutable'
import Exercise from './Exercise'

/**
 * @property {number} id
 * @property {string} letter
 * @property {string} name
 * @property {boolean} active
 * @property {Immutable.List<Exercise>} exercises
 */
export default class Workout extends Record({
  id: 0,
  letter: 'A',
  name: '',
  active: true,
  exercises: new List()
}) {

  constructor(id, data = {}) {
    data.id     = id
    data.letter = String.fromCharCode('A'.charCodeAt(0) + id - 1)
    data.name   = data.name || 'Workout ' + data.letter
    super(data)
  }

  pushExercise(exercise = null) {
    return new Workout({
      id       : this.id,
      letter   : this.letter,
      name     : this.name,
      active   : this.active,
      exercises: this.exercises
                     .map(e => e.mutate({ active: false }))
                     .push(exercise || new Exercise(this.exercises.last().id + 1))
    })
  }

  mutate(data) {
    return new Workout(this.id, Object.assign(this.toJS(), data))
  }

}