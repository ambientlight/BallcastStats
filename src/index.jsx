// polyfills
import 'babel-polyfill';
import 'whatwg-fetch';
import 'intl';

import * as React from 'react'
import { render } from 'react-dom'
import { addLocaleData } from 'react-intl'
import enLocaleData from 'react-intl/locale-data/en'
import ruLocaleData from 'react-intl/locale-data/ru'

addLocaleData(enLocaleData);
addLocaleData(ruLocaleData);

import Amplify from 'aws-amplify';
import awsmobile from './aws-exports';
import Root from './Root'

import { Auth } from 'aws-amplify'

// configure Amplify
Amplify.configure(awsmobile);

const root = document.createElement('div')
document.body.appendChild(root)
render(<Root />, root)