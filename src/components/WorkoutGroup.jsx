import React, { Component } from 'react'
import { connect } from 'react-redux'
import Workout from './Workout'
import { addWorkout } from '../redux/actions'

const state2props = state => {
  const group = state.get('groups').filter(group => group.active).get(0)
  return { group }
}
const dispatch2props = dispatch => ({
  addWorkout: (groupId) => () => { dispatch(addWorkout(groupId)) }
})

class WorkoutGroup extends Component {


  constructor(props) {
    super(props)
    this.addWorkout = this.addWorkout.bind(this)
  }

  addWorkout() {
    this.props.addWorkout(this.props.group.id)()
  }

  render() {
    /** @type Group */
    const group = this.props.group
    if (!group) {
      return null
    }

    return (
      <section className="panel" id="workout-group">
        <div className="panel-block"><em>{group.description}</em></div>

        <nav className="panel-tabs">
          {group === undefined? null :
           group.workouts.map(w => <a key={w.id} className={w.active? 'is-active' : ''}>{w.name}</a>)}
          <a onClick={this.addWorkout.bind(this)}>➕</a>
        </nav>

        <Workout workout={group.workouts.filter(w => w.active).get(0)}/>
      </section>
    )
  }

}

export default connect(state2props, dispatch2props)(WorkoutGroup)