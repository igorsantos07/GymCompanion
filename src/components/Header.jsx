import React from 'react'
import GroupSelector from '../elements/GroupSelector'

const Header = props => (
  <header className="hero is-primary">
    <nav className="nav">
      <div className="nav-left">
        <div className="nav-item">
          <div className="emoji">💪</div>{/* Flexed biceps */}
          Gym<br/>Companion
        </div>
      </div>

      <div className="nav-center">
        <div className="nav-item">
          <GroupSelector/>
        </div>
      </div>

      <div className="nav-right">
        <div className="nav-item">
          <button className="button emoji">➡️⌚</button>{/* Arrow pointing to watch */}
        </div>
      </div>
    </nav>
  </header>
)

export default Header