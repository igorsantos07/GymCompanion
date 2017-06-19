export default class Exercise {

  static lastId = 0

  /** @type number */
  id

  /** @type string */
  name

  /** @type number */
  sets

  /** @type number */
  reps

  /** @type number */
  interval

  /** @type number|string */
  weight

  /** @type string */
  note


  constructor(name, sets, reps, interval, weight = null, note = '') {
    this.id       = ++Exercise.lastId
    this.name     = name
    this.sets     = sets
    this.reps     = reps
    this.interval = interval
    this.weight   = weight
    this.note     = note
  }
}