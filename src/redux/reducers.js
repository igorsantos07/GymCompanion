import { fromJS } from 'immutable'
import { ACTIONS } from './actions'

export const initialState = fromJS({
  groups: []
})

export function mainReducer(state = initialState, action) {
  return {
    groups: (function(groups) {
      switch (action.type) {
        case ACTIONS.ADD_GROUP:
        case ACTIONS.ARCHIVE_GROUP:
          throw new Error('Not implemented')

        default:
          if (action.type.indexOf('WORKOUT')) {
            const locator = [action.group, 'workouts']
            let workouts = groups.getIn(locator, [])
            groups.setIn(locator, workoutReducer(workouts, action))
          }
          else if (action.type.indexOf('EXERCISE')) {
            const locator = [action.group, 'workouts', action.workout, 'exercises'];
            let exercises = groups.getIn(locator, [])
            groups.setIn(locator, exerciseReducer(exercises, action))
          }
      }
      return groups
    })(state.get('groups'))
  }
}

function workoutReducer(workouts, action) {
  switch (action.type) {
    case ACTIONS.ADD_WORKOUT:
    case ACTIONS.REMOVE_WORKOUT:
      throw new Error('Not implemented')
  }
}

function exerciseReducer(exercises, action) {
  switch (action.type) {
    case ACTIONS.ADD_EXERCISE:
    case ACTIONS.REMOVE_EXERCISE:
      throw new Error('Not implemented')
  }
}
