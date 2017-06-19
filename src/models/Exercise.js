import { Record } from 'immutable'

/**
 * @property {number} id
 * @property {string} name
 * @property {number} sets
 * @property {number} reps
 * @property {number} interval
 * @property {number|string} weight
 * @property {string} note
 * @property {boolean} active
 */
export default class Exercise extends Record({
  id: 0,
  name: '',
  sets: 3,
  reps: 12,
  interval: 60,
  weight: '',
  note: '',
  active: true
}) {

  constructor(id, data = {}) {
    data.id   = id
    data.name = data.name || `Exercise #${data.id}`
    super(data)
  }

  withStatus(bool) {
    return new Exercise(this.id, Object.assign({}, this.toJS(), { active: bool }))
  }

}