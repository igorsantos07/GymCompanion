import { List } from 'immutable'

export default class Group {

  static lastId = 0

  /** @type number */
  id

  /** @type Date */
  date

  /** @type string */
  name

  /** @type string */
  description

  /** @type Immutable.List */
  workouts

  constructor(name = '', description = '') {
    this.id          = ++Group.lastId
    this.date        = new Date()
    this.name        = name || `#${this.id} ${this.date.toLocaleString(undefined,{ month: 'short', year: 'numeric' })}`
    this.description = description
    this.workouts    = new List()
  }
}
