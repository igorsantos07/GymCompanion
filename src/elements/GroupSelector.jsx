import React, { Component } from 'react'
import { connect } from 'react-redux'
import { addGroup, setActiveGroup } from '../redux/actions'

const state2props = state => {
  return {
    active: state.getIn(['groups', 'active']),
    groups: state.getIn(['groups', 'list']).map(group => ({
      id: group.id,
      name: group.name
    })).toJS()
  }
}

const dispatch2props = dispatch => ({
  addGroup : () => { dispatch(addGroup(true)) },
  setActive: id => { dispatch(setActiveGroup(id)) }
})

class GroupSelector extends Component {

  constructor(props) {
    super(props)
    this.onChange = this.onChange.bind(this)
  }

  onChange(e) {
    switch (e.target.value) {
      case 'new':
        this.props.addGroup()
        break

      default:
        this.props.setActive(e.target.value)
    }
  }

  render() {
    return (
      <select value={this.props.active} onChange={this.onChange}>
        {
          this.props.groups.length ?
            this.props.groups.map(group => <option key={group.id} value={group.id}>{group.name}</option>) :
            <option value={0} disabled>Select group</option>
        }
        <option value="new">Create new</option>
      </select>
    )
  }
}

export default connect(state2props, dispatch2props)(GroupSelector)