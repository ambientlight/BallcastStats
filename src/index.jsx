import * as React from 'react'
import { render } from 'react-dom'
import Amplify from 'aws-amplify';
import awsmobile from './aws-exports';
import Root from './Root'

import 'babel-polyfill';
import 'whatwg-fetch';
import { Auth } from 'aws-amplify'

// configure Amplify
Amplify.configure(awsmobile);

const root = document.createElement('div')
document.body.appendChild(root)
render(<Root />, root)

// Auth.signIn('ambientlight', '').then(user => {
//     console.log(user);
// });