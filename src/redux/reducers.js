/* eslint-disable eqeqeq */
import { fromJS } from 'immutable'
import { ACTIONS } from '../lib/constants'
import { combineReducers } from 'redux-immutable'
import Group from '../models/Group'

export const initialState = fromJS({
  groups: [new Group()],
})

/**
 * @param {Immutable.List<Group>} groups
 * @param {Object} action
 * @returns {*}
 */
const groups = (groups, action) => {
  switch (action.type) {
    case ACTIONS.ADD_GROUP:
      return groups
        .map(group => group.withStatus(false))
        .push(new Group())
        .sortBy(g => g.id).reverse()

    case ACTIONS.SET_ACTIVE_GROUP:
      return groups.map(group => group.withStatus(action.id == group.id))

    case ACTIONS.ARCHIVE_GROUP:
      throw new Error('Not implemented')

    default:
      if (action.type.indexOf('WORKOUT') != -1) {
        const [ key, group ] = groups.findEntry(g => g.id == action.groupId)
        return groups.set(key, workoutReducer(group, action))
      }
      // else if (action.type.indexOf('EXERCISE')) {
      //   const locator = [action.group, 'workouts', action.workout, 'exercises'];
      //   let exercises = groups.getIn(locator, [])
      //   return groups.setIn(locator, exerciseReducer(exercises, action))
      // }
      return groups
  }
}

/**
 *
 * @param {Group} group
 * @param action
 * @returns {*}
 */
function workoutReducer(group, action) {
  switch (action.type) {
    case ACTIONS.ADD_WORKOUT:
      return group.pushWorkout()

    case ACTIONS.REMOVE_WORKOUT:
      throw new Error('Not implemented')

    default:
      return group
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