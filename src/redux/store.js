import { createStore } from 'redux'
import { reducers, initialState } from './reducers'

//noinspection JSUnresolvedVariable,JSUnresolvedFunction
export default createStore(
  reducers,
  initialState,
  window.__REDUX_DEVTOOLS_EXTENSION__ && window.__REDUX_DEVTOOLS_EXTENSION__()
)