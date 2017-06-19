import { ACTIONS } from '../lib/constants'

export const addGroup       = (setActive = false) => ({ type: ACTIONS.ADD_GROUP, setActive })
export const setActiveGroup = (id) => ({ type: ACTIONS.SET_ACTIVE_GROUP, id })