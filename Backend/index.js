import express from 'express'
import dotenv from 'dotenv'
import path from 'path'

const __dirname = path.resolve()
dotenv.config()
const PORT = process.env.PORT || 3000
const FRONTEND_PATH = process.env.FRONTEND_PATH || ''
const app = express()

app.use(express.json())

app.post('/api', (req, res) => {
  switch (req.body.func) {
    case 'getStartMessage': {
      res.json({message: 'Hello from backend!'})
      break
    }
    default: {
      res.json({message: `Handler for "${req.body.func}" not found`})
    }
  }
})

app.get('*', (req, res) => {
  let filePath = req.originalUrl
  if (!filePath.includes('.')) filePath = '/index.html'

  res.sendFile(path.resolve(__dirname, FRONTEND_PATH + filePath))
})

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`)
})