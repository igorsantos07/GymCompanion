import React, { Component } from 'react'
import Header from '../components/Header'
import WorkoutGroup from '../components/WorkoutGroup'

class MainScreen extends Component {

  render() {
    return (
      <main className="container">
        <Header/>
        <WorkoutGroup/>

      </main>
    )
  }

}

export default MainScreen