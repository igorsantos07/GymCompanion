import { fromJS } from 'immutable'
import { ACTIONS } from '../lib/constants'
import { combineReducers } from 'redux-immutable'
import Group from '../models/Group'

export const initialState = fromJS({
  groups: { list: [], active: 0 },
})

/**
 * @param {Immutable.Map} groups
 * @param {Object} action
 * @returns {*}
 */
const groups = (groups, action) => {
  switch (action.type) {
    case ACTIONS.ADD_GROUP:
      const group = new Group()
      return groups
        .set('list', groups.get('list').push(group).sortBy(g => g.id).reverse())
        .set('active', group.id)

    case ACTIONS.SET_ACTIVE_GROUP:
      return groups.set('active', action.id)

    case ACTIONS.ARCHIVE_GROUP:
      throw new Error('Not implemented')

    default:
      // if (action.type.indexOf('WORKOUT')) {
      //   const locator = [action.group, 'workouts']
      //   let workouts  = groups.getIn(locator, [])
      //   return groups.setIn(locator, workoutReducer(workouts, action))
      // }
      // else if (action.type.indexOf('EXERCISE')) {
      //   const locator = [action.group, 'workouts', action.workout, 'exercises'];
      //   let exercises = groups.getIn(locator, [])
      //   return groups.setIn(locator, exerciseReducer(exercises, action))
      // }
      return groups
  }
}

function workoutReducer(workouts, action) {
  switch (action.type) {
    case ACTIONS.ADD_WORKOUT:
    case ACTIONS.REMOVE_WORKOUT:
      throw new Error('Not implemented')
    default:
      return workouts
  }
}

function exerciseReducer(exercises, action) {
  switch (action.type) {
    case ACTIONS.ADD_EXERCISE:
    case ACTIONS.REMOVE_EXERCISE:
      throw new Error('Not implemented')
    default:
      return exercises
  }
}

export const reducers = combineReducers({ groups })