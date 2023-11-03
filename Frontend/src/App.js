import React, {useEffect, useState} from 'react'
import logo from './logo.svg'
import './App.css'

const data = {
  func: "getStartMessage"
}

const requestOptions = {
  method: "POST",
  headers: {
    "Content-Type": "application/json"
  },
  body: JSON.stringify(data)
}

function App() {

  const [data, setData] = useState('Loading...')

  useEffect(() => {
    fetch('/api', requestOptions)
      .then((res) => {
        if (res.ok) {
          return res.json()
        } else {
          return {message: 'Something went wrong'}
        }
      })
      .then((data) => setData(data.message))
  }, [])

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          {data}
        </p>
      </header>
    </div>
  )
}

export default App
