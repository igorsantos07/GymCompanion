import { ACTIONS } from '../lib/constants'

export const addGroup       = () => ({ type: ACTIONS.ADD_GROUP })
export const setActiveGroup = id => ({ type: ACTIONS.SET_ACTIVE_GROUP, id })
export const addWorkout     = groupId => ({ type: ACTIONS.ADD_WORKOUT, groupId })