import React, { Component } from 'react'
import { connect } from 'react-redux'
import { addGroup, setActiveGroup } from '../redux/actions'
// import { pick } from 'lodash/object'

const state2props = state => {
  return {
    // groups: state.get('groups').map(group => pick(group, ['id', 'name', 'active'])).toJS()
    groups: state.get('groups').map(group => ({
      id: group.id,
      name: group.name,
      active: group.active
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
    const active = this.props.groups.filter(group => group.active)[0].id || 0
    return (
      <select value={active} onChange={this.onChange}>
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