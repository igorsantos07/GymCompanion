import { Record, List } from 'immutable'
import Workout from './Workout'

/**
 * @property {number} id
 * @property {Date} date
 * @property {string} name
 * @property {boolean} active
 * @property {Immutable.List<Workout>} workouts
 */
export default class Group extends Record({
  id: 0,
  date: null,
  name: '',
  description: '',
  active: true,
  workouts: new List([new Workout(1)])
}) {

  static lastId = 0

  constructor(data = {}) {
    data.id          = data.id || ++Group.lastId
    data.date        = data.date || new Date()
    data.name        = data.name || `#${data.id} ${data.date.toLocaleString(undefined, { month: 'short', year: 'numeric' })}`
    data.description = data.description || `No description for Workout Group ${data.name}`
    super(data)
  }

  pushWorkout(workout = null) {
    return new Group({
      id         : this.id,
      date       : this.date,
      name       : this.name,
      description: this.description,
      active     : this.active,
      workouts   : this.workouts
                       .map(w => w.mutate({ active: false }))
                       .push(workout || new Workout(this.workouts.last().id + 1))
    })
  }

  mutate(data) {
    return new Group(Object.assign(this.toJS(), data))
  }
}
