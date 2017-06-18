import { createStore } from 'redux'
import { mainReducer as reducer, initialState } from './reducers'

//noinspection JSUnresolvedVariable,JSUnresolvedFunction
export default createStore(
  reducer,
  initialState,
  window.__REDUX_DEVTOOLS_EXTENSION__ && window.__REDUX_DEVTOOLS_EXTENSION__()
)