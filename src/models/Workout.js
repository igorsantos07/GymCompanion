import { List } from 'immutable'
export default class Workout {

  static lastId     = 0
  static lastLetter = 64

  /** @type number */
  id

  /** @type string */
  letter

  /** @type string */
  name

  /** @type Immutable.List */
  exercises

  constructor(name = null) {
    this.id        = ++Workout.lastId
    this.letter    = String.fromCharCode(++Workout.lastLetter)
    this.name      = name || 'Workout ' + this.letter
    this.exercises = new List()
  }
}