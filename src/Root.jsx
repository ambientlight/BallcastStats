import * as React from 'react'
import { hot } from 'react-hot-loader'

// contents of the module App gets compiled as array
// (by the order of definitions in App module, jsComponent is last)
const ReasonAppModule = require('reason/components/App.bs').App
const ReasonRoot = ReasonAppModule[ReasonAppModule.length - 1]

// Reason app gets wrapped for the sake of hot reloading
const Root = () => (
    <ReasonRoot title="Todos"></ReasonRoot>
)

export default hot(module)(Root)