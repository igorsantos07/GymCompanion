import { List } from 'immutable'
export default class Workout {

  /** @type number */
  id

  /** @type string */
  letter

  /** @type string */
  name

  /** @type boolean */
  active = true

  /** @type Immutable.List */
  exercises

  constructor(id, name = null) {
    this.id        = id
    this.letter    = String.fromCharCode('A'.charCodeAt(0) + id - 1)
    this.name      = name || 'Workout ' + this.letter
    this.exercises = new List()
  }
}