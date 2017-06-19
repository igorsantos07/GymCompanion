import React, { Component } from 'react'
import PropTypes from 'prop-types'
import Model from '../models/Exercise'

class Exercise extends Component {

  static propTypes = {
    exercise: PropTypes.instanceOf(Model)
  }

  render() {
    return <p>this.props.exercise.name</p>
  }

}

export default Exercise