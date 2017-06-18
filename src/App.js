import React from 'react'
import { Provider } from 'react-redux'
import store from './redux/store'
import MainScreen from './screens/MainScreen'

const App = props => <Provider store={store}><MainScreen {...props}/></Provider>

export default App;
