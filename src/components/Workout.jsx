import React, { Component } from 'react'
import PropTypes from 'prop-types'
import Model from '../models/Workout'

class Workout extends Component {

  static propTypes = {
    workout: PropTypes.instanceOf(Model)
  }

  render() {
    return (
      <div>
        {this.props.workout.exercises.map(exercise => {
          return (
            <div className={`panel-block ${exercise.active? 'is-active' : ''}`}>
              <p className="title">${exercise.name}</p>
            </div>
          )
        })}

        <div className="panel-block">
          <button className="button is-primary is-fullwidth">➕ Add Exercise</button>
          <button className="button"><span className="emoji">↕</span>️ Reorder</button>
        </div>
      </div>
    )
  }

}

export default Workout